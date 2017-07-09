/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Douglas Lazaro
  Francisco Knebel
*/

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

  int initNewRegister(int registerIndex, int VBN, int LBN);

  int initFileRegister(int registerIndex, int LBN);

  DIRENT2 initDentry(struct t2fs_record record);


  int findOffsetTupla(struct t2fs_4tupla * tuplas, unsigned int initialBlock, REGISTER_T* reg);

#endif
