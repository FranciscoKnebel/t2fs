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

int findRecord(struct t2fs_4tupla tupla, char* name, struct t2fs_record * record) {
  BLOCK_T blockBuffer;
  struct t2fs_record records[constants.RECORD_PER_BLOCK];

  switch (tupla.atributeType) {
    case REGISTER_MAP:
      if(readBlock(tupla.logicalBlockNumber, &blockBuffer) == FALSE) {
        return FALSE;
      };

      parseDirectory(blockBuffer, records);

      int i, foundFile = FALSE;
      for (i = 0; i < constants.RECORD_PER_BLOCK && foundFile != TRUE; i++) {
        if(strcmp(records[i].name, name) == 0) { // FILE NAME FOUND
          memcpy((void*) record, (void*) &records[i], RECORD_SIZE);

          foundFile = TRUE;
        }
      }

      return foundFile == TRUE ? i : FALSE;

      break;
    case REGISTER_FIM:
      return tupla.atributeType;
      break;
    case REGISTER_ADITIONAL:
      // find new tupla
      break;
    case REGISTER_FREE:
    default:
      break;
  }

  return TRUE;
}

int lookup(char* pathname, struct t2fs_record * fileRecord) {
  char ** parsedPath = malloc(sizeof(char) * MAX_FILE_NAME_SIZE);
  int parseCount = parsePath(pathname, parsedPath);

  if(parseCount == FALSE) {
    return FALSE;
  }

  REGISTER_T root;
  struct t2fs_4tupla *tuplas = malloc(sizeof(struct t2fs_4tupla));

  if(readRegister(REGISTER_ROOT, &root) != TRUE) {
    return FALSE;
  }
  parseRegister(root.at, tuplas);

  /* ITERAR NA ÁRVORE ATÉ ACHAR FOLHA */
  struct t2fs_record record;

  int i = 0;
  int j = 1;
  // olhar todas tuplas para ver se encontra
  if(findRecord(tuplas[i], parsedPath[j], &record) == FALSE) {
    // return FALSE; // FILE NOT FOUND
  }
  // se não achar em nenhuma tupla, retorna FALSE
  // se achar, possuí record do arquivo com nome parsedPath[j]

  // É o último elemento de parsedPath? Se sim e encontrou, retorna TRUE. fileRecord é o record procurado.
  // Se sim e não encontrou, retorna FALSE.
  // Se não, fetch do registro do arquivo record, e itera novamente.

  memcpy(fileRecord, &record, RECORD_SIZE);
  free(tuplas);
  i++;

  return TRUE;
}
