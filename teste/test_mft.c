/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Testes dos métodos do módulo MFT

  Desenvolvido por Douglas Lázaro
*/

#include "libs.h"

int main(int argc, char const *argv[]) {
  initConfig();

  // int indexMFT[constants.MAX_REGISTERS];

  initMFT();
  setMFT(15, 1);
  printMFT(10, 20);


  return 0;
}
