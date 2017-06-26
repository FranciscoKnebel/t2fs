#ifndef __mft__
#define	__mft__

  void initMFT();

  void printMFT(int begin, int end);

  int getMFT(int registerIndex);

  int setMFT(int registerIndex, int allocated);

  int searchMFT(int allocated);

#endif
