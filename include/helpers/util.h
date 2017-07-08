/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Douglas Lazaro
  Francisco Knebel
*/

#ifndef __helperutil__
#define	__helperutil__

  int initConfig();

  int convertTwoBytes(unsigned char* buffer, int index, char* str);

  int convertFourBytes(unsigned char* buffer, int index, char* str);

#endif
