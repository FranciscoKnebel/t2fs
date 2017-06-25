/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Testes dos métodos de src/disk.c

  Desenvolvido por Francisco Knebel
*/

#include "libs.h"

void test_readSector() {
  printf("-- READ SECTOR --\n");
  SECTOR_T sectorBuffer;
  readSector(0, &sectorBuffer);
  printSector(sectorBuffer.at);
}

void test_readBlock() {
  printf("-- READ BLOCK --\n");
  BLOCK_T blockBuffer;
  readBlock(1, &blockBuffer);
  printBlock(blockBuffer.at);
}

void test_showBlock() {
  /* Mostrar blocos de importância */
  printf("-- BOOT BLOCK --\n");
  showBlock(constants.BOOT_BLOCK);
  printf("-- MFT BLOCK --\n");
  showBlock(constants.MFT_BLOCK);
  printf("-- DATA BLOCK --\n");
  showBlock(constants.DATA_BLOCK);
}

void test_writeSector() {
  SECTOR_T sectorBuffer2, sectorBufferBackup;
  readSector(0, &sectorBuffer2);
  readSector(1, &sectorBufferBackup);

  writeSector(1, &sectorBuffer2);

  // Mostra setor modificado
  readSector(1, &sectorBuffer2);
  printSector(sectorBuffer2.at);

  // Restaura setor modificado
  writeSector(1, &sectorBufferBackup);
  readSector(1, &sectorBuffer2);
  printSector(sectorBuffer2.at);
}

void test_writeBlock() {
  BLOCK_T blockBuffer, blockBufferBackup;
  readBlock(0, &blockBuffer);
  readBlock(1, &blockBufferBackup);

  writeBlock(1, &blockBuffer);

  // Mostra bloco modificado
  readBlock(1, &blockBuffer);
  printBlock(blockBuffer.at);

  // Restaura bloco modificado
  writeBlock(1, &blockBufferBackup);
  readBlock(1, &blockBuffer);
  printBlock(blockBuffer.at);
}

void test_readRegister() {
  REGISTER_T reg;
  int registerIndex = 0;

  printf("Lendo registro %d\n", registerIndex);
  if(readRegister(registerIndex, &reg) != TRUE) {
    printf("Registro %d não existe, logo retorna erro.\n", registerIndex);
    return;
  }
  printRegister(reg.at, registerIndex);


  REGISTER_T reg2;
  int registerIndex2 = 4096;

  printf("\n\nLendo registro %d\n", registerIndex2);
  if(readRegister(registerIndex2, &reg2) != TRUE) {
    printf("Registro %d não existe, logo retorna erro.\n", registerIndex2);
    return;
  }

  printRegister(reg2.at, registerIndex2);
}

void old() {
  /* READ SECTOR */
  test_readSector();

  /* READ BLOCK */
  test_readBlock();

  /* SHOW BLOCK */
  test_showBlock();

  /* WRITE SECTOR */
  test_writeSector();

  /* WRITE BLOCK */
  test_writeBlock();
}

int main(int argc, char const *argv[]) {
  initConfig();

  /* READ REGISTER */
  test_readRegister();

  return 0;
}
