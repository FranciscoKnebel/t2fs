/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Douglas Lazaro
  Francisco Knebel
*/

#include "libs.h"

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
