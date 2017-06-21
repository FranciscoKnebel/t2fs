/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Casos de teste desenvolvidos por Francisco Knebel  
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "apidisk.h"
#include "bitmap2.h"
#include "t2fs.h"

#include "aux.h"
#include "constants.h"
#include "disk.h"

#define IDENTIFY_SIZE 80

int main(int argc, char const *argv[]) {
  char identify[IDENTIFY_SIZE];

  identify2(identify, IDENTIFY_SIZE);
  printf("%s\n", identify);

  return 0;
}
