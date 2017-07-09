/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Testes dos métodos de API: create

  Desenvolvido por Francisco Knebel
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libs.h"

#define IDENTIFY_SIZE 80

FILE2 test_executeCreate2(char * filename) {
  FILE2 handle;

  handle = create2(filename);

  if(handle < 0) {
    printf("Erro no create2: ");

    switch (handle) {
      case PARSED_PATH_ERROR:
        printf("Path '%s' inválida.\n", filename);
        break;
      case FOUND_FILE_ERROR:
        printf("Arquivo '%s' já existe.\n", filename);
        break;
      case DIRECTORY_NOT_FOUND:
        printf("Um dos diretórios da path '%s' não existe.\n", filename);
        break;
      case FILE_LIMIT_REACHED:
        printf("Máximo de arquivos abertos alcançado.\n");
        break;
      default:
        printf("Erro %d\n", handle);
    }

  } else {
    printf("Arquivo '%s' criado com handle '%d'.\n", filename, handle);
    close2(handle);
  }

  return handle;
}

void test_create2_error() {
  char path1[50] = "batata";
  char path2[50] = "/batata/batata";
  char path3[50] = "/file1";
  char path4[50] = "/file/FILE2";

  test_executeCreate2(path1);
  test_executeCreate2(path2);
  test_executeCreate2(path3);
  test_executeCreate2(path4);
}

void test_create2_maxFiles() {
  char path[50] = "/test_";

  int i = 0;
  for (i = 0; i <= MAX_FILES_OPEN; i++) {
    char aux[50];

    sprintf(aux, "%s%d", path, i);
    test_executeCreate2(aux);
  }
}

void test_create2_root() {
  char path1[50] = "/file3";
  char path2[50] = "/file4";
  char path3[50] = "/file5";

  test_executeCreate2(path1);
  test_executeCreate2(path2);
  test_executeCreate2(path3);
}

void test_create2_showfiles_parseandprint(int block) {
  BLOCK_T blockBuffer;
  blockBuffer.at = malloc(sizeof(unsigned char) * constants.BLOCK_SIZE);
  struct t2fs_record records[constants.RECORD_PER_BLOCK];
  int i;

  if(readBlock(block, &blockBuffer) == FALSE) {
    return;
  };
  parseDirectory(blockBuffer, records);

  for (i = 0; i < constants.RECORD_PER_BLOCK; i++) {
    printRecord(records[i]);
    printf("\n");
  }
}

void test_create2_newmap_showfiles() {
  REGISTER_T reg;
  readRegister(REGISTER_ROOT, &reg);

  struct t2fs_4tupla *tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));
  parseRegister(reg.at, tuplas);

  int i = 0;
  int blocks[constants.MAX_TUPLAS_REGISTER];

  while(tuplas[i].atributeType != REGISTER_FIM) {
    // printTupla(tuplas[i]);
    blocks[i] = tuplas[i].logicalBlockNumber;

    i++;
  }

  test_create2_showfiles_parseandprint(blocks[0]);
  getchar();
  test_create2_showfiles_parseandprint(blocks[1]);
  getchar();
}

void test_create2_newmap() {
  char path[50] = "/test_";

  int i = 0;
  for (i = 0; i <= constants.RECORD_PER_BLOCK; i++) {
    char aux[50];

    sprintf(aux, "%s%d", path, i);
    test_executeCreate2(aux);
  }

  test_create2_newmap_showfiles();
}

void test_create2_aditional_show() {
  REGISTER_T reg;
  readRegister(REGISTER_ROOT, &reg);

  struct t2fs_4tupla *tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));
  parseRegister(reg.at, tuplas);

  int i = 0;
  while(tuplas[i].atributeType != REGISTER_FIM && i < constants.MAX_TUPLAS_REGISTER) {
    if(tuplas[i].atributeType == REGISTER_ADITIONAL) {
      printf("REGISTRO ADICIONAL!\n");
      readRegister(tuplas[i].virtualBlockNumber, &reg);

      free(tuplas);
      tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));
      parseRegister(reg.at, tuplas);

      i = 0;
    } else if(tuplas[i].atributeType == REGISTER_MAP) {
      test_create2_showfiles_parseandprint(tuplas[i].logicalBlockNumber);
      getchar();

      i++;
    }
  }
}

void test_create2_aditional() {
  char path[50] = "/test_";

  int i = 0;
  for (i = 0; i < (constants.RECORD_PER_BLOCK * constants.MAX_TUPLAS_REGISTER); i++) {
    char aux[50];

    sprintf(aux, "%s%d", path, i);
    test_executeCreate2(aux);
  }

  test_create2_aditional_show();
}

void test_create2() {
  /* CREATE FILE */
  printf("--- CREATE2 ---\n");

  //test_create2_error();

  //test_create2_maxFiles();

  //test_create2_root();

  //test_create2_newmap();

  test_create2_aditional();

  printf("--- ENCERRANDO CREATE2 ---\n");
  return;
}

int main(int argc, char const *argv[]) {
  char identify[IDENTIFY_SIZE];

  identify2(identify, IDENTIFY_SIZE);
  printf("%s\n\n", identify);

  initConfig();

  test_create2();

  return 0;
}
