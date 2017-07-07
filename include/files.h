#ifndef __files2__
#define	__files2__
  #include "declarations.h"

  struct t2fs_record createRecord(char* pathname, BYTE typeVal);

  int createNewFile(char * filename, BYTE typeVal);

  int addRecordToDirectory(struct t2fs_record record, char * filename);

  int openFile(char* filename);

  int closeFile(int handle, BYTE TypeVal);

  int deleteFileFromDisk(struct t2fs_record file, char* filename);

#endif
