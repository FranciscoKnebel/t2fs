#ifndef __files__
#define	__files__

  int validPath(char* path);

  int isFileName(char* path);

  int parsePath(char* path, char ** elements);

  int lookup(char* pathname, struct t2fs_record * fileRecord);

#endif
