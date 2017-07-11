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
    char *string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    size_t stringLen = 26*2+10;
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
    check = read(handle, 32, &newBuffer[offset]);
    if(check > 0) {
      offset += check;
    }
  } while(check != 0);

  newBuffer[offset] = '\0';
  printf("\nArquivo '%s' lido, com tamanho de '%d' bytes:\n", path, offset);
  printf("Pressione ENTER para imprimir o arquivo."); getchar();
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

void test_contiguousBlocks(char* path) {
  int handle = open2(path);

  int i = 0, loops = 40, bytesPerWrite = 64;
  int size = loops * bytesPerWrite;
  for (i = 0; i < loops; i++) {
    write(handle, bytesPerWrite, randstring(bytesPerWrite));
  }

  readAndPrint(handle, path, size);

  printf("\n\n");
  BLOCK_T block1, block2, block3;
  block1.at = malloc(sizeof(unsigned char) * constants.BLOCK_SIZE);
  block2.at = malloc(sizeof(unsigned char) * constants.BLOCK_SIZE);
  block3.at = malloc(sizeof(unsigned char) * constants.BLOCK_SIZE);

  /*
  readBlock(2052, &block1);
  readBlock(2053, &block2);
  readBlock(2054, &block3);
  printBlock(block1.at);
  printBlock(block2.at);
  printBlock(block3.at);
  */

  free(block1.at);
  free(block2.at);
  free(block3.at);

  close2(handle);
}

void test_contiguousBlocksThenMap(char* path, char* newFileName) {
  int handle = open2(path);

  int i = 0, loops = 40, bytesPerWrite = 64;
  int size = loops * bytesPerWrite;

  char *string = malloc(sizeof(char) * bytesPerWrite);
  sprintf(string, "$$%s##", randstring(bytesPerWrite - 4)); // String aleatório, com elementos indicando inicio e fim, para testes.

  for (i = 0; i < loops; i++) {
    write(handle, bytesPerWrite, string);
  }

  int handle2 = create2(newFileName); // Para ocupar um bloco e forçar map

  // Escreve novamente, e deverá indicar que não é possível alocar contiguo, e criará outra tupla
  for (i = 0; i < loops; i++) {
    write(handle, bytesPerWrite, string);
  }

  readAndPrint(handle, path, size*2);

  printf("\n\n");
  BLOCK_T block1, block2, block3, block4, block5, block6;
  block1.at = malloc(sizeof(unsigned char) * constants.BLOCK_SIZE);
  block2.at = malloc(sizeof(unsigned char) * constants.BLOCK_SIZE);
  block3.at = malloc(sizeof(unsigned char) * constants.BLOCK_SIZE);
  block4.at = malloc(sizeof(unsigned char) * constants.BLOCK_SIZE);
  block5.at = malloc(sizeof(unsigned char) * constants.BLOCK_SIZE);
  block6.at = malloc(sizeof(unsigned char) * constants.BLOCK_SIZE);

  /*
  readBlock(2052, &block1);
  readBlock(2053, &block2);
  readBlock(2054, &block3);

  readBlock(2058, &block1);
  readBlock(2059, &block2);
  readBlock(2060, &block3);

  printBlock(block1.at);
  printBlock(block2.at);
  printBlock(block3.at);
  printBlock(block4.at);
  printBlock(block5.at);
  printBlock(block6.at);
  */

  free(block1.at);
  free(block2.at);
  free(block3.at);
  free(block4.at);
  free(block5.at);
  free(block6.at);

  close2(handle);
  close2(handle2);
}

void test_contiguousBlocksBlocksThenMapThenAditional(char* path) {
  int handle = open2(path);

  int bytesPerWrite = 1024, amountOfIterations = 40;
  int size = bytesPerWrite;

  char *string = malloc(sizeof(char) * bytesPerWrite);
  sprintf(string, "$$%s##", randstring(bytesPerWrite - 4)); // String aleatório, com elementos indicando inicio e fim, para testes.

  char *tempFileName = malloc(sizeof(char) * 20);

  int j = 0;
  for (j = 0; j < amountOfIterations; j++) {
    write(handle, bytesPerWrite, string);

    free(tempFileName);
    tempFileName = malloc(sizeof(char) * 20);
    sprintf(tempFileName, "/file_%s", randstring(5));
    create2(tempFileName); // Para ocupar um bloco e forçar map
  }

  readAndPrint(handle, path, size*amountOfIterations);
}

void test_write2() {
  /* WRITE FILE */
  printf("\n--- WRITE2 ---\n");
  char buffer[10];
  strcpy(buffer, "0123456789");

  //test_writeRoot("/file1", 100, buffer);

  //test_openFileAndWrite("/file2");

  //test_writeBlock("/file2");

  //test_contiguousBlocks("/file2");

  //test_contiguousBlocksThenMap("/file2", "/file3");

  test_contiguousBlocksBlocksThenMapThenAditional("/file2");

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
