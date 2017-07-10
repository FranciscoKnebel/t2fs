/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Douglas Lazaro
  Francisco Knebel
*/

#include "libs.h"

struct t2fs_record createRecord(char* pathname, BYTE typeVal) {
  struct t2fs_record newFile;

  char ** parsedPath = malloc(sizeof(char) * MAX_FILE_NAME_SIZE);
  int parseCount = parsePath(pathname, parsedPath);

  newFile = initRecord(typeVal, parsedPath[parseCount], 1, 0, -1);

  /* Encontrar registro do MFT livre, escrever registro e associar ao arquivo. */
  int check;

  int fileLBN = searchBitmap2(BM_LIVRE); // Encontra bloco de dados para o arquivo
  check = setBitmap2(fileLBN, BM_OCUPADO);
  /*
  if (check < 0) {
    return BM_ERROR;
  }
  */

  int registro = searchMFT(MFT_BM_LIVRE);
  initFileRegister(registro, fileLBN); // Inicializa registro no MFT do arquivo
  check = setMFT(registro, MFT_BM_OCUPADO);    // seta registro como ocupado no bitmap.
  /*
  if (check < 0) {
    return MFT_BM_ERROR;
  }
  */

  newFile.MFTNumber = registro; // Associa arquivo ao registro criado

  return newFile;
}

int createNewFile(char * filename, BYTE typeVal) {
  struct t2fs_record file;
  int return_value = lookup(filename, &file);
  int check;

  switch (return_value) {
    case REGISTER_READ_ERROR:
      printf("Erro crítico na leitura de um registro no lookup.\n");
      break;
    case FIND_REGISTER_ADITIONAL:
      printf("Erro! Valor de retorno de lookup nunca deve ser FIND_REGISTER_ADITIONAL.\n");
      break;
    case PARSED_PATH_ERROR:
      printf("Path '%s' inválida.\n", filename);
      break;
    default:
      if(return_value >= 0) {
        return_value = FOUND_FILE_ERROR;
      } else if (return_value != DIRECTORY_NOT_FOUND) {
        if(canAddToLDAA(filename)) { // Possível criar mais um arquivo?
          /* Arquivo não encontrado, logo pode criar. */
          file = createRecord(filename, typeVal);

          /* salvar record no diretório */
          check = addRecordToDirectory(file, filename, FALSE);
          if(check < 0) {
            return check;
          }

          /* adicionar para LDAA, e retornar valor do handle */
          int handle = insertLDAA(file, filename);

          return_value = handle;
        } else {
          if(isFreeLDAA() == TRUE) {
            return_value = FILE_LIMIT_REACHED;
          } else {
            return_value = NOT_FOUND_LDAA;
          }
        }
      }
      break;
  }

  return return_value;
}

int addToDirectory(DWORD directoryMFTNumber, struct t2fs_record record, int updatingRecord) {
  int return_value;
  int registerIndex = directoryMFTNumber;

  REGISTER_T reg;
  if(readRegister(registerIndex, &reg) != TRUE) {
    return FALSE;
  }

  struct t2fs_4tupla *tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));
  parseRegister(reg.at, tuplas);

  struct t2fs_record records[constants.RECORD_PER_BLOCK];
  BLOCK_T blockBuffer;
  blockBuffer.at = malloc(sizeof(unsigned char) * constants.BLOCK_SIZE);

  unsigned int i = 0, amountOfBlocksRead = 0, foundSpaceToAdd = FALSE, block, fileBlocksCounter = 0;
  int allocated;
  while (i < constants.MAX_TUPLAS_REGISTER && foundSpaceToAdd != TRUE) {
    switch (tuplas[i].atributeType) {
      case REGISTER_MAP:
        while(amountOfBlocksRead < tuplas[i].numberOfContiguosBlocks && foundSpaceToAdd != TRUE) {
          block = tuplas[i].logicalBlockNumber + amountOfBlocksRead;
          amountOfBlocksRead++;

          if(readBlock(block, &blockBuffer) == FALSE) {
            return FALSE;
          };

          parseDirectory(blockBuffer, records);

          unsigned int j;
          for (j = 0; j < constants.RECORD_PER_BLOCK && foundSpaceToAdd != TRUE; j++) {
            if((records[j].TypeVal != TYPEVAL_REGULAR && records[j].TypeVal != TYPEVAL_DIRETORIO) || (updatingRecord == TRUE && strcmp(record.name, records[j].name) == 0)){
              // ADICIONAR RECORD PARA O DIRETÓRIO
              if(writeRecord(tuplas[i].logicalBlockNumber, j, record) == FALSE) {
                return RECORD_WRITE_ERROR;
              };

              foundSpaceToAdd = TRUE;
              return_value = i;
            }
          }
        }

        fileBlocksCounter += amountOfBlocksRead;
        amountOfBlocksRead = 0;

        if(foundSpaceToAdd != TRUE) {
          i++;
        }

        break;
      case REGISTER_FIM:
        // CHEGOU AO FIM DAS TUPLAS
        block = tuplas[i-1].logicalBlockNumber + tuplas[i-1].numberOfContiguosBlocks;
        allocated = getBitmap2(block);

        if(allocated < 0) {
          return BM_ERROR;
        }

        // Possível adicionar mais um bloco contiguo para a tupla
        if(allocated == BM_LIVRE) {
          tuplas[i-1].numberOfContiguosBlocks += 1;
          writeTupla(reg.at, &tuplas[i-1], i-1);
          setBitmap2(block, BM_OCUPADO);

          resetBlock(block);

          writeRegister(registerIndex, &reg);

          if(writeRecord(block, 0, record) == FALSE) {
            return RECORD_WRITE_ERROR;
          };

          foundSpaceToAdd = TRUE;
          return_value = ADDDIR_CONTIGUOUS;
        } else if(i == constants.MAX_TUPLAS_REGISTER - 1) { // tupla atual no final do registro?
          /* Encontra indice para o novo registro. */
          int novoRegisterIndex = searchMFT(MFT_BM_LIVRE);
          int check = setMFT(novoRegisterIndex, MFT_BM_OCUPADO); // seta registro como ocupado no bitmap.
          if (check < 0) {
            return MFT_BM_ERROR;
          }

          // Tupla atual vira um REGISTER_ADITIONAL, para indicar o novo registro do arquivo, encontrado no indice novoRegisterIndex
          tuplas[i] = initTupla(REGISTER_ADITIONAL, novoRegisterIndex, 0, 0);
          writeTupla(reg.at, &tuplas[i], i);
          writeRegister(registerIndex, &reg);

          /* Operações no novo registro */
          int fileLBN;

          fileLBN = searchBitmap2(BM_LIVRE); // Encontra bloco de dados para o arquivo
          check = setBitmap2(fileLBN, BM_OCUPADO);
          if (check < 0) {
            return BM_ERROR;
          }

          // Inicializa o novo registro.
          initNewRegister(novoRegisterIndex, fileBlocksCounter, fileLBN);

          // Adiciona record para o inicio do bloco, do novo registro.
          if(writeRecord(fileLBN, 0, record) == FALSE) {
            return RECORD_WRITE_ERROR;
          };

          foundSpaceToAdd = TRUE;
          return_value = ADDDIR_ADITIONAL;
        } else {
          /* Não está no final, logo a tupla atual vira REGISTRO_MAP para novos blocos do arquivo. */

          /* ATUALIZAÇÃO DO REGISTRO */
          readRegister(registerIndex, &reg);

          int newLBN = searchBitmap2(BM_LIVRE); // Encontra bloco de dados para o arquivo
          int check = setBitmap2(newLBN, BM_OCUPADO);
          if (check < 0) {
            return BM_ERROR;
          }

          resetBlock(newLBN);

          // Tupla atual vira um MAP, para os novos blocos do arquivo.
          tuplas[i] = initTupla(REGISTER_MAP, fileBlocksCounter, newLBN, 1);
          writeTupla(reg.at, &tuplas[i], i);

          // Tupla seguinte vira o novo fim do registro.
          tuplas[i+1] = initTupla(REGISTER_FIM, 0, 0, 0);
          writeTupla(reg.at, &tuplas[i+1], i+1);

          // Atualização do registro com novos valores.
          writeRegister(registerIndex, &reg);

          /* ATUALIZAÇÃO DO BLOCO DE DIRETÓRIO */
          if(writeRecord(tuplas[i].logicalBlockNumber, 0, record) == FALSE) {
            return RECORD_WRITE_ERROR;
          };

          foundSpaceToAdd = TRUE;
          return_value = ADDDIR_MAP;
        }
        break;
      case REGISTER_ADITIONAL: // LER NOVO REGISTRO E RECOMEÇAR LEITURA
        registerIndex = tuplas[i].virtualBlockNumber;

        if(readRegister(registerIndex, &reg) != TRUE) {
          return FALSE;
        }
        free(tuplas);
        tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));

        parseRegister(reg.at, tuplas);
        i = 0; // reset i para 0, começar a ler tuplas novamente

        break;
      case REGISTER_FREE:
      default:
        return_value = FIND_REGISTER_FREE;
        i++;
        break;
    }
  }

  return return_value;
}

int addRecordToDirectory(struct t2fs_record record, char * filename, int updatingRecord) {
  struct t2fs_record directory;
  char * directoryname = malloc(strlen(filename));
  int return_value = FALSE;

  getFileDirectory(filename, directoryname);

  if(strcmp("/", directoryname) == 0) { // adicionar para a root
    return_value = addToDirectory(REGISTER_ROOT, record, updatingRecord);

    return return_value;
  }

  return_value = lookup(directoryname, &directory);

  switch (return_value) {
    case REGISTER_READ_ERROR:
      printf("Erro crítico na leitura de um registro.\n");
      break;
    case FIND_REGISTER_ADITIONAL:
      printf("ERRO! Valor de retorno de lookup nunca deve ser FIND_REGISTER_ADITIONAL.\n");
      break;
    case FIND_REGISTER_NOTFOUND:
    case FIND_REGISTER_FIM:
    case FIND_REGISTER_FREE:
      printf("Diretório '%s' não encontrado.\n", directoryname);
      break;
    default:
      return_value = addToDirectory(directory.MFTNumber, record, updatingRecord);
      break;
  }

  return return_value;
}
