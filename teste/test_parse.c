/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Testes desenvolvidos por Francisco Knebel
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libs.h"

void test_parseBootBlock() {
  BLOCK_T bootBlock;
  struct BootBlock config;

  readBlock(0, &bootBlock);

  config = parseBootBlock(bootBlock.at);

  printf("ID: %s\n", config.id);
  printf("Version: %s\n", config.version);
  printf("Block Size: %d\n", config.blockSize);
  printf("MFT Blocks Size: %d\n", config.MFTBlocksSize);
  printf("Disk Sector Size: %d\n", config.diskSectorSize);
}

void test_parseRegister() {
  int registerIndex = 0;
  REGISTER_T reg;

  if(readRegister(registerIndex, &reg) != TRUE) {
    return;
  }

  struct t2fs_4tupla *tuplas = malloc(sizeof(struct t2fs_4tupla));
  parseRegister(reg.at, tuplas);

  int i;
  for (i = 0; i < constants.MAX_TUPLAS_REGISTER; i++) {
    printf("\n");
    printTupla(tuplas[i]);
  }
}

int main(int argc, char const *argv[]) {
  initConfig();

  // PARSE BOOT BLOCK
  test_parseBootBlock();

  /* Parse Register */
  test_parseRegister();

  return 0;
}
