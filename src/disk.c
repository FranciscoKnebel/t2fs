/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Douglas Lazaro
  Francisco Knebel
*/

#include "libs.h"

int readSector(int sector, SECTOR_T* buffer) {
  if(sector < 0 || sector > constants.DISK_SECTORS) {
    return FALSE;
  }

  if (read_sector((unsigned int) sector, (unsigned char*) buffer) == 0) {
    return TRUE;
  }

  return FALSE;
}

int readBlock(int block, BLOCK_T* buffer) {
  int i, sector;

  if (block < 0 || block > constants.DISK_BLOCKS) {
    return FALSE;
  }

  sector = block * constants.SECTOR_PER_BLOCK;

  for(i = 0; i < constants.SECTOR_PER_BLOCK; i++) {
    if (readSector(sector, (SECTOR_T*) &buffer->at[i * SECTOR_SIZE]) == FALSE) {
      return FALSE;
    }

    sector++;
  }

  return TRUE;
}

int readBootBlock(SECTOR_T* buffer) {
  if (read_sector(REGISTER_BOOT_BLOCK, (unsigned char*) buffer) == 0) {
    return TRUE;
  }

  return FALSE;
}

int readRegister(int registerIndex, REGISTER_T* reg) {
  if(registerIndex < 0 || registerIndex > constants.MAX_REGISTERS) {
    return -1;
  }

  int sector = constants.MFT_SECTOR + registerIndex * constants.REGISTER_PER_BLOCK;
  /* Leitura dos dois setores do Registro */
  /* Primeiro setor */
  if (readSector(sector, (SECTOR_T*) reg) == FALSE) {
    return FALSE;
  }

  /* Segundo setor */
  if (readSector(sector + 1, (SECTOR_T*) reg + 1) == FALSE) {
    return FALSE;
  }

  return TRUE;
}

int readRecord(int block, int index, struct t2fs_record * record) {
  if(block < constants.DATA_BLOCK || block >= constants.DISK_BLOCKS || index < 0 || index >= constants.RECORD_PER_BLOCK) {
    return FALSE;
  }

  int offset = (index * RECORD_SIZE);
  BLOCK_T blockBuffer;
  blockBuffer.at = malloc(sizeof(unsigned char) * constants.BLOCK_SIZE);

  if (readBlock(block, &blockBuffer) == FALSE) {
    return FALSE;
  }

  parseRecord(blockBuffer, record, offset);

  return TRUE;
}

int writeSector(int sector, SECTOR_T* buffer) {
  if (sector < 0 || sector > constants.DISK_SECTORS) {
    return FALSE;
  }

  if (write_sector((unsigned int) sector, (unsigned char*) buffer) == 0) {
    return TRUE;
  }

  return FALSE;
}

int writeBlock(int block, BLOCK_T* buffer) {
  int i, sector;

  if (block < 0 || block > constants.DISK_BLOCKS) {
    return FALSE;
  }

  sector = block * constants.SECTOR_PER_BLOCK;

  for(i = 0; i < constants.SECTOR_PER_BLOCK; i++){
    if (writeSector(sector++, (SECTOR_T*) &buffer->at[i*SECTOR_SIZE]) == FALSE) {
      return FALSE;
    }
  }

  return TRUE;
}

int writeRegister(int registerIndex, REGISTER_T* reg) {
  if(registerIndex < 0 || registerIndex > constants.MAX_REGISTERS) {
    return -1;
  }

  int sector = constants.MFT_SECTOR + registerIndex * constants.REGISTER_PER_BLOCK;

  /* Escrita dos dois setores do Registro */
  /* Primeiro setor */
  if (writeSector(sector, (SECTOR_T*) reg) == FALSE) {
    return FALSE;
  }

  /* Segundo setor */
  if (writeSector(sector + 1, (SECTOR_T*) reg + 1) == FALSE) {
    return FALSE;
  }

  return TRUE;
}

int writeRecord(int block, int index, struct t2fs_record record) {
  if(block < constants.DATA_BLOCK || block >= constants.DISK_BLOCKS || index < 0 || index >= constants.RECORD_PER_BLOCK) {
    return FALSE;
  }

  int sector_offset = index / constants.SECTOR_PER_BLOCK;
  int record_offset = index % constants.SECTOR_PER_BLOCK;
  int sector = block * constants.SECTOR_PER_BLOCK + sector_offset;
  SECTOR_T sectorBuffer;

  if(readSector(sector, &sectorBuffer) == FALSE) {
    return FALSE;
  }

  memcpy(&sectorBuffer.at[RECORD_SIZE * record_offset], &record, RECORD_SIZE);

  writeSector(sector, &sectorBuffer);

  return TRUE;
}

void* writeTupla(unsigned char* buffer, struct t2fs_4tupla* tupla, int index) {
  return memcpy(&buffer[constants.TUPLA_SIZE * index], tupla, sizeof(struct t2fs_4tupla));
}

int resetBlock(int block) {
  BLOCK_T tempBlock;

  tempBlock.at = malloc(sizeof(unsigned char) * constants.BLOCK_SIZE);
  memset(tempBlock.at, 0, sizeof(unsigned char) * constants.BLOCK_SIZE);

  return writeBlock(block, &tempBlock);
}

int freeRegister(int registerIndex) {
  REGISTER_T reg;
  memset(reg.at, -1, sizeof(reg.at));

  return writeRegister(registerIndex, &reg);
}
