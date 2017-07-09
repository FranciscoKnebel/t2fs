/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Douglas Lazaro
  Francisco Knebel
*/

#include "libs.h"

/*-----------------------------------------------------------------------------
Função: parseDiskSectorSize, parseMFTBlocksSize, parseBlockSize, parseVersion
  Parsing de buffer do bloco de boot para encontrar valores para estrutura

Entrada:
  Buffer com valor respectivo lido do bloco de boot.

Saida:
  Retorna valor numeral convertido do elemento.

Responsável: Francisco Knebel
-----------------------------------------------------------------------------*/
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

  return convertTwoBytes(MFTBlocksSize, 0, temp);
}

WORD parseBlockSize(unsigned char* blockSize) {
  /* 04 00 => 0x0004 => 4 */
  char temp[2] = "";

  return convertTwoBytes(blockSize, 0, temp);
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

struct BootBlock parseBootBlock(unsigned char* bootBlock) {
  struct BootBlock config;

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

struct t2fs_4tupla parseRegister_tupla(unsigned char* buffer, int tuplaIndex) {
  struct t2fs_4tupla tupla;
  char temp[8];

  tupla.atributeType = convertFourBytes(buffer, tuplaIndex, temp);
  tupla.virtualBlockNumber = convertFourBytes(buffer, tuplaIndex + 4, temp);
  tupla.logicalBlockNumber = convertFourBytes(buffer, tuplaIndex + 8, temp);
  tupla.numberOfContiguosBlocks = convertFourBytes(buffer, tuplaIndex + 12, temp);

  return tupla;
}

int parseRegister(unsigned char* buffer, struct t2fs_4tupla * tuplas) {
  unsigned int i, offset;

  for (i = 0; i < constants.MAX_TUPLAS_REGISTER; ++i) {
    offset = constants.TUPLA_SIZE * i;

    tuplas[i] = parseRegister_tupla(&buffer[offset], 0);
  }

  return TRUE;
}

int parseRecord(BLOCK_T blockBuffer, struct t2fs_record * record, int offset) {
  char str[2];

  record->TypeVal = blockBuffer.at[RECORD_TYPE + offset];
  memcpy(record->name, &blockBuffer.at[RECORD_NAME + offset], MAX_FILE_NAME_SIZE * sizeof(char));
  record->blocksFileSize = convertFourBytes(blockBuffer.at, RECORD_BLOCK_FILE_SIZE + offset, str);
  record->bytesFileSize = convertFourBytes(blockBuffer.at, RECORD_BYTES_FILE_SIZE + offset, str);
  record->MFTNumber = convertFourBytes(blockBuffer.at, RECORD_MFT_NUMBER + offset, str);

  return TRUE;
}

int parseDirectory(BLOCK_T block, struct t2fs_record* records) {
  unsigned int i;
  unsigned int offset;

  for (i = 0; i < constants.RECORD_PER_BLOCK; i++) {
    offset = (i * RECORD_SIZE);

    parseRecord(block, &records[i], offset);
  }

  return TRUE;
}
