/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Testes dos métodos de src/disk.c

  Desenvolvido por Douglas Lázaro
*/

#include "libs.h"

void test_getBitmap2(int bloco, int flag) {
  printf("-- GET BITMAP --\n");
  printf("--Bloco %d --\n", bloco);
  if(getBitmap2(bloco) == flag){
    printf("--Bloco %d --\n", bloco);
    if(flag == 1)
      printf("--Corretamente Alocado --\n");
    else
      printf("--Corretamente Livre --\n");
  }
  else{
    printf("--Bloco %d --\n", bloco);
    if(flag == 1)
      printf("--Está Alocado e não Livre--\n");
    else
      printf("--Está Livre e não Alocado --\n");
  }
}

void test_setBitmap2(int bloco, int flag) {
  printf("-- SET BITMAP --\n");
  if (setBitmap2(bloco, flag) == 0)
  {
    printf("--Bloco %d SETADO --\n", bloco);
  }
  else
    printf("\t -- Erro ao SETAR Bloco %d -- \n", bloco);
}

void test_searchBitmap2(int flag) {
  printf("-- SEARCH BITMAP --\n");
  printf("--Procurando Bloco ");
  if(flag == 1)
      printf(" Alocado --\n");
    else
      printf(" Livre --\n");

  int bloco = searchBitmap2(flag); 
  if(bloco <= 0)
    printf("ERRO :: Não Encontrado --\n");
  else{
    printf("--Bloco %d --\n", bloco);
  }

}


void test_Bitmap(int bloco, int flag) {
  test_searchBitmap2(flag);
  test_setBitmap2(bloco, flag);
  test_getBitmap2(bloco, flag);
}

int main(int argc, char const *argv[]) {
  initConfig();

  /* TEST BITMAP */
  int bloco = searchBitmap2(0);
  test_Bitmap(bloco, 0);

  return 0;
}
