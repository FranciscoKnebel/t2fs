#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "apidisk.h"
#include "bitmap2.h"
#include "t2fs.h"

int identify2 (char *name, int size) {
  if (size == 0) {
    name[0] = '\0';

    return 0;
  }

  char info[] = "\n Douglas Lazaro S P Silva \t- 207305 \n Francisco Paiva Knebel \t- 243688";
  int length = strlen(info);

  if(size < length) {
    memcpy(name, info, size);
    name[size] = '\0';
  } else {
    memcpy(name, info, length);
    name[length] = '\0';
  }

  return 0;
};

FILE2 create2 (char *filename) {
  return -1;
};

int delete2 (char *filename) {
  return -1;
};

FILE2 open2 (char *filename) {
  return -1;
};

int close2 (FILE2 handle) {
  return -1;
};

int read2 (FILE2 handle, char *buffer, int size) {
  return -1;
};

int write2 (FILE2 handle, char *buffer, int size) {
  return -1;
};

int truncate2 (FILE2 handle) {
  return -1;
};

int seek2 (FILE2 handle, DWORD offset) {
  return -1;
};

int mkdir2 (char *pathname) {
  return -1;
};

int rmdir2 (char *pathname) {
  return -1;
};

DIR2 opendir2 (char *pathname) {
  return -1;
};

int readdir2 (DIR2 handle, DIRENT2 *dentry) {
  return -1;
};

int closedir2 (DIR2 handle) {
  return -1;
};
