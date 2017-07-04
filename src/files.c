/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Douglas Lazaro
  Francisco Knebel
*/

#include "libs.h"

struct t2fs_4tupla initTupla(DWORD atributeType, DWORD VBN, DWORD LBN, DWORD numberOfContiguosBlocks) {
  struct t2fs_4tupla tupla;

  tupla.atributeType = REGISTER_MAP;
  tupla.virtualBlockNumber = VBN;
  tupla.logicalBlockNumber = LBN;
  tupla.numberOfContiguosBlocks = numberOfContiguosBlocks;

  return tupla;
}

int initFileRegister(int registerIndex, int LBN) {
  struct t2fs_4tupla tupla, tuplaFim;

  tupla     = initTupla(REGISTER_MAP, 0, LBN, 1);
  tuplaFim  = initTupla(REGISTER_FIM, 0, 0, 0);

  REGISTER_T reg;
  readRegister(registerIndex, &reg);

  writeTupla(reg.at, &tupla,    0);
  writeTupla(reg.at, &tuplaFim, 1);

  writeRegister(registerIndex, &reg);

  return 1;
}

struct t2fs_record createRecord(char* pathname) {
  struct t2fs_record newFile;

  char ** parsedPath = malloc(sizeof(char) * MAX_FILE_NAME_SIZE);
  int parseCount = parsePath(pathname, parsedPath);

  memset(&newFile, 0, sizeof(newFile));

  newFile.TypeVal = TYPEVAL_REGULAR;
  strcpy(newFile.name, parsedPath[parseCount]); // name é o último elemento parseado: /directory/directory/file1 -> name = file1
  newFile.blocksFileSize = 1;
  newFile.bytesFileSize = 0;

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

int addToDirectory(DWORD directoryMFTNumber, struct t2fs_record record) {
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

  int i = 0, amountOfBlocksRead = 0, foundSpaceToAdd = FALSE, block, allocated, fileBlocksCounter = 0;
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

          int j;
          for (j = 0; j < constants.RECORD_PER_BLOCK && foundSpaceToAdd != TRUE; j++) {
            if(records[j].TypeVal != TYPEVAL_REGULAR && records[j].TypeVal != TYPEVAL_DIRETORIO) {
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

          writeRegister(registerIndex, &reg);

          if(writeRecord(block, 0, record) == FALSE) {
            return RECORD_WRITE_ERROR;
          };
        } else if(i == constants.MAX_TUPLAS_REGISTER - 1) { // tupla atual no final do registro?
          /*
            Se sim,
              tupla[i] vira REGISTER_ADITIONAL
              encontrar registro livre com o Bitmap MFT.
              passar registro para tupla[i]
              setar registro como ocupado.

              criar novo registro MAP, verificando blocos livres e setando ocupação.
              no novo bloco da primeira nova tupla, adicionar o record.
          */
        } else {
          /* Não está no final, logo a tupla atual vira REGISTRO_MAP para novos blocos do arquivo. */

          /* ATUALIZAÇÃO DO REGISTRO */
          REGISTER_T reg;
          readRegister(registerIndex, &reg);

          int newLBN = searchBitmap2(BM_LIVRE); // Encontra bloco de dados para o arquivo
          int check = setBitmap2(newLBN, BM_OCUPADO);
          if (check < 0) {
            return BM_ERROR;
          }

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

int addRecordToDirectory(struct t2fs_record record, char * filename) {
  struct t2fs_record directory;
  char * directoryname = malloc(strlen(filename));
  int return_value = FALSE;

  getFileDirectory(filename, directoryname);

  if(strcmp("/", directoryname) == 0) { // adicionar para a root
    return_value = addToDirectory(REGISTER_ROOT, record);

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
      return_value = addToDirectory(directory.MFTNumber, record);

      break;
  }

  return return_value;
}

int openRoot(char* filename) {
  int return_value;
  struct t2fs_record root;

  memset(&root, 0, sizeof(root));

  root.TypeVal = TYPEVAL_DIRETORIO;
  strcpy(root.name, filename);
  root.blocksFileSize = -1;
  root.bytesFileSize = -1;
  root.MFTNumber = REGISTER_ROOT;

  if (isFreeLDAA() == TRUE) {
    return_value = insertLDAA(root);
  } else {
    return_value = FILE_LIMIT_REACHED;
  }

  return return_value;
}

int openFile(char* filename) {
  if(strcmp(filename, "/") == 0) {
    return openRoot(filename);
  }

  struct t2fs_record file;
  int return_value = lookup(filename, &file);

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
        if (isFreeLDAA() == TRUE) {
          return_value = insertLDAA(file);
        } else {
          return_value = FILE_LIMIT_REACHED;
        }
      } else {
        return_value = FILE_NOT_FOUND;
      }
  }

  return return_value;
}

int closeFile(int handle, BYTE TypeVal) {
  int return_value = -1;
  struct descritor result;

  if(searchLDAA(handle, TypeVal, &result) == TRUE) {
    if (removeLDAA(handle) == TRUE) {
      return_value = 0;
    } else {
      return_value = ERRO_REMOVE_LDAA;
    }
  } else {
    return_value = NOT_FOUND_LDAA;
  }

  return return_value;
}
