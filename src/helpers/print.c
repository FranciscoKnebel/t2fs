/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Módulo desenvolvido por Francisco Knebel
  Funções:
    printSector, printSector2, printBlock, showBlock, printBootBlock
*/

#include "libs.h"

int printSector(unsigned char* buffer) {
  return printSector2(buffer, 0);
}

int printSector2(unsigned char* buffer, int currentSector) {
  int i, j;
  char str[20];

  for (i = 0; i < 16; ++i) {
    printf("%04X  ", (SECTOR_SIZE * currentSector) + 16 * i);
    for (j = 0; j < 16; ++j) {
      int index = 16*i+j;
      char c = buffer[index];

      if (c>=' ' && c<='z') {
        str[j] = c;
      } else {
        str[j] = ' ';
      }

      printf ("%02X ", c&0xFF);
    }

    str[16]='\0';
    printf (" *%s*\n", str);
  }

  return 0;
}

int printBlock(unsigned char* buffer) {
  int i;

  for(i = 0; i < constants.SECTOR_PER_BLOCK; i++) {
    printSector2(&buffer[i * SECTOR_SIZE], i);
    printf("\n");
  }

  return 0;
}

void showBlock(int block) {
  BLOCK_T buffer;

  readBlock(block, &buffer);

  printBlock(buffer.at);

  return;
}

void printBootBlock() {
  printf("ID: %s\n", config.bootBlock.id);
  printf("Version: %s\n", config.bootBlock.version);
  printf("Block Size: %d\n", config.bootBlock.blockSize);
  printf("MFT Blocks Size: %d\n", config.bootBlock.MFTBlocksSize);
  printf("Disk Sector Size: %d\n", config.bootBlock.diskSectorSize);
}

void printConstants() {
  printf("\n");
  printf("CONSTANTS:\n");
  printf("Sector Size: %u\n", constants.SECTORSIZE);
  printf("Sector per Block: %u\n", constants.SECTOR_PER_BLOCK);
  printf("Block Size: %u\n", constants.BLOCKSIZE);

  printf("\n");
  printf("Disk Sectors: %u\n", constants.DISK_SECTORS);
  printf("Disk Blocks: %u\n", constants.DISK_BLOCKS);

  printf("\n");
  printf("Boot Block Size: %u\n", constants.BOOT_BLOCK_SIZE);
  printf("Boot Sector Size: %u\n", constants.BOOT_SECTOR_SIZE);

  printf("MFT Block Size: %u\n", constants.MFT_BLOCK_SIZE);
  printf("MFT Sector Size: %u\n", constants.MFT_SECTOR_SIZE);

  printf("Data Block Size: %u\n", constants.DATA_BLOCK_SIZE);
  printf("Data Sector Size: %u\n", constants.DATA_SECTOR_SIZE);

  printf("\n");
  printf("BOOT first sector: %u\n", constants.BOOT_SECTOR);
  printf("MFT first sector : %u\n", constants.MFT_SECTOR);
  printf("Data first sector: %u\n", constants.DATA_SECTOR);

  printf("\n");
  printf("BOOT first block: %u\n", constants.BOOT_BLOCK);
  printf("MFT first block : %u\n", constants.MFT_BLOCK);
  printf("Data first block: %u\n", constants.DATA_BLOCK);
}
