#ifndef __types__
#define	__types__

  struct BootBlock {
    char id[5];
    char version[7];
    WORD blockSize;
    WORD MFTBlocksSize;
    DWORD diskSectorSize;
  };

  struct Configuration {
    int initiated;

    struct BootBlock bootBlock;

    int indexMFT[MAX_REGISTERS];
  };
#endif
