#include "libs.h"

int initConfig() {
  BLOCK_T bootBlock;

  if(readBlock(0, &bootBlock) == FALSE) {
    return FALSE;
  };

  config.bootBlock = parseBootBlock(bootBlock.at);

  return TRUE;
}
