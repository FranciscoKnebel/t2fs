#include "libs.h"

void test_initConfig() {
  initConfig();

  printBootBlock();

  printConstants();
}

int main(int argc, char const *argv[]) {
  /* INIT configuration structure */
  test_initConfig();


  return 0;
}
