#ifndef __ldaa__
#define	__ldaa__
  #include "declarations.h"

  struct descritor {
  	int flag;
  	int currentPointer;
  	struct t2fs_record record;
  };

  void initLDAA();

  int getFreeLDAA();

  int insertLDAA(struct t2fs_record records);

  int removeLDAA(int handle);

  int searchLDAA(int handle, int type);

#endif
