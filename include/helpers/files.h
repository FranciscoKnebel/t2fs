#ifndef __helperfiles__
#define	__helperfiles__
  #include "declarations.h"

  int validPath(char* path);

  int isFileName(char* path);

  int getFileDirectory(char* path, char* directory);

  int parsePath(char* path, char ** elements);

  int lookup(char* pathname, struct t2fs_record * fileRecord);


  struct t2fs_record initRecord(BYTE typeVal, char* name, DWORD blocksFileSize, DWORD bytesFileSize, DWORD MFTNumber);

  struct t2fs_4tupla initTupla(DWORD atributeType, DWORD VBN, DWORD LBN, DWORD numberOfContiguosBlocks);

  int initFileRegister(int registerIndex, int LBN);

  DIRENT2 initDentry(struct t2fs_record record);

#endif
