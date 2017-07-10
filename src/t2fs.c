/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Douglas Lazaro
  Francisco Knebel
*/

#include "libs.h"

struct Configuration config = { .initiated = FALSE };
struct Constants constants;

int identify2 (char *name, int size) {
  if (size == 0) {
    name[0] = '\0';

    return 0;
  }

  char info[] = "\n Douglas Lazaro S P Silva \t- 207305 \n Francisco Paiva Knebel \t- 243688";
  int length = strlen(info);

  if(size < length) {
    memcpy(name, info, size);
    name[size] = '\0';
  } else {
    memcpy(name, info, length);
    name[length] = '\0';
  }

  return 0;
};

FILE2 create2 (char *filename) {
  if (!config.initiated) {
    initConfig();
  }

  return createNewFile(filename, TYPEVAL_REGULAR);
};

int delete2 (char *filename) {
  if (!config.initiated) {
    initConfig();
  }

  struct t2fs_record file;
  int return_value = lookup(filename, &file);

  int handle, check;

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
        // Remover da lista de arquivos abertos
        handle = findByNameLDAA(filename);
        if(handle >= 0) {
          removeLDAA(handle);
        }

        // Remover arquivo do disco
        check = deleteFileFromDisk(file, filename);
        if(check < 0) {
          return_value = check;
        } else {
          return_value = 0;
        }
      }
      break;
  }

  return return_value;
};

FILE2 open2 (char *filename) {
  if (!config.initiated) {
    initConfig();
  }

  return openFile(filename);
};

int close2 (FILE2 handle) {
  if (!config.initiated) {
    initConfig();
  }

  return closeFile(handle, TYPEVAL_REGULAR);
};

int read2 (FILE2 handle, char *buffer, int size) {
  if (!config.initiated) {
    initConfig();
  }

  int check, return_value;
  struct descritor descritor;

  check = searchLDAA(handle, TYPEVAL_REGULAR, &descritor);
  if(check == FALSE) {
    return_value = -1;
  } else {
    return_value = readFile(handle, descritor, buffer, size);
  }

  return return_value;
};

int write2 (FILE2 handle, char *buffer, int size) {
  if (!config.initiated) {
    initConfig();
  }

  int check, return_value;
  struct descritor descritor;

  check = searchLDAA(handle, TYPEVAL_REGULAR, &descritor);
  if(check == FALSE) {
    return_value = -1;
  } else {
    return_value = writeFile(handle, descritor, buffer, size);
  }

  return return_value;
};

int truncate2 (FILE2 handle) {
  if (!config.initiated) {
    initConfig();
  }

  return -1;
};

int seek2 (FILE2 handle, DWORD offset) {
  if (!config.initiated) {
    initConfig();
  }
  struct descritor descritor;
  if(searchLDAA(handle, TYPEVAL_REGULAR, &descritor) == TRUE) {
    if (offset == (unsigned int) EOF_OFFSET || offset > descritor.record.bytesFileSize)
      descritor.currentPointer = descritor.record.bytesFileSize;
    else
      descritor.currentPointer = offset;

  return updateLDAA(handle, TYPEVAL_REGULAR, descritor);
  }
  else
    return NOT_FOUND_LDAA;
};

int mkdir2 (char *pathname) {
  if (!config.initiated) {
    initConfig();
  }

  if(createNewFile(pathname, TYPEVAL_DIRETORIO) >= 0) {
    return 0;
  }
  return -1;
};

int rmdir2 (char *pathname) {
  if (!config.initiated) {
    initConfig();
  }

  if(delete2(pathname) == 0) {
    return 0;
  }
  return -1;
};

DIR2 opendir2 (char *pathname) {
  if (!config.initiated) {
    initConfig();
  }

  return openFile(pathname);
};

int readdir2 (DIR2 handle, DIRENT2 *dentry) {
  if (!config.initiated) {
    initConfig();
  }

  struct descritor descritorDir;
  if(searchLDAA(handle, TYPEVAL_DIRETORIO, &descritorDir) == TRUE) {

    REGISTER_T reg;
    if(readRegister(descritorDir.record.MFTNumber, &reg) != TRUE) {
      return REGISTER_READ_ERROR;
    }
    else {
      struct t2fs_4tupla *tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));
      parseRegister(reg.at, tuplas);

      /* TO DO */
      /* usar current pointer para definir valores de i e j */
      /* precisa condição pra ler as tuplas adicionais */

      struct t2fs_record records[constants.RECORD_PER_BLOCK]; //loop
      struct t2fs_record record;
      BLOCK_T blockBuffer;
      blockBuffer.at = malloc(sizeof(unsigned char) * constants.BLOCK_SIZE);

      int i = 0, j = 0, indexRecord = 0, amountOfBlocksRead = 0, fimDir = FALSE, block, fileBlocksCounter = 0;

      int entry = descritorDir.currentPointer/RECORD_SIZE;

      if(entry > constants.RECORD_PER_BLOCK) // > OU >=
        indexRecord = constants.RECORD_PER_BLOCK - (entry % constants.RECORD_PER_BLOCK);
      else
        indexRecord = entry;

      // while (i < constants.MAX_TUPLAS_REGISTER && fimDir != TRUE) {
        switch(tuplas[i].atributeType) {
          case REGISTER_ADITIONAL:
              // ler novo MFT Register, indicado pelo número de bloco em tuplas[i].virtualBlockNumber
              if(readRegister(tuplas[i].virtualBlockNumber, &reg) != TRUE) {
                return REGISTER_READ_ERROR;
              }

              free(tuplas);
              tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));

              parseRegister(reg.at, tuplas);
              i = 0;
              // break; /* Tirando esse break, depois de ele ler o registro adicional ele vai executar o comportamento esperado, que é de MAP, na primeira tupla */

          case REGISTER_MAP: // VBN-LBN
            /* TO DO */
            /* Uso do current pointer para indicar qual arquivo deve retornar */
            /* Leitura de blocos contíguos */

             readRecord(tuplas[i].logicalBlockNumber, j, &record);
             DIRENT2 dentryTemp;

             switch(record.TypeVal) {
                case TYPEVAL_REGULAR:
                case TYPEVAL_DIRETORIO:
                  dentryTemp = initDentry(record);

                  descritorDir.currentPointer += RECORD_SIZE;
                  memcpy(dentry, &dentryTemp, sizeof(DIRENT2));
                  updateLDAA(handle, TYPEVAL_DIRETORIO, descritorDir);

                  return 0;
                  break;
                case TYPEVAL_INVALIDO:
                default:
                while(amountOfBlocksRead < tuplas[i].numberOfContiguosBlocks && fimDir != TRUE) {
                  block = tuplas[i].logicalBlockNumber + amountOfBlocksRead;
                  amountOfBlocksRead++;

                  if(readBlock(block, &blockBuffer) == FALSE) {
                    return FALSE;
                  };

                  parseDirectory(blockBuffer, records);

                  entry = descritorDir.currentPointer/RECORD_SIZE;

                  if(entry > constants.RECORD_PER_BLOCK) // > OU >=
                    indexRecord = constants.RECORD_PER_BLOCK - (entry % constants.RECORD_PER_BLOCK);
                  else
                    indexRecord = entry;

                  for (j=indexRecord; j < constants.RECORD_PER_BLOCK && fimDir != TRUE; j++) {
                    switch(records[j].TypeVal) {
                      case TYPEVAL_REGULAR:
                      case TYPEVAL_DIRETORIO:
                        dentryTemp = initDentry(records[j]);

                        descritorDir.currentPointer += RECORD_SIZE *j+1;
                        memcpy(dentry, &dentryTemp, sizeof(DIRENT2));
                        updateLDAA(handle, TYPEVAL_DIRETORIO, descritorDir);

                        return 0;
                        break;
                      case TYPEVAL_INVALIDO:
                      default:
                        // Passa para o proximo record
                        break;
                    }
                  }
                  fileBlocksCounter = amountOfBlocksRead;
                }
              }

            break;
          case REGISTER_FIM:
          case REGISTER_FREE:
          default:
            fimDir = TRUE;
            return -END_OF_DIR;
            break;
      }

     /* i++; incrementa contador de tuplas
    }*/
    return READDIR_ERROR;
  }
}
  else {
    return NOT_FOUND_LDAA;
  }
};

int closedir2 (DIR2 handle) {
  if (!config.initiated) {
    initConfig();
  }

  return closeFile(handle, TYPEVAL_DIRETORIO);
};
