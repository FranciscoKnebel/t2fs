/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Douglas Lazaro
  Francisco Knebel
*/

#include "libs.h"

int closeFile(int handle, BYTE TypeVal) {
  int return_value = -1;
  struct descritor result;

  if(searchLDAA(handle, TypeVal, &result) == TRUE) {
    if (removeLDAA(handle) == TRUE) {
      return_value = 0;
    } else {
      return_value = ERRO_REMOVE_LDAA;
    }
  } else {
    return_value = NOT_FOUND_LDAA;
  }

  return return_value;
}
