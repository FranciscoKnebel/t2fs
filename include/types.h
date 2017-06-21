#ifndef __types__
#define	__types__

  struct bootBlock {
    char id[5];
    char version[7];
    WORD blockSize;
    WORD MFTBlocksSize;
    DWORD diskSectorSize;
  };

#endif
