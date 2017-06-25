#include "libs.h"

int validPath(char* path) {
  int lastWasSlash = FALSE;

  int length = strlen(path);

  // Min Size || Path não começa no root || Max Size
  if(length == 0 || path[0] != '/' || length > MAX_FILE_NAME_SIZE) {
    return FALSE;
  }

  // Dupla Barra
  int i;
  for(i = 0; i < length; i++) {
    if(path[i] == '/') {
      if (lastWasSlash == TRUE) {
        return FALSE;
      }

      lastWasSlash = TRUE;
    } else {
      lastWasSlash = FALSE;
    }
  }

  return TRUE;
}

int isFileName(char* filename) {
  if(strlen(filename) == 0) {
    return -1;
  }

  char* temp = malloc(strlen(filename));
  strcpy(temp, filename);

  char * pch = strtok(temp, ".");

  if(strcmp(pch, filename)) { //nenhum ponto foi encontrado
    return FALSE; // é um nome de diretório
  }

  return TRUE; // is a directory
}

int parsePath(char* path, char ** elements) {
  if(validPath(path) == FALSE) {
    return FALSE;
  }

  char opPath[MAX_FILE_NAME_SIZE];
  strcpy(opPath, path);

  char * pch = strtok(opPath, "/");

  int i = 0;
  elements[i++] = "/";

  while(pch != NULL) {
    /*
      Alocação direta no array, e não elemento por elemento.

    if(elementsSize < ((i + 1) * MAX_FILE_NAME_SIZE)) {
      printf("extendendo tamanho do array\n");
      int newArraySize = (i * MAX_FILE_NAME_SIZE) * growthFactor;

      char ** temp_Array = realloc(elements, newArraySize);
      if(temp_Array) {
        printf("Novo tamanho de array realocado.\n");
        elements = temp_Array;
        elementsSize = newArraySize;
      } else {
        printf("Erro alocando novo array de paths. You should have done error handling.\n");
      }
    } */

    elements[i] = (char*) malloc((strlen(pch) + 1) * sizeof(char));
    strcpy(elements[i++], pch);

    pch = strtok(NULL, "/");
  }

  return i;
}
