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
  printf("-- ENCERROU READ SECTOR --\n");
}

void test_readBlock() {
  printf("-- READ BLOCK --\n");
  BLOCK_T blockBuffer;
  readBlock(1, &blockBuffer);
  printBlock(blockBuffer.at);
  printf("-- ENCERROU READ BLOCK --\n");
}

void test_showBlock() {
  printf("-- SHOW BLOCK --\n");

  /* Mostrar blocos de importância */
  printf("-- BOOT BLOCK --\n");
  showBlock(constants.BOOT_BLOCK);
  printf("-- MFT BLOCK --\n");
  showBlock(constants.MFT_BLOCK);
  printf("-- DATA BLOCK --\n");
  showBlock(constants.DATA_BLOCK);

  printf("-- ENCERROU SHOW BLOCK --\n");
}

void test_writeSector() {
  printf("-- WRITE SECTOR --\n");

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

  printf("-- ENCERROU WRITE SECTOR --\n");
}

void test_writeBlock() {
  printf("-- WRITE BLOCK --\n");

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

  printf("-- ENCERROU WRITE BLOCK --\n");
}

void test_readRegister() {
  printf("-- READ REGISTER --\n");

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

  printf("-- ENCERROU READ REGISTER --\n");
}

/* READ REGISTER */
void test_writeRegister() {
  printf("-- WRITE REGISTER --\n");

  REGISTER_T reg;

  REGISTER_T registerBefore, registerAfter;

  int registerRead = 5;   // Um dos registros definidos no disco
  int registerWrite = 20; // Um registro não definido qualquer

  /* SAVE PREVIOUS REGISTER */
  if(readRegister(registerWrite, &reg) != TRUE) {
    return;
  }
  memcpy((void*) &registerBefore.at, (void*) &reg, sizeof(REGISTER_T)); // Copiando _valor_ para registerBefore

  /* GET REGISTER TO COPY */
  if(readRegister(registerRead, &reg) != TRUE) {
    return;
  }

  /* WRITE NEW REGISTER */
  if(writeRegister(registerWrite, &reg) != TRUE) {
    return;
  };

  /* READ WRITTEN REGISTER */
  if(readRegister(registerWrite, &registerAfter) != TRUE) {
    return;
  }
  printf("------------------------------\n");
  printf("--------- Before \n");
  printRegister(registerBefore.at, registerWrite);
  printf("--------- After \n");
  printRegister(registerAfter.at, registerWrite);
  printf("------------------------------\n");

  /* WRITE OLD VALUE BACK ON REGISTER */
  if(writeRegister(registerWrite, &registerBefore) != TRUE) {
    return;
  };

  printf("-- ENCERROU WRITE REGISTER --\n");
}

void test_readRecord() {
  printf("-- READ RECORD --\n");

  struct t2fs_record record1, record2;

  readRecord(2050, 0, &record1);
  readRecord(2050, 1, &record2);

  printRecord(record1);
  printf("\n");
  printRecord(record2);
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

  /* READ REGISTER */
  test_readRegister();

  /* WRITE REGISTER */
  test_writeRegister();

  /* READ RECORD */
  test_readRecord();

  /* WRITE RECORD */

}

int main(int argc, char const *argv[]) {
  initConfig();

  test_readRecord();

  return 0;
}
