#ifndef __parse__
#define	__parse__
  #include "declarations.h"

  struct BootBlock parseBootBlock(unsigned char* bootBlock);

  int parseRegister(unsigned char* buffer, struct t2fs_4tupla * tuplas);

  int parseRecord(BLOCK_T blockBuffer, struct t2fs_record * record, int offset);

  int parseDirectory(BLOCK_T block, struct t2fs_record* records);
  
#endif
