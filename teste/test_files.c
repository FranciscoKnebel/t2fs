#include "libs.h"

void test_callValidPath(char* path) {
  if(validPath(path) == FALSE) {
    printf("Erro na path \"%s\".\n", path);
  } else {
    printf("Path \"%s\" é válida.\n", path);
  }
}

void test_validPath() {
  char path0[20] = "/um/arquivo.txt";
  char path1[20] = "/";
  char path2[20] = "";
  char path3[20] = "/valido.txt";
  char path4[20] = "/valido/show.dat";
  char path5[20] = "/invalido//inval.bin";
  char path6[60] = "/invalido/invalido/invalido/invalido/invalido/invalido.bin";

  printf("--- Teste de validação de path ---\n\n");

  test_callValidPath(path0);
  test_callValidPath(path1);
  test_callValidPath(path2);
  test_callValidPath(path3);
  test_callValidPath(path4);
  test_callValidPath(path5);
  test_callValidPath(path6);

  printf("\n--- Acabou o teste de validação de path ---\n");
}

int test_printPath(char * pathname, char ** elements) {
  int i, count;

  count = parsePath(pathname, elements);

  printf("\"%s\"\n\t", pathname);
  for (i = 0; i < count; i++) {
    printf("'%s' ", elements[i]);
  }
  printf("\n\n");

  free(elements);
  elements = malloc(sizeof(char) * MAX_FILE_NAME_SIZE);

  return i;
}

void test_parsePath() {
  char ** elements = malloc(sizeof(char) * MAX_FILE_NAME_SIZE);

  char path1[20] = "/arquivo.txt";
  char path2[20] = "/";
  char path3[20] = "/valido.txt";
  char path4[20] = "/valido/show.dat";
  char path5[60] = "/valido/arquivo/dentro/de/diretorio/com/arquivo.dat";

  printf("\n--- Teste de parsing de path ---\n\n");

  test_printPath(path1, elements);
  test_printPath(path2, elements);
  test_printPath(path3, elements);
  test_printPath(path4, elements);
  test_printPath(path5, elements);

  printf("--- Acabou o teste de parsing de path ---\n");
}

void test_parseIsFileName(char * path) {
  char ** elements = malloc(sizeof(char) * MAX_FILE_NAME_SIZE);

  int count = parsePath(path, elements);

  if(count == FALSE) {
    printf("'%s':\n\t", path);
    printf("é uma path inválida.\n\n");
  } else {
    // Path válida, verifica se o último elemento é um nome de arquivo normal ou diretório.
    printf("\"%s\":\n\t", path);
    printf("%s\n\n", isFileName(elements[count - 1]) ? "é um nome de diretório." : "é um nome de arquivo.");
  }
}

void test_isFileName() {
  char path0[30] = "/show";
  char path1[30] = "/arquivo.txt";
  char path2[30] = "/valido/";
  char path3[30] = "show.dat";
  char path4[30] = "/valido/show/de/bola.dat";
  char path5[60] = "/path/invalida/de/teste/path/invalida/de/teste/invalido.txt";
  char path6[30] = "/";
  char path7[40] = "/valido/testando/nos/ultimos/niveis.dat";
  char path8[40] = "/valido/testando/nos/ultimos/niveis";
  char path9[40] = "/valido/show.dat";

  printf("\n--- Teste de verificação de filename ---\n\n");

  test_parseIsFileName(path0);
  test_parseIsFileName(path1);
  test_parseIsFileName(path2);
  test_parseIsFileName(path3);
  test_parseIsFileName(path4);
  test_parseIsFileName(path5);
  test_parseIsFileName(path6);
  test_parseIsFileName(path7);
  test_parseIsFileName(path8);
  test_parseIsFileName(path9);

  printf("--- Acabou o teste de verificação de filename ---\n");
}

int main(int argc, char const *argv[]) {
  /* Valid Path */
  test_validPath();

  /* Parse Path */
  test_parsePath();

  /* Valid File Name */
  test_isFileName();

  return 0;
}
