#ifndef __disk__
#define	__disk__

  typedef struct {
    unsigned char at[SECTOR_SIZE];
  } SECTOR_T;

  typedef struct {
    unsigned char at[BLOCK_SIZE];
  } BLOCK_T;

  typedef struct {
    unsigned char at[SECTOR_SIZE * 2];
  } REGISTER_T;

  typedef BLOCK_T RECORD_T;

  int readSector(int sector, SECTOR_T* buffer);

  int readBlock(int block, BLOCK_T* buffer);

  int readRegister(int registerIndex, REGISTER_T* reg);

  int readRecord(int block, int index, struct t2fs_record * record);

  int writeSector(int sector, SECTOR_T* buffer);

  int writeBlock(int block, BLOCK_T* buffer);

  int writeRegister(int registerIndex, REGISTER_T* reg);

#endif
