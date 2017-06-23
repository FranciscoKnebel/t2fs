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

  for(i = 0; i < SECTOR_PER_BLOCK; i++) {
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
