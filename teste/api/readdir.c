/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Testes dos métodos de API: readdir

  Desenvolvido por Francisco Knebel
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libs.h"

#define IDENTIFY_SIZE 80

int main() {
  char identify[IDENTIFY_SIZE];

  identify2(identify, IDENTIFY_SIZE);
  printf("%s\n\n", identify);

  initConfig();

  printf("--- READDIR2 ---\n");

  int handle = open2("/");
  DIRENT2 dentry;

  int check = 0;
  while(check != -END_OF_DIR) {
    check = readdir2(handle, &dentry);

    if(check != -END_OF_DIR) {
      printf("%s: %d, %d\n", dentry.name, dentry.fileType, dentry.fileSize);
    } else {
      printf("END OF DIR REACHED!\n");
    }
  }

  printf("--- END READDIR2 ---\n");

  return 0;
}
