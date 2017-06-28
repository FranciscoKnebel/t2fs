#ifndef __disk__
#define	__disk__
  #include "declarations.h"

  int readSector(int sector, SECTOR_T* buffer);

  int readBlock(int block, BLOCK_T* buffer);

  int readBootBlock(SECTOR_T* buffer);

  int readRegister(int registerIndex, REGISTER_T* reg);

  int readRecord(int block, int index, struct t2fs_record * record);

  int writeSector(int sector, SECTOR_T* buffer);

  int writeBlock(int block, BLOCK_T* buffer);

  int writeRegister(int registerIndex, REGISTER_T* reg);

#endif
