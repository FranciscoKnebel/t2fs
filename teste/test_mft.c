/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Testes dos métodos do módulo MFT

  Desenvolvido por Douglas Lázaro
*/

#include "libs.h"

int main(int argc, char const *argv[]) {
  initConfig();

  setMFT(15, 1);
  printMFT(0, 20);

  return 0;
}
