/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Testes desenvolvidos por Francisco Knebel
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libs.h"

#define IDENTIFY_SIZE 80

int main(int argc, char const *argv[]) {
  initConfig();

  char identify[IDENTIFY_SIZE];

  identify2(identify, IDENTIFY_SIZE);
  printf("%s\n\n", identify);

  printBootBlock();

  return 0;
}
