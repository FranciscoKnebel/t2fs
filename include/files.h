/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Douglas Lazaro
  Francisco Knebel
*/

#ifndef __files2__
#define	__files2__
  #include "declarations.h"

  /* Create */
  struct t2fs_record createRecord(char* pathname, BYTE typeVal);

  int createNewFile(char * filename, BYTE typeVal);

  int addRecordToDirectory(struct t2fs_record record, char * filename);

  /* Delete */
  int deleteFileFromDisk(struct t2fs_record file, char* filename);

  /* Open */
  int openFile(char* filename);

  /* Close */
  int closeFile(int handle, BYTE TypeVal);

  /* Read */
  int readFile(int handle, struct descritor descritor, char * buffer, int size);

#endif
