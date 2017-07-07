/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Testes dos métodos do módulo LDAA

  Desenvolvido por Douglas Lázaro
*/

#include "libs.h"

void test_findByNameValidate(char* name) {
  int handle = findByNameLDAA(name);

  if(handle < 0) {
    printf("Arquivo '%s' não encontrado na LDAA.\n", name);
  } else {
    printf("Arquivo '%s' encontrado, com handle '%d'.\n", name, handle);

    printf("\tRemovendo '%s' e efetuando busca novamente.\n", name);
    removeLDAA(handle);

    handle = findByNameLDAA(name);
    if(handle < 0) {
      printf("\tArquivo '%s' não encontrado, como esperado.\n", name);
    } else {
      printf("\tArquivo '%s' não removido corretamente.\n", name);
    }
  }
}

void test_findByName() {
  char path0[30] = "/directory0/filename0";
  char path1[30] = "/directory1/filename1";
  char path2[30] = "/directory2/filename2";
  char path3[30] = "/directory3/filename3";
  char path4[30] = "/directory4/filename4";

  struct t2fs_record record0, record1, record2, record3, record4;
  record0 = initRecord(TYPEVAL_REGULAR, path0, 1, 0, -1);
  record1 = initRecord(TYPEVAL_REGULAR, path1, 1, 0, -1);
  record2 = initRecord(TYPEVAL_DIRETORIO, path2, 1, 0, -1);
  record3 = initRecord(TYPEVAL_REGULAR, path3, 1, 0, -1);
  record4 = initRecord(TYPEVAL_REGULAR, path4, 1, 0, -1);

  // Inserindo na LDAA
  insertLDAA(record0, path0);
  insertLDAA(record1, path1);
  insertLDAA(record2, path2);

  // Pesquisando na LDAA
  test_findByNameValidate(path0);
  test_findByNameValidate(path2);
  test_findByNameValidate(path4);

  test_findByNameValidate(path1);
  test_findByNameValidate(path3);
}

void test_insertSearch() {
  int i = getFreeLDAA();

  printf("Primeiro LDAA livre :: %d \n", i);

  printf("\t Inserindo no LDAA \n");

  struct t2fs_record record = {1, "Teste", 1, 1, 10 };

  if(insertLDAA(record, "Teste") != -1)
  printf("Inserido com sucesso \n");
  else
    printf("Erro ao inserir \n");

  printf("\t Procurando no LDAA \n");

  struct descritor result;
  if(searchLDAA(i, 1, &result) == TRUE){
    printf("Encontrado Handle %d \n", i);

    printf("\t Printando: \n");
    printf("typeVal = %d \n name = %s \n MFT = %d \n", result.record.TypeVal, result.record.name, result.record.MFTNumber);

    printf("\t Original: \n");
    printf("typeVal = %d \n name = %s \n MFT = %d \n", config.LDAA[i].record.TypeVal, config.LDAA[i].record.name, config.LDAA[i].record.MFTNumber);
  }
  else
  printf("Erro ao encontrar \n");
}

int main(int argc, char const *argv[]) {
  initConfig();

  test_insertSearch();

  test_findByName();

  return 0;
}
