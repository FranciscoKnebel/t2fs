/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Testes dos métodos de API: open e close

  Desenvolvido por Francisco Knebel
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libs.h"

#define IDENTIFY_SIZE 80

void test_create_open2_close2(BYTE TypeVal, char * path) {
  struct descritor file;

  if(TypeVal == TYPEVAL_REGULAR) {
    printf("--- OPEN2 - CLOSE2 ---\n");

    int handle = create2(path);
    if(handle >= 0) {
      printf("Novo arquivo '%s' criado com handle '%d'.\n", path, handle);

      printf("Fechando arquivo criado.\n");
      if(close2(handle) == 0) {
        printf("\tArquivo fechado corretamente.\n");

        printf("Abrindo arquivo novamente.\n");
        handle = open2(path);
        if(handle >= 0) {
          printf("\tArquivo aberto normalmente.\n");
          if(searchLDAA(handle, TypeVal, &file) == TRUE) {
            printDescritor(file);
          }
        }
      }
    }
  } else if(TypeVal == TYPEVAL_DIRETORIO) {
    printf("--- OPENDIR2 - CLOSEDIR2 ---\n");

    int handle = mkdir2(path);
    if(handle >= 0) {
      printf("Novo diretório '%s' criado com handle '%d'.\n", path, handle);

      printf("Fechando diretório criado.\n");
      if(closedir2(handle) == 0) {
        printf("\tDiretório fechado corretamente.\n");

        printf("Abrindo diretório novamente.\n");
        handle = opendir2(path);
        if(handle >= 0) {
          printf("\tDiretório aberto normalmente.\n");
          if(searchLDAA(handle, TypeVal, &file) == TRUE) {
            printDescritor(file);
          }
        } else {
          printf("Falhou ao abrir diretório.\n");
        }
      } else {
        printf("Falhou ao fechar diretório.\n");
      }
    }
  }

  printf("--- ENCERRANDO OPEN2 e CLOSE2 ---\n");
}

void test_open_close_root() {
  printf("--- OPENDIR2 -ROOT- CLOSEDIR2 ---\n");

  char path[50] = "/";
  struct descritor file;

  int handle = opendir2(path);
  printf("Abrindo diretório root.\n");
  if(handle >= 0) {
    printf("Diretório aberto normalmente.\n");
    if(searchLDAA(handle, TYPEVAL_DIRETORIO, &file) == TRUE) {
      printDescritor(file);
    }
  } else {
    printf("Falhou ao abrir diretório.\n");
  }

  printf("Fechando diretório de root.\n");
  if(closedir2(handle) == 0) {
    printf("Diretório fechado corretamente.\n");
  } else {
    printf("Falhou ao fechar diretório.\n");
  }

  printf("--- OPENDIR2 -ROOT- CLOSEDIR2 ---\n");
}

int main(int argc, char const *argv[]) {
  char identify[IDENTIFY_SIZE];

  identify2(identify, IDENTIFY_SIZE);
  printf("%s\n\n", identify);


  //test_create_open2_close2(TYPEVAL_REGULAR, "/test3");

  //test_create_open2_close2(TYPEVAL_DIRETORIO, "/");

  test_open_close_root();

  return 0;
}
