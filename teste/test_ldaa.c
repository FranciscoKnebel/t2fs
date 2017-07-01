/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Testes dos métodos do módulo LDAA

  Desenvolvido por Douglas Lázaro
*/

#include "libs.h"

int main(int argc, char const *argv[]) {
  initConfig();

  int i = getFreeLDAA(config.LDAA);

  printf("Primeiro LDAA livre :: %d \n", i);

  printf("\t Inserindo no LDAA \n");

  struct t2fs_record record = {1, "Teste", 1, 1, 10 };

  if(insertLDAA(config.LDAA, record) == TRUE)
	printf("Inserido com sucesso \n");
  else
  	printf("Erro ao inserir \n");

  printf("\t Procurando no LDAA \n");

  if(searchLDAA(config.LDAA, i, 1) == TRUE){
  	printf("Encontrado Handle %d \n", i);
	
	printf("\t Printando: \n");

	printf("typeVal = %d \n name = %s \n MFT = %d \n", config.LDAA[i].record.TypeVal, config.LDAA[i].record.name, config.LDAA[i].record.MFTNumber);
  }
  else
	printf("Erro ao encontrar \n");  	
    
  return 0;
}
