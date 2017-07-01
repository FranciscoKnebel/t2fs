#ifndef __types__
#define	__types__
  #include "t2fs.h"

  struct BootBlock {
    char id[5];
    char version[7];
    WORD blockSize;
    WORD MFTBlocksSize;
    DWORD diskSectorSize;
  };

  struct Constants {
    int SECTOR_PER_BLOCK;
    int BLOCK_SIZE;

    int DISK_SECTORS;
    int DISK_BLOCKS;

    /* Partições do disco */
    int BOOT_BLOCK_SIZE;
    int BOOT_SECTOR_SIZE;

    int MFT_BLOCK_SIZE;
    int MFT_SECTOR_SIZE;

    int DATA_BLOCK_SIZE;
    int DATA_SECTOR_SIZE;

    /* Blocos do disco */
    int BOOT_BLOCK;
    int MFT_BLOCK;
    int DATA_BLOCK;

    /* Blocos do disco */
    int BOOT_SECTOR;
    int MFT_SECTOR;
    int DATA_SECTOR;

    /* Registro */
    int REGISTER_SIZE;
    int REGISTER_PER_BLOCK;
    int MAX_REGISTERS;

    /* Tupla */
    int MAX_TUPLAS_REGISTER;
    int TUPLA_SIZE;

    /* Records */
    int RECORD_PER_BLOCK;
  };

  struct Configuration {
    int initiated;

    struct BootBlock bootBlock;

    int* indexMFT;

    struct descritor* LDAA;
  };

  typedef struct {
    unsigned char at[SECTOR_SIZE];
  } SECTOR_T;

  typedef struct {
    unsigned char * at;
  } BLOCK_T;

  typedef struct {
    unsigned char at[SECTOR_SIZE * 2];
  } REGISTER_T;

#endif
