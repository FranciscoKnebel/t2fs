/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Testes dos métodos de API: delete

  Desenvolvido por Francisco Knebel
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libs.h"

#define IDENTIFY_SIZE 80

FILE2 test_executeCreate2(char * filename) {
  FILE2 handle;

  handle = create2(filename);

  if(handle < 0) {
    printf("Erro no create2: ");

    switch (handle) {
      case PARSED_PATH_ERROR:
        printf("Path '%s' inválida.\n", filename);
        break;
      case FOUND_FILE_ERROR:
        printf("Arquivo '%s' já existe.\n", filename);
        break;
      case DIRECTORY_NOT_FOUND:
        printf("Um dos diretórios da path '%s' não existe.\n", filename);
        break;
      case FILE_LIMIT_REACHED:
        printf("Máximo de arquivos abertos alcançado.\n");
        break;
      default:
        printf("Erro %d\n", handle);
    }

  } else {
    printf("Arquivo '%s' criado com handle '%d'.\n", filename, handle);
    close2(handle);
  }

  return handle;
}

void test_create2_showfiles_parseandprint(int block) {
  BLOCK_T blockBuffer;
  blockBuffer.at = malloc(sizeof(unsigned char) * constants.BLOCK_SIZE);
  struct t2fs_record records[constants.RECORD_PER_BLOCK];
  unsigned int i;

  if(readBlock(block, &blockBuffer) == FALSE) {
    return;
  };
  parseDirectory(blockBuffer, records);

  for (i = 0; i < constants.RECORD_PER_BLOCK; i++) {
    printRecord(records[i]);
    printf("\n");
  }
}

void executeDelete(char * path) {
  int check = delete2(path);

  if(check == 0) {
    printf("Removeu arquivo '%s' corretamente.\n", path);
  } else {
    printf("%d: Erro ao remover arquivo '%s'.\n", check, path);
  }
}

void test_knownfile() {
  char path[50] = "/file2";

  // Remove arquivo "/file2"
  executeDelete(path);
  // test_create2_showfiles_parseandprint(2050);

  // Criar outro arquivo, que deve ser escrito no mesmo local onde estava "/file2"
  test_executeCreate2("/file3");

  // test_create2_showfiles_parseandprint(2050);
}

void test_delete2() {
  /* DELETE FILE */
  printf("--- DELETE2 ---\n");

  test_knownfile();

  printf("--- ENCERRANDO DELETE2 ---\n");
  return;
}

int main() {
  char identify[IDENTIFY_SIZE];

  identify2(identify, IDENTIFY_SIZE);
  printf("%s\n\n", identify);

  initConfig();

  test_delete2();

  return 0;
}
