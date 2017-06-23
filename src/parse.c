/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Módulo desenvolvido por Francisco Knebel
  Funções:
    parseBootBlock, parseVersion, parseBlockSize, parseMFTBlocksSize, parseDiskSectorSize
*/

#include "libs.h"

DWORD parseDiskSectorSize(unsigned char* DiskSectorSize) {
  /* 00 80 00 00 => 0x00008000 => 32768 */
  char temp[8] = "";
  int num = 0;

  sprintf(temp, "%02X%02X%02X%02X", DiskSectorSize[2], DiskSectorSize[3], DiskSectorSize[1], DiskSectorSize[0]);
  sscanf(temp, "%x", &num);

  return num;
}

WORD parseMFTBlocksSize(unsigned char* MFTBlocksSize) {
  /* 00 08 => 0x0800 => 2048 */
  char temp[2] = "";
  int num = 0;

  sprintf(temp, "%02X%02X", MFTBlocksSize[1], MFTBlocksSize[0]);
  sscanf(temp, "%x", &num);

  return num;
}

WORD parseBlockSize(unsigned char* blockSize) {
  /* 04 00 => 0x0004 => 4 */
  char temp[2] = "";
  int num = 0;

  sprintf(temp, "%02X%02X", blockSize[1], blockSize[0]);
  sscanf(temp, "%x", &num);

  return num;
}

void parseVersion(char* version, char* str) {
  unsigned char lowNibble = version[0] & 0x0f;
  unsigned char highNibble = (version[0] >> 4) & 0x0f;

  char temp[7] = "";
  sprintf(temp, "%d-%d", (version[1] << 4) | highNibble, lowNibble);
  temp[6] = '\0';

  strcpy(str, temp);
  return;
}

struct bootBlock parseBootBlock(unsigned char* bootBlock) {
  struct bootBlock config;

  char version[2] = "";
  unsigned char blockSize[2] = "";
  unsigned char MFTBlocksSize[2] = "";
  unsigned char diskSectorSize[4] = "";

  // ID
  memcpy(config.id, bootBlock, 4 * sizeof(BYTE));
  config.id[4] = '\0';

  // VERSION
  memcpy(version, bootBlock + 4, 2 * sizeof(BYTE));
  parseVersion(version, config.version);

  // BLOCK SIZE
  memcpy(blockSize, bootBlock + 6, 2 * sizeof(BYTE));
  config.blockSize = parseBlockSize(blockSize);

  // MFT BLOCK SIZE
  memcpy(MFTBlocksSize, bootBlock + 8, 2 * sizeof(BYTE));
  config.MFTBlocksSize = parseMFTBlocksSize(MFTBlocksSize);

  // DISK SECTOR SIZE
  memcpy(diskSectorSize, bootBlock + 10, 4 * sizeof(BYTE));
  config.diskSectorSize = parseDiskSectorSize(diskSectorSize);

  return config;
}

void parseRegister() {
  return;
}

void parseDirectory() {
  parseRegister();
  return;
}

void parseFile() {
  parseRegister();
  return;
};
