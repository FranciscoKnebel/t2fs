/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Testes dos métodos de API: Seek

  Desenvolvido por Douglas Lázaro
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libs.h"

#define IDENTIFY_SIZE 80


void test_seek(int offset) {
  
  struct t2fs_record record = {1, "Teste", 1, 896, 10 };
  int handle = insertLDAA(record, "Teste");

  printf("Inserindo no LDAA \n");
  if(handle != -1)
    printf("\t Inserido com sucesso \n");
  else
    printf("\t Erro ao inserir \n");

  printf("Descritor no LDAA \n");
  printf("\t currentPointer = %d \n\t name = %s \n\t MFT = %d \n", config.LDAA[handle].currentPointer, config.LDAA[handle].record.name, config.LDAA[handle].record.MFTNumber);

  printf("SEEK2() \n");
  if (seek2(handle, offset) == 0)
    printf("\t currentPointer atualizado com sucesso \n");
  else
    printf("\t Erro ao atualizar \n");

  printf("Descritor no LDAA \n");
  printf("\t currentPointer = %d \n\t name = %s \n\t MFT = %d \n", config.LDAA[handle].currentPointer, config.LDAA[handle].record.name, config.LDAA[handle].record.MFTNumber);

}



int main(int argc, char const *argv[]) {
  char identify[IDENTIFY_SIZE];

  identify2(identify, IDENTIFY_SIZE);
  printf("%s\n\n", identify);

  initConfig();

  printf("\n ---Seek Default--- \n");
  test_seek(768);

  printf("\n ---Seek EOF--- \n");
  test_seek(-1);


  return 0;
}
