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
  blockBuffer.at = malloc(sizeof(unsigned char) * constants.BLOCK_SIZE);

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
  blockBuffer.at = malloc(sizeof(unsigned char) * constants.BLOCK_SIZE);
  blockBufferBackup.at = malloc(sizeof(unsigned char) * constants.BLOCK_SIZE);

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

  printf("-- ENCERROU READ RECORD --\n");
}

void test_executeWriteRecord(int block, int index, struct t2fs_record record) {
  if (writeRecord(block, index, record) == FALSE) {
    printf("Erro no write record: ");

    if(index >= constants.RECORD_PER_BLOCK) {
      printf("Index passado maior do que %d. ", constants.RECORD_PER_BLOCK);
    }

    if(index < 0) {
      printf("Index passado menor do que %d. ", 0);
    }

    if(block >= constants.DISK_BLOCKS) {
      printf("Número de bloco maior do que %d (acima do limite do disco). ", constants.DISK_BLOCKS -1);
    }

    if(block < constants.DATA_BLOCK) {
      printf("Número de bloco menor do que %d (antes do bloco de dados). ", constants.DATA_BLOCK);
    }

    printf("\n");
  } else {
    printf("Sucesso no write record.\n");
  }
}

void test_writeRecord() {
  printf("-- WRITE RECORD --\n");

  printf("Lendo records...\n");
  struct t2fs_record record1, record2;
  readRecord(2050, 0, &record1);
  readRecord(2050, 1, &record2);

  printf("Alterando valores para teste...\n");
  strcpy(record1.name, "file3");
  record1.MFTNumber = 6;

  strcpy(record2.name, "file4");
  record2.MFTNumber = 7;

  printf("Backup de records...\n");
  struct t2fs_record record1_backup, record2_backup, record3_backup;
  readRecord(3000, 5, &record1_backup);
  readRecord(3000, 11, &record2_backup);
  readRecord(3000, 15, &record3_backup);

  printf("Escrevendo records modificados...\n");
  test_executeWriteRecord(3000, 5, record1);
  test_executeWriteRecord(3000, 11, record2);
  test_executeWriteRecord(3000, 15, record2);

  /* Erros */
  test_executeWriteRecord(3000, 16, record2);
  test_executeWriteRecord(3000, -1, record2);
  test_executeWriteRecord(constants.DATA_BLOCK -1, 15, record2);
  test_executeWriteRecord(constants.DISK_BLOCKS, 15, record2);

  printf("\nImprimindo diretório alterado...\n");
  BLOCK_T blockBuffer;
  blockBuffer.at = malloc(sizeof(unsigned char) * constants.BLOCK_SIZE);
  struct t2fs_record records[constants.RECORD_PER_BLOCK];

  if(readBlock(3000, &blockBuffer) == FALSE) {
    return;
  };
  parseDirectory(blockBuffer, records);

  int i;
  for (i = 0; i < constants.RECORD_PER_BLOCK; i++) {
    printRecord(records[i]);
    printf("\n");
  }

  printf("Restaurando valores antigos...\n");
  writeRecord(3000, 5, record1_backup);
  writeRecord(3000, 11, record2_backup);
  writeRecord(3000, 15, record3_backup);

  printf("-- ENCERROU WRITE RECORD --\n");
}

void test_freeRegister() {
  printf("-- FREE REGISTER --\n");

  REGISTER_T reg;
  readRegister(REGISTER_ROOT, &reg);
  printRegister(reg.at, 0);

  getchar();

  freeRegister(REGISTER_ROOT);
  readRegister(REGISTER_ROOT, &reg);
  printRegister(reg.at, 0);

  printf("-- ENCERROU FREE REGISTER --\n");
}

int main(int argc, char const *argv[]) {
  initConfig();

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
  test_writeRecord();

  /* FREE REGISTER */
  test_freeRegister();

  return 0;
}
