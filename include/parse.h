#ifndef __parse__
#define	__parse__

  struct BootBlock parseBootBlock(unsigned char* bootBlock);

  int parseRegister(unsigned char* buffer, struct t2fs_4tupla * tuplas);

#endif
