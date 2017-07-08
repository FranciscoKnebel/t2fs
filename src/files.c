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
          check = addRecordToDirectory(file, filename);
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

  root = initRecord(TYPEVAL_DIRETORIO, filename, -1, -1, REGISTER_ROOT);
  if (isFreeLDAA() == TRUE) {
    return_value = insertLDAA(root, "/");
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
          return_value = insertLDAA(file, filename);
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

int removeFileFromDirectory(DWORD directoryMFTNumber, struct t2fs_record file) {
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

  int i = 0, amountOfBlocksRead = 0, removedFile = FALSE, block, fileBlocksCounter = 0;

  while (i < constants.MAX_TUPLAS_REGISTER && removedFile != TRUE) {
    switch (tuplas[i].atributeType) {
      case REGISTER_MAP:
        while(amountOfBlocksRead < tuplas[i].numberOfContiguosBlocks && removedFile != TRUE) {
          block = tuplas[i].logicalBlockNumber + amountOfBlocksRead;
          amountOfBlocksRead++;

          if(readBlock(block, &blockBuffer) == FALSE) {
            return FALSE;
          };

          parseDirectory(blockBuffer, records);

          int j;
          for (j = 0; j < constants.RECORD_PER_BLOCK && removedFile != TRUE; j++) {
            if(strcmp(records[j].name, file.name) == 0 && (records[j].TypeVal == TYPEVAL_REGULAR || records[j].TypeVal == TYPEVAL_DIRETORIO)) {
              // Achou arquivo válido com o mesmo nome
              file.TypeVal = TYPEVAL_INVALIDO;

              if(writeRecord(tuplas[i].logicalBlockNumber, j, file) == FALSE) {
                return RECORD_WRITE_ERROR;
              };

              removedFile = TRUE;
              return_value = i;
            }
          }
        }

        fileBlocksCounter += amountOfBlocksRead;
        amountOfBlocksRead = 0;

        if(removedFile != TRUE) {
          i++;
        }

      break;
    case REGISTER_FIM:
      // CHEGOU AO FIM DAS TUPLAS E NÃO ACHOU
      return REGISTER_FIM;
      break;
    case REGISTER_ADITIONAL:
      // Ler novo registro e recomeçar a leitura.
      registerIndex = tuplas[i].virtualBlockNumber;

      if(readRegister(registerIndex, &reg) != TRUE) {
        return FALSE;
      }
      free(tuplas);
      tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));

      parseRegister(reg.at, tuplas);
      i = 0; // reset i para 0, começar a ler tuplas novamente

      break;
    default:
      return_value = FIND_REGISTER_FREE;
      i++;
      break;
    }
  }

  return return_value;
}

int removeFileFromMFT(struct t2fs_record file) {
  int registerIndex = file.MFTNumber;
  int check, return_value;
  DWORD temp;
  BLOCK_T blockBuffer;
  blockBuffer.at = malloc(sizeof(unsigned char) * constants.BLOCK_SIZE);

  REGISTER_T reg;
  if(readRegister(registerIndex, &reg) != TRUE) {
    return FALSE;
  }

  struct t2fs_4tupla *tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));
  parseRegister(reg.at, tuplas);


  /* Desalocando registro do MFT usado. */
  check = setMFT(registerIndex, MFT_BM_LIVRE);
  if (check < 0) {
    return MFT_BM_ERROR;
  }

  // Marca registro como livre, para uso das funções de MFT.
  /* TO DO */
  /* função única para isso, passando indice do registro */
  /* não deve ser preciso reescrever apenas para mudar um número */
  temp = tuplas[0].atributeType;
  tuplas[0].atributeType = REGISTER_FREE;
  writeTupla(reg.at, &tuplas[0], 0);
  writeRegister(registerIndex, &reg);
  tuplas[0].atributeType = temp; // Restaura para poder iterar

  /* Desalocar blocos indicados pelas tuplas */
  int i = 0, amountOfBlocksRead = 0, fileBlocksCounter = 0, block;
  while (i < constants.MAX_TUPLAS_REGISTER) {
    switch (tuplas[i].atributeType) {
      case REGISTER_MAP:
        while(amountOfBlocksRead < tuplas[i].numberOfContiguosBlocks) {
          block = tuplas[i].logicalBlockNumber + amountOfBlocksRead;
          amountOfBlocksRead++;

          check = setBitmap2(block, BM_LIVRE);
          if (check < 0) {
            return BM_ERROR;
          }
        }

        fileBlocksCounter += amountOfBlocksRead;
        amountOfBlocksRead = 0;

        i++;
        break;
      case REGISTER_ADITIONAL:
        registerIndex = tuplas[i].virtualBlockNumber;

        if(readRegister(registerIndex, &reg) != TRUE) {
          return FALSE;
        }

        /* Desalocando registro do MFT usado. */
        check = setMFT(registerIndex, MFT_BM_LIVRE);
        if (check < 0) {
          return MFT_BM_ERROR;
        }

        free(tuplas);
        tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));

        parseRegister(reg.at, tuplas);
        i = 0; // reset i para 0, começar a ler tuplas novamente

        // Marca registro como livre, para uso das funções de MFT.
        temp = tuplas[0].atributeType;
        tuplas[0].atributeType = REGISTER_FREE;
        writeTupla(reg.at, &tuplas[0], 0);
        writeRegister(registerIndex, &reg);
        tuplas[0].atributeType = temp; // Restaura para poder iterar

        break;
      case REGISTER_FIM:
        return REGISTER_FIM;
        break;
      default:
        return_value = tuplas[i].atributeType;
        i++;
        break;
    }
  }

  return return_value;
}

int deleteFileFromDisk(struct t2fs_record file, char* filename) {
  /* Remover record do diretório */
  char * directoryname = malloc(strlen(filename));
  int return_value = -1;

  getFileDirectory(filename, directoryname);

  if(strcmp("/", directoryname) == 0) { // remover da root
    return_value = removeFileFromDirectory(REGISTER_ROOT, file);
  } else {
    struct t2fs_record directory;

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
        return_value = removeFileFromDirectory(directory.MFTNumber, file);
        break;
    }
  }

  /* Desalocar blocos e registros do arquivo */
  removeFileFromMFT(file);

  return 0;
}
