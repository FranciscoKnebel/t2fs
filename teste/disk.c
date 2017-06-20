#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aux.h"
#include "constants.h"
#include "disk.h"
#include "apidisk.h"

int main(int argc, char const *argv[]) {
  /* READ SECTOR */
  printf("-- READ SECTOR --\n");
  SECTOR_T sectorBuffer;
  readSector(0, &sectorBuffer);
  printSector(sectorBuffer.at);

  /* READ BLOCK */
  printf("-- READ BLOCK --\n");
  BLOCK_T blockBuffer;
  readBlock(1, &blockBuffer);
  printBlock(blockBuffer.at);

  /* Mostrar blocos de importância */
  printf("-- BOOT BLOCK --\n");
  showBlock(BOOT_BLOCK);
  printf("-- MFT BLOCK --\n");
  showBlock(MFT_BLOCK);
  printf("-- DATA BLOCK --\n");
  showBlock(DATA_BLOCK);

  return 0;
}
