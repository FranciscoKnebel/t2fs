#ifndef __types__
#define	__types__

  struct BootBlock {
    char id[5];
    char version[7];
    WORD blockSize;
    WORD MFTBlocksSize;
    DWORD diskSectorSize;
  };

  struct Constants {
    int SECTORSIZE;
    int SECTOR_PER_BLOCK;
    int BLOCKSIZE;

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
  };

  struct Configuration {
    int initiated;

    struct BootBlock bootBlock;

    struct Constants constants;
  };
#endif
