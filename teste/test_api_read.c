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

void test_readRoot() {
  int handle;

  handle = open2("/file1");
  if(handle < 0) {
    printf("%d: Erro abrindo arquivo.\n", handle);
    return;
  }

  char buffer[50];
  int check;

  check = read2(handle, buffer, 20);
  if(check < 0) {
    printf("%d: Erro lendo arquivo.\n", check);
    return;
  }

  printf("Imprimindo arquivo lido:\n");
  printf("%s", buffer);
  printf("\n -- Print encerrado -- \n");
}

void test_read2() {
  /* READ FILE */
  printf("--- READ2 ---\n");

  test_readRoot();

  printf("--- ENCERRANDO READ2 ---\n");
  return;
}

int main(int argc, char const *argv[]) {
  char identify[IDENTIFY_SIZE];

  identify2(identify, IDENTIFY_SIZE);
  printf("%s\n\n", identify);

  initConfig();

  test_read2();

  return 0;
}
