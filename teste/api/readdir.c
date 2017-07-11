/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Testes dos métodos de API: ReadDir

  Desenvolvido por Douglas Lázaro
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libs.h"

#define IDENTIFY_SIZE 80


void test_readdir() {
  
  struct t2fs_record record = {1, "Teste", 1, 896, 10 };
  int handle = opendir2 ("/");
  DIRENT2 dentry;

  printf("Abrindo diretório root... \n");
  if(handle != -1)
    printf("\t Aberto com sucesso \n");
  else
    printf("\t Erro ao abrir \n");

  printf("Descritor no LDAA \n");
  printf("\t currentPointer = %d \n\t name = %s \n\t MFT = %d \n", config.LDAA[handle].currentPointer, config.LDAA[handle].record.name, config.LDAA[handle].record.MFTNumber);

  printf("READDIR2() \n");
  while(readdir2(handle, &dentry) != -1)
  {
    printf("Arquivo: %s \t\t Tipo: %d \t\t Tamanho: %d \n", dentry.name, dentry.fileType, dentry.fileSize);
    
  }
  
  printf("Descritor no LDAA \n");
  printf("\t currentPointer = %d \n\t name = %s \n\t MFT = %d \n", config.LDAA[handle].currentPointer, config.LDAA[handle].record.name, config.LDAA[handle].record.MFTNumber);

}



int main(int argc, char const *argv[]) {
  char identify[IDENTIFY_SIZE];

  identify2(identify, IDENTIFY_SIZE);
  printf("%s\n\n", identify);

  initConfig();

  printf("\n ------ \n");
  test_readdir();

  printf("\n ------ \n");


  return 0;
}
