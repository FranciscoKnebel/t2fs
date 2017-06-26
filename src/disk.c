/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Módulo desenvolvido por Francisco Knebel
  Funções:
    readSector, readBlock, writeSector, writeBlock, readRegister, writeRegister
*/

#include "libs.h"

int readSector(int sector, SECTOR_T* buffer) {
  if(sector < 0) {
    return FALSE;
  }

  if (read_sector((unsigned int) sector, (unsigned char*) buffer) == 0) {
    return TRUE;
  }

  return FALSE;
}

int readBlock(int block, BLOCK_T* buffer) {
  int i, sector;

  if (block < 0) {
    return FALSE;
  }

  sector = block * SECTOR_PER_BLOCK;

  for(i = 0; i < SECTOR_PER_BLOCK; i++) {
    if (readSector(sector, (SECTOR_T*) &buffer->at[i * SECTOR_SIZE]) == FALSE) {
      return FALSE;
    }

    sector++;
  }

  return TRUE;
}

int readRegister(int registerIndex, REGISTER_T* reg) {
  if(registerIndex > MAX_REGISTERS) {
    return -1;
  }

  int sector = MFT_SECTOR + registerIndex * REGISTER_PER_BLOCK;
  /* Leitura dos dois setores do Registro */
  /* Primeiro setor */
  if (readSector(sector, (SECTOR_T*) reg) == FALSE) {
    return FALSE;
  }

  /* Segundo setor */
  if(readSector(sector + 1, (SECTOR_T*) reg + 1) == FALSE) {
    return FALSE;
  }

  return TRUE;
}

int readRecord(int block, int index, struct t2fs_record * record) {
  if(block < DATA_BLOCK || block > DISK_BLOCKS) {
    return FALSE;
  }

  int offset = (index * RECORD_SIZE);
  BLOCK_T blockBuffer;
  char str[2];

  readBlock(block, &blockBuffer);

  (* record).TypeVal = blockBuffer.at[RECORD_TYPE + offset];
  memcpy((* record).name, &blockBuffer.at[RECORD_NAME + offset], MAX_FILE_NAME_SIZE * sizeof(char));
  (* record).blocksFileSize = convertFourBytes(blockBuffer.at, RECORD_BLOCK_FILE_SIZE + offset, str);
  (* record).bytesFileSize = convertFourBytes(blockBuffer.at, RECORD_BYTES_FILE_SIZE + offset, str);
  (* record).MFTNumber = convertFourBytes(blockBuffer.at, RECORD_MFT_NUMBER + offset, str);

  return TRUE;
}

int writeSector(int sector, SECTOR_T* buffer) {
  if (sector < 0) {
    return FALSE;
  }

  if (write_sector((unsigned int) sector, (unsigned char*) buffer) == 0) {
    return TRUE;
  }

  return FALSE;
}

int writeBlock(int block, BLOCK_T* buffer) {
  int i, sector;

  if (block < 0) {
    return FALSE;
  }

  sector = block * SECTOR_PER_BLOCK;

  for(i = 0; i < SECTOR_PER_BLOCK; i++){
    if (writeSector(sector++, (SECTOR_T*) &buffer->at[i*SECTOR_SIZE]) == FALSE) {
      return FALSE;
    }
  }

  return TRUE;
}

int writeRegister(int registerIndex, REGISTER_T* reg) {
  if(registerIndex > MAX_REGISTERS) {
    return -1;
  }

  int sector = MFT_SECTOR + registerIndex * REGISTER_PER_BLOCK;

  /* Escrita dos dois setores do Registro */
  /* Primeiro setor */
  if (writeSector(sector, (SECTOR_T*) reg) == FALSE) {
    return FALSE;
  }

  /* Segundo setor */
  if(writeSector(sector + 1, (SECTOR_T*) reg + 1) == FALSE) {
    return FALSE;
  }

  return TRUE;
}
