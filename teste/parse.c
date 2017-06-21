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
#include "types.h"
#include "constants.h"
#include "disk.h"
#include "parse.h"

void test_parseBootBlock() {
  BLOCK_T bootBlock;
  struct bootBlock config;

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
