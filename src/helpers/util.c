/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Módulo desenvolvido por Francisco Knebel
  Funções:
    initConfig, convertTwoBytes, convertFourBytes
*/

#include "libs.h"

int initConfig() {
  BLOCK_T bootBlock;

  if(readBlock(REGISTER_BOOT_BLOCK, &bootBlock) == FALSE) {
    return FALSE;
  };

  config.bootBlock = parseBootBlock(bootBlock.at);

  // config.indexMFT[MAX_REGISTERS];

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
