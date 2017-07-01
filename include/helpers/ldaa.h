#ifndef __ldaa__
#define	__ldaa__
  #include "declarations.h"

  struct descritor {
  	int flag;
  	int currentPointer;
  	struct t2fs_record record;
  };

  void initLDAA(struct descritor * lista);

  int getFreeLDAA(struct descritor * lista);

  int insertLDAA(struct descritor * lista, struct t2fs_record records);

  int removeLDAA(struct descritor * lista, int handle);

  int searchLDAA(struct descritor * lista, int handle, int type);

#endif
