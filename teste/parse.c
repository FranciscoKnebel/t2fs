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

int main(int argc, char const *argv[]) {
  // PARSE BOOT BLOCK
  test_parseBootBlock();

  return 0;
}
