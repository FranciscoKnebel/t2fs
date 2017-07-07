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
        }

        return_value = 0;
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

  return -1;
};

int write2 (FILE2 handle, char *buffer, int size) {
  if (!config.initiated) {
    initConfig();
  }

  return -1;
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

  return -1;
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
    } else {
      struct t2fs_4tupla *tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));
      parseRegister(reg.at, tuplas);

      /* TO DO */
      /* usar current pointer para definir valores de i e j */
      /* precisa condição pra ler as tuplas adicionais */

      int i = 0, j = 0;
      struct t2fs_record record;

      //for (i = 0; i < constants.MAX_TUPLAS_REGISTER; ++i) {
        int atributeType = tuplas[i].atributeType;

        switch(atributeType) {
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
            /* Desnecessário ambos loops FOR, pois só retornamos um arquivo por vez, e sabemos qual arquivo deve ser lido devido ao current pointer. */

            /* Leitura de blocos contíguos */

            //for (j = 0; j < constants.RECORD_PER_BLOCK; ++j) {
              readRecord(tuplas[i].logicalBlockNumber, j, &record);
              int typeVal = record.TypeVal;
              DIRENT2 dentryTemp;

              switch(typeVal) {
                case TYPEVAL_REGULAR:
                case TYPEVAL_DIRETORIO:
                  dentryTemp = initDentry(record);

                  descritorDir.currentPointer += RECORD_SIZE;
                  memcpy(dentry, &dentryTemp, sizeof(DIRENT2));
                  /* TO DO */
                  /* update current pointer na lista */
                  /* criar função de update na LDAA */
                  /* já deixei a declaração da função no ldaa.h, e o uso está comentado abaixo. */

                  //updateLDAA(handle, TYPEVAL_DIRETORIO, descritorDir);

                  return 0;
                  break;
                case TYPEVAL_INVALIDO:
                default:
                  /* TO DO */
                  /* Ler o restante do bloco (aqui precisa de um loop) até achar algum válido. */
                  /*  Se existir, retornar o dentry e atualizar o current pointer até esse arquivo. */
                  /*  Se não, verificar se possuí blocos contiguos. */
                  /*    Se sim, ler novo bloco e procurar por record, com a mesma lógica acima. */
                  /*    Se não, chegou ao final do diretório e retornar -END_OF_DIR. */
                  break;
              }
            //}
            break;
          case REGISTER_FIM:
          case REGISTER_FREE:
          default:
            return -END_OF_DIR;
            break;
        }
      //}
    }

    return READDIR_ERROR;
  } else {
    return NOT_FOUND_LDAA;
  }
};

int closedir2 (DIR2 handle) {
  if (!config.initiated) {
    initConfig();
  }

  return closeFile(handle, TYPEVAL_DIRETORIO);
};
