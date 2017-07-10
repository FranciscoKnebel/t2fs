/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Testes dos métodos de API: write

  Desenvolvido por Francisco Knebel
*/

#include <time.h>

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

int write(int handle, int size, char* buffer) {
  if(handle < 0) {
    printf("%d: Erro abrindo arquivo.\n", handle);
    return -1;
  }

  int check = write2(handle, buffer, size);
  if(check < 0) {
    printf("%d: Erro escrevendo arquivo.\n", check);
    return -1;
  }

  return check;
}

char *randstring(int length) {
    char *string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
    size_t stringLen = 26*2+10+7;
    char *randomString;

    randomString = malloc(sizeof(char) * (length +1));

    if (!randomString) {
        return (char*)0;
    }

    unsigned int key = 0;
    int n;
    for (n = 0;n < length;n++) {
        key = rand() % stringLen;
        randomString[n] = string[key];
    }

    randomString[length] = '\0';

    return randomString;
}

void readAndPrint(int handle, char* path, int size) {
  int offset = 0, check;
  char *newBuffer = malloc(sizeof(char) * size);
  strcpy(newBuffer, "");
  seek2(handle, 0);
  do {
    check = read(handle, 3, &newBuffer[offset]);
    if(check > 0) {
      offset += check;
    }
  } while(check != 0);

  newBuffer[offset] = '\0';
  printf("\nImprimindo arquivo '%s', de '%d' bytes:\n", path, offset);
  printf("%s", newBuffer);
}

void test_openFileAndWrite(char* path) {
  int handle = open2(path);
  seek2(handle, -1); // Ponteiro para o final do arquivo

  write(handle, 10, randstring(10));
  write(handle, 10, randstring(10));
  write(handle, 10, randstring(10));

  readAndPrint(handle, path, 30);

  close2(handle);
}

void test_writeRoot(char* path, int size, char* buffer) {
  int check;

  int handle = open2(path);
  seek2(handle, -1);

  check = write(handle, strlen(buffer), buffer);

  char newBuffer[size];
  seek2(handle, 0);
  check = read(handle, size, newBuffer);
  if(check > 0) {
    printf("\nImprimindo arquivo '%s', de '%d' bytes:\n", path, check);
    puts(newBuffer);
  }

  close2(handle);
}

void test_writeBlock(char* path) {
  int handle = open2(path);

  int i = 0, loops = 16, bytesPerWrite = 64;
  int size = loops * bytesPerWrite;
  for (i = 0; i < loops; i++) {
    write(handle, bytesPerWrite, randstring(bytesPerWrite));
  }

  readAndPrint(handle, path, size);

  printf("\n\n");
  BLOCK_T block;
  block.at = malloc(sizeof(char) * constants.BLOCK_SIZE);

  readBlock(2052, &block);
  printBlock(block.at);

  close2(handle);
}

void test_write2() {
  /* WRITE FILE */
  printf("\n--- WRITE2 ---\n");
  char buffer[10];
  strcpy(buffer, "0123456789");

  //test_writeRoot("/file1", 100, buffer);

  //test_openFileAndWrite("/file2");

  test_writeBlock("/file2");

  printf("\n--- ENCERRANDO WRITE2 ---\n");
  return;
}

int main() {
  char identify[IDENTIFY_SIZE];

  identify2(identify, IDENTIFY_SIZE);
  printf("%s\n\n", identify);
  srand(time(NULL));
  initConfig();

  test_write2();

  return 0;
}
