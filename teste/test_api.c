/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "t2fs.h"
#include "constants.h"

#define IDENTIFY_SIZE 80

FILE2 test_executeCreate2(char * filename) {
  FILE2 handle;

  handle = create2(filename);

  if(handle < 0) {
    printf("Erro no create2: ");

    switch (handle) {
      case FOUND_FILE_ERROR:
        printf("Arquivo '%s' já existe.\n", filename);
        break;
      case DIRECTORY_NOT_FOUND:
        printf("Um dos diretórios da path '%s' não existe.\n", filename);
        break;
      default:
        printf("Erro %d\n", handle);
    }

  } else {
    printf("Arquivo '%s' criado com handle '%d'.\n", filename, handle);
  }

  return handle;
}

void test_create2() {
  /* CREATE FILE */
  printf("--- CREATE2 ---\n");

  char path1[50] = "batata";
  char path2[50] = "/batata";
  char path3[50] = "/file1";
  char path4[50] = "/file/FILE2";

  test_executeCreate2(path1);
  test_executeCreate2(path2);
  test_executeCreate2(path3);
  test_executeCreate2(path4);

  printf("--- ENCERRANDO CREATE2 ---\n");
  return;
}

int main(int argc, char const *argv[]) {
  char identify[IDENTIFY_SIZE];

  identify2(identify, IDENTIFY_SIZE);
  printf("%s\n\n", identify);

  test_create2();

  return 0;
}
