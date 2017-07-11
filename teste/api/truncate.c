/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Testes dos métodos de API: truncate

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

int writeContiguousBlocks(char* path) {
  int handle = open2(path);

  int i = 0, loops = 30, bytesPerWrite = 64;
  for (i = 0; i < loops; i++) {
    write(handle, bytesPerWrite, randstring(bytesPerWrite));
  }

  close2(handle);

  return loops * bytesPerWrite;
}

void test_truncate_contiguous(char* path) {
  int size = writeContiguousBlocks(path);

  int handle = open2(path);

  // le o arquivo e imprime
  readAndPrint(handle, path, size);

  // seta currentPointer, para truncar, e reseta valor do buffer.
  seek2(handle, 128);
  truncate2(handle);

  // seta currentPointer para o inicio.
  seek2(handle, 0);
  readAndPrint(handle, path, 256);
}

int main() {
  char identify[IDENTIFY_SIZE];

  identify2(identify, IDENTIFY_SIZE);
  printf("%s\n\n", identify);

  initConfig();

  printf("--- TRUNCATE2 ---\n");

  test_truncate_contiguous("/file2");

  printf("--- END TRUNCATE2 ---\n");

  return 0;
}
