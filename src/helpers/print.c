/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Módulo desenvolvido por Francisco Knebel
  Funções:
    printSector, printSector2, printBlock, showBlock, printBootBlock, printRegister, printTuplaBinary, printTupla, printRecord

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

void printRegister(unsigned char* buffer, int sector) {
  int i;

  for (i = 0; i < 2; i++) {
    printSector2(&buffer[i * SECTOR_SIZE], sector + i);
  }
}

void printTuplaBinary(unsigned char* buffer, int tupla) {
  char temp[8] = "";
  int j, num = 0;

  for (j = 0; j < constants.TUPLA_SIZE; j += 4) {
    printf("%02X ", buffer[tupla + j     ]&0xFF);
    printf("%02X ", buffer[tupla + j + 1 ]&0xFF);
    printf("%02X ", buffer[tupla + j + 2 ]&0xFF);
    printf("%02X ", buffer[tupla + j + 3 ]&0xFF);
    printf("---");

    sprintf(temp, "%02X%02X%02X%02X", buffer[tupla + j + 3], buffer[tupla + j + 2], buffer[tupla + j + 1], buffer[tupla + j]);
    sscanf(temp, "%x", &num);

    printf(" %s ", temp);

    printf("--- %d\n", num);
  }
}

void printTupla(struct t2fs_4tupla registro) {
  printf("Attribute Type: %d\n", registro.atributeType);
  printf("Virtual Block Number: %d\n", registro.virtualBlockNumber);
  printf("Logical Block Number: %d\n", registro.logicalBlockNumber);
  printf("Number of Contiguous Blocks: %d\n", registro.numberOfContiguosBlocks);
}

void printRecord(struct t2fs_record record) {
  printf("TypeVal: %d\n", record.TypeVal);
  printf("Name: %s\n", record.name);
  printf("Blocks: %d\n", record.blocksFileSize);
  printf("Bytes: %d\n", record.bytesFileSize);
  printf("MFT Number: %d\n", record.MFTNumber);
}
