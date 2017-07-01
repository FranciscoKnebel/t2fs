#ifndef __files2__
#define	__files2__
  #include "declarations.h"

  struct t2fs_4tupla initTupla(DWORD atributeType, DWORD VBN, DWORD LBN, DWORD numberOfContiguosBlocks);

  struct t2fs_record createRecord(char* pathname);

#endif
