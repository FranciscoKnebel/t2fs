#ifndef __files2__
#define	__files2__
  #include "declarations.h"

  struct t2fs_record createRecord(char* pathname);

  int addRecordToDirectory(struct t2fs_record record, char * filename);

  int openFile(char* filename);

  int closeFile(int handle, BYTE TypeVal);

#endif
