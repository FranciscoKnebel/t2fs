/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Módulo desenvolvido por Francisco Knebel
  Funções:
    initConfig, initConstants, convertTwoBytes, convertFourBytes
*/

#include "libs.h"

struct Constants initConstants(struct BootBlock bootBlock) {
  struct Constants k;

  /* Disco */
  k.SECTOR_PER_BLOCK = bootBlock.blockSize;
  k.BLOCK_SIZE = k.SECTOR_PER_BLOCK * SECTOR_SIZE; // SECTOR_SIZE IS A DEFINED CONSTANT

  k.DISK_SECTORS = bootBlock.diskSectorSize;
  k.DISK_BLOCKS = k.DISK_SECTORS / k.SECTOR_PER_BLOCK;

  k.BOOT_BLOCK_SIZE = 1;
  k.BOOT_SECTOR_SIZE = k.BOOT_BLOCK_SIZE * k.SECTOR_PER_BLOCK;

  k.MFT_BLOCK_SIZE = bootBlock.MFTBlocksSize;
  k.MFT_SECTOR_SIZE = k.MFT_BLOCK_SIZE * k.SECTOR_PER_BLOCK;

  k.DATA_BLOCK_SIZE = k.DISK_BLOCKS - (k.BOOT_BLOCK_SIZE + k.MFT_BLOCK_SIZE);
  k.DATA_SECTOR_SIZE = k.DISK_SECTORS - (k.BOOT_SECTOR_SIZE + k.MFT_SECTOR_SIZE);

  k.BOOT_SECTOR = 0;
  k.MFT_SECTOR = k.BOOT_SECTOR + k.SECTOR_PER_BLOCK;
  k.DATA_SECTOR = k.MFT_SECTOR_SIZE + k.SECTOR_PER_BLOCK;

  k.BOOT_BLOCK = 0;
  k.MFT_BLOCK = k.BOOT_BLOCK + 1;
  k.DATA_BLOCK = k.MFT_BLOCK_SIZE + 1;

  /* Tuplas */
  k.REGISTER_SIZE = 512;
  k.REGISTER_PER_BLOCK = k.BLOCK_SIZE / k.REGISTER_SIZE;
  k.MAX_REGISTERS = (k.MFT_SECTOR_SIZE / k.REGISTER_PER_BLOCK);

  /* Registro */
  k.MAX_TUPLAS_REGISTER = 32;
  k.TUPLA_SIZE = k.REGISTER_SIZE / k.MAX_TUPLAS_REGISTER;

  /* Records */
  k.RECORD_PER_BLOCK = k.BLOCK_SIZE / RECORD_SIZE;

  return k;
}


int initConfig() {
  SECTOR_T bootBlock;

  if(readBootBlock(&bootBlock) == FALSE) {
    return FALSE;
  };

  struct BootBlock boot = parseBootBlock(bootBlock.at);
  constants = initConstants(boot);

  int * MFT = malloc(sizeof(int) * constants.MAX_REGISTERS);
  config.indexMFT = MFT;
  initMFT();

  initLDAA(config.LDAA);

  return TRUE;
}

int convertTwoBytes(unsigned char* buffer, int index, char* str) {
  int num;

  sprintf(str, "%02X%02X", buffer[index + 1], buffer[index]);
  sscanf(str, "%x", &num);

  return num;
}

int convertFourBytes(unsigned char* buffer, int index, char* str) {
  int num;

  sprintf(str, "%02X%02X%02X%02X", buffer[index + 3], buffer[index + 2], buffer[index + 1], buffer[index]);
  sscanf(str, "%x", &num);

  return num;
}
