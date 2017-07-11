/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Douglas Lazaro
  Francisco Knebel
*/

#ifndef __helperaux__
#define	__helperaux__

  /*-----------------------------------------------------------------------------
  Função: printX
    Múltiplas funções para imprimir estruturas do sistema, para debuggação.
    Cada função tem o seu comportamento definido independentemente, e deverá ser
    visto cada implementação individualmente.

  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  int printSector(unsigned char* buffer);

  int printSector2(unsigned char* buffer, int currentSector);

  int printBlock(unsigned char* buffer);

  void showBlock(int block);

  void printBootBlock();

  void printConstants();

  void printRegister(unsigned char* buffer, int sector);

  void printTuplaBinary(unsigned char* buffer, int tupla);

  void printTupla(struct t2fs_4tupla registro);

  void printRecord(struct t2fs_record record);

  void printDescritor(struct descritor file);

#endif
