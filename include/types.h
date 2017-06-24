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
    unsigned short SECTORSIZE;
    unsigned short SECTOR_PER_BLOCK;
    unsigned short BLOCKSIZE;

    unsigned short DISK_SECTORS;
    unsigned short DISK_BLOCKS;

    /* Partições do disco */
    unsigned short BOOT_BLOCK_SIZE;
    unsigned short BOOT_SECTOR_SIZE;

    unsigned short MFT_BLOCK_SIZE;
    unsigned short MFT_SECTOR_SIZE;

    unsigned short DATA_BLOCK_SIZE;
    unsigned short DATA_SECTOR_SIZE;

    /* Blocos do disco */
    unsigned short BOOT_BLOCK;
    unsigned short MFT_BLOCK;
    unsigned short DATA_BLOCK;

    /* Blocos do disco */
    unsigned short BOOT_SECTOR;
    unsigned short MFT_SECTOR;
    unsigned short DATA_SECTOR;
  };

  struct Configuration {
    int initiated;

    struct BootBlock bootBlock;

    struct Constants constants;
  };
#endif
