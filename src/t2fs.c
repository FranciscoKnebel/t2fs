/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

*/

#include "libs.h"

struct Configuration config = { .initiated = FALSE };
struct Constants constants;

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
  if (!config.initiated) {
    initConfig();
  }

  return -1;
};

int delete2 (char *filename) {
  if (!config.initiated) {
    initConfig();
  }

  return -1;
};

FILE2 open2 (char *filename) {
  if (!config.initiated) {
    initConfig();
  }

  return -1;
};

int close2 (FILE2 handle) {
  if (!config.initiated) {
    initConfig();
  }

  return -1;
};

int read2 (FILE2 handle, char *buffer, int size) {
  if (!config.initiated) {
    initConfig();
  }

  return -1;
};

int write2 (FILE2 handle, char *buffer, int size) {
  if (!config.initiated) {
    initConfig();
  }

  return -1;
};

int truncate2 (FILE2 handle) {
  if (!config.initiated) {
    initConfig();
  }

  return -1;
};

int seek2 (FILE2 handle, DWORD offset) {
  if (!config.initiated) {
    initConfig();
  }

  return -1;
};

int mkdir2 (char *pathname) {
  if (!config.initiated) {
    initConfig();
  }

  return -1;
};

int rmdir2 (char *pathname) {
  if (!config.initiated) {
    initConfig();
  }

  return -1;
};

DIR2 opendir2 (char *pathname) {
  if (!config.initiated) {
    initConfig();
  }

  return -1;
};

int readdir2 (DIR2 handle, DIRENT2 *dentry) {
  if (!config.initiated) {
    initConfig();
  }

  return -1;
};

int closedir2 (DIR2 handle) {
  if (!config.initiated) {
    initConfig();
  }

  return -1;
};
