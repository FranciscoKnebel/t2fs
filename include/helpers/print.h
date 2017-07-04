#ifndef __helperaux__
#define	__helperaux__

  int printSector(unsigned char* buffer);

  int printSector2(unsigned char* buffer, int currentSector);

  int printBlock(unsigned char* buffer);

  void showBlock(int block);

  void printBootBlock();

  void printConstants();

  void printRegister(unsigned char* buffer, int sector);

  void printTuplaBinary(unsigned char* buffer, int tupla);

  void printTupla(struct t2fs_4tupla registro);

  void printRecord(struct t2fs_record record);

  void printDescritor(struct descritor file);

#endif
