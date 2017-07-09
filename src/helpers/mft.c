/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Módulo desenvolvido por Douglas Lázaro
*/

#include "libs.h"


void initMFT() {
  SECTOR_T reg;
  int i, j;

  for (i = 0; i < REGISTER_REGULAR; i++) { //primeiros resgistros reservados
    config.indexMFT[i] = MFT_BM_OCUPADO;
  }

  for (i = REGISTER_REGULAR, j = REGISTER_REGULAR*2; j < constants.MAX_SECTORS_REGISTER; i++, j += 2) {
    int sector = constants.MFT_SECTOR + j;

    if(readSector(sector, &reg) != TRUE) {
      return;
    }

    struct t2fs_4tupla tuplaInicial = parseRegister_tupla(reg.at, 0);

    if(tuplaInicial.atributeType == REGISTER_FREE) {
      config.indexMFT[i] = MFT_BM_LIVRE; //livre
    } else {
      config.indexMFT[i] = MFT_BM_OCUPADO; //ocupado
    }
  }
}

void printMFT(int begin, int end){
  int i;
  for (i = begin; i < end; ++i)
  {
    if (config.indexMFT[i] == MFT_BM_LIVRE)
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
      return -1;
}

int searchMFT(int allocated){
  int i;
  for (i = REGISTER_REGULAR; i < constants.MAX_REGISTERS; ++i)
  {
    if (config.indexMFT[i] == allocated)
      return i;
  }
  return -1;
}
