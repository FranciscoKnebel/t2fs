/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Módulo desenvolvido por Francisco Knebel
  Funções:
    readSector, readBlock, writeSector, writeBlock
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "disk.h"

#include "t2fs.h"
#include "apidisk.h"

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
