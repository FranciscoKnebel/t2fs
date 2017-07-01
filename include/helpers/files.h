#ifndef __helperfiles__
#define	__helperfiles__

  int validPath(char* path);

  int isFileName(char* path);

  int getFileDirectory(char* path, char* directory);

  int parsePath(char* path, char ** elements);

  int lookup(char* pathname, struct t2fs_record * fileRecord);

#endif
