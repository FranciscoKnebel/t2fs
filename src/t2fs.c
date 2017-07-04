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
        return_value = FOUND_FILE_ERROR;
      } else if (return_value != DIRECTORY_NOT_FOUND) {
        if(isFreeLDAA() == TRUE) { // Possível abrir mais um arquivo?
          /* Arquivo não encontrado, logo pode criar. */
          file = createRecord(filename);

          /* salvar record no diretório */
          addRecordToDirectory(file, filename);
          //checks pra verificar que tudo deu certo ao adicionar

          /* adicionar para LDAA, e retornar valor do handle */
          int handle = insertLDAA(file);

          return_value = handle;
        } else {
          return_value = FILE_LIMIT_REACHED;
        }
      }
      break;
  }

  return return_value;
};

int delete2 (char *filename) {
  if (!config.initiated) {
    initConfig();
  }

  return -1;
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

  return -1;
};

int rmdir2 (char *pathname) {
  if (!config.initiated) {
    initConfig();
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
  if(searchLDAA(handle, 2, &descritorDir) == TRUE){
    REGISTER_T reg;
      if(readRegister(descritorDir.record.MFTNumber, &reg) != TRUE) {
      return -1;
      }
      else{
        struct t2fs_4tupla *tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));
        parseRegister(reg.at, tuplas);
        
        int i = 0;
        int j = 0;
        struct t2fs_record tempRecord;
        DIRENT2 dentryTemp;

        for (i = 0; i < constants.MAX_TUPLAS_REGISTER; ++i){
          int atributeType = tuplas[i].atributeType;
          switch(atributeType){
          case 2: //MFT adicional
              // ler novo MFT Register, indicado pelo número de bloco em tuplas[i].virtualBlockNumber
              if(readRegister(tuplas[i].virtualBlockNumber, &reg) != TRUE) {
                return REGISTER_READ_ERROR;
              }
              free(tuplas);
              tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));

              parseRegister(reg.at, tuplas);
              i = 0;
              break;
            case 1: //VBN-LBN
              for (j = 0; j < constants.RECORD_PER_BLOCK; ++j)
              {
                readRecord(tuplas[i].atributeType, j, &tempRecord);
                int typeVal = tempRecord.TypeVal;

                switch(typeVal){

                  case 1: //arquivo ou subdiretório
                  case 2:

                    strcpy(dentryTemp.name, tempRecord.name);
                    dentryTemp.fileType = tempRecord.TypeVal;
                    dentryTemp.fileSize = tempRecord.bytesFileSize;

                    memcpy(dentry, &dentryTemp, sizeof(DIRENT2));
                    descritorDir.currentPointer += RECORD_SIZE;

                    break;

                  case 0: //invalido => erro?
                  default:
                    j = constants.RECORD_PER_BLOCK;
                    break;

                }
              }
              break;
            case 0: //FIM
              i = constants.MAX_TUPLAS_REGISTER;
              break;
            default:
            case -1: //LIVRE => ERRO 
              // -END_OF_DIR;
              return -1;
              break;
          }
        }
        return 0;
      }

  }
  else
    return NOT_FOUND_LDAA;
};

int closedir2 (DIR2 handle) {
  if (!config.initiated) {
    initConfig();
  }

  return closeFile(handle, TYPEVAL_DIRETORIO);
};
