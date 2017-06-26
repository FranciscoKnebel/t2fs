/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Módulo desenvolvido por Douglas Lázaro
  Funções:
    initMFT, printMFT, getMFT, setMFT, searchMFT

*/

#include "libs.h"

void initMFT() {
  REGISTER_T reg;
  int i;

  for (i = 0; i < REGISTER_REGULAR; i++) { //primeiros resgistros reservados
    config.indexMFT[i] = 1;
  }

  for (i = REGISTER_REGULAR; i < MAX_REGISTERS; i++) {
    if(readRegister(i, &reg) != TRUE) {
      return;
    }

    struct t2fs_4tupla *tuplas = malloc(sizeof(struct t2fs_4tupla));
    parseRegister(reg.at, tuplas);

    if(tuplas[0].atributeType == -1){
      config.indexMFT[i] = 0; //livre
    } else {
      config.indexMFT[i] = 1; //ocupado
    }
  }
}

void printMFT(int begin, int end){
  int i;
  for (i = begin; i < end; ++i)
  {
    if (config.indexMFT[i] == 0)
      printf("Index MFT [%d] =  LIVRE \n",i);
    else
      printf("Index MFT [%d] =  OCUPADO \n",i);
  }

}

int getMFT(int registerIndex){
  return config.indexMFT[registerIndex];
}

int setMFT(int registerIndex, int allocated){

    config.indexMFT[registerIndex] = allocated;

    if (config.indexMFT[registerIndex] == allocated)
      return TRUE;
    else
      return FALSE;
}

int searchMFT(int allocated){
  int i;
  for (i = REGISTER_REGULAR; i < MAX_REGISTERS; ++i)
  {
    if (config.indexMFT[i] == allocated)
      return i;
  }
  return FALSE;
}
