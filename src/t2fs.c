/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

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
    default:
      if(return_value >= 0) {
        return_value = FOUND_FILE_ERROR;
      } else if (return_value != DIRECTORY_NOT_FOUND) {
        if(getFreeLDAA() < 0) { // Possível abrir mais um arquivo?
          // Arquivo não encontrado, logo pode criar.
          file = createRecord(filename);

          /* salvar record no diretório */
          addRecord(file, filename);

          /* adicionar para LDAA, e retornar valor do handle */
          int handle = insertLDAA(file);

          return_value = handle;
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

  return -1;
};

int close2 (FILE2 handle) {
  if (!config.initiated) {
    initConfig();
  }

  return -1;
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

  return -1;
};

int readdir2 (DIR2 handle, DIRENT2 *dentry) {
  if (!config.initiated) {
    initConfig();
  }

  return -1;
};

int closedir2 (DIR2 handle) {
  if (!config.initiated) {
    initConfig();
  }

  return -1;
};
