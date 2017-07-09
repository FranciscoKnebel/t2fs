/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Testes dos métodos de API: read

  Desenvolvido por Francisco Knebel
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libs.h"

#define IDENTIFY_SIZE 80

int read(int handle, int size, char* buffer) {
  if(handle < 0) {
    printf("%d: Erro abrindo arquivo.\n", handle);
    return -1;
  }

  int check = read2(handle, buffer, size);
  if(check < 0) {
    printf("%d: Erro lendo arquivo.\n", check);
    return -1;
  }

  return check;
}

void test_readRoot(char* path, int size) {
  int check;
  char* buffer = malloc(sizeof(char) * size);

  int handle = open2(path);
  check = read(handle, size, buffer);
  close2(handle);

  printf("\nImprimindo arquivo '%s', de '%d' bytes:\n", path, check);
  printf("%s", buffer);
}

void test_currentPointer(char* path, int bytesPerRead) {
  int amountOfBytesRead;
  int offset = 0;
  char* buffer = malloc(sizeof(char) * bytesPerRead);
  char* fileContent = malloc(sizeof(char) * bytesPerRead * 100);

  int handle = open2(path);
  do {
    amountOfBytesRead = read(handle, bytesPerRead, buffer);

    if(amountOfBytesRead > 0) {
      memcpy(&fileContent[offset], buffer, amountOfBytesRead);
    } else {
      printf("\nArquivo lido até EOF.\n");
    }
    offset += amountOfBytesRead;
  } while(amountOfBytesRead != 0);
  close2(handle);

  printf("\nImprimindo arquivo '%s', de '%d' bytes:\n", path, offset);
  puts(fileContent);
}

void test_read2() {
  /* READ FILE */
  printf("\n--- READ2 ---\n");

  //test_readRoot("/file1", 30);
  //test_readRoot("/file2", 30);

  test_currentPointer("/file1", 10);

  printf("\n--- ENCERRANDO READ2 ---\n");
  return;
}

int main() {
  char identify[IDENTIFY_SIZE];

  identify2(identify, IDENTIFY_SIZE);
  printf("%s\n\n", identify);

  initConfig();

  test_read2();

  return 0;
}
