/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Douglas Lazaro
  Francisco Knebel
*/

#ifndef __types__
#define	__types__

  #include "declarations.h"
  #include "t2fs.h"

  struct BootBlock {
    char id[5];
    char version[7];
    WORD blockSize;
    WORD MFTBlocksSize;
    DWORD diskSectorSize;
  };

  struct Constants {
    unsigned int SECTOR_PER_BLOCK;
    unsigned int BLOCK_SIZE;

    unsigned int DISK_SECTORS;
    unsigned int DISK_BLOCKS;

    /* Partições do disco */
    unsigned int BOOT_BLOCK_SIZE;
    unsigned int BOOT_SECTOR_SIZE;

    unsigned int MFT_BLOCK_SIZE;
    unsigned int MFT_SECTOR_SIZE;

    unsigned int DATA_BLOCK_SIZE;
    unsigned int DATA_SECTOR_SIZE;

    /* Blocos do disco */
    unsigned int BOOT_BLOCK;
    unsigned int MFT_BLOCK;
    unsigned int DATA_BLOCK;

    /* Blocos do disco */
    unsigned int BOOT_SECTOR;
    unsigned int MFT_SECTOR;
    unsigned int DATA_SECTOR;

    /* Registro */
    unsigned int REGISTER_SIZE;
    unsigned int REGISTER_PER_BLOCK;
    unsigned int MAX_REGISTERS;
    unsigned int MAX_SECTORS_REGISTER;

    /* Tupla */
    unsigned int MAX_TUPLAS_REGISTER;
    unsigned int TUPLA_SIZE;

    /* Records */
    unsigned int RECORD_PER_BLOCK;
  };


  /*
    Estrutura de configuração do T2FS,
    com valores necessários para execução do sistema.
  */
  struct Configuration {
    int initiated;

    struct BootBlock bootBlock;

    BYTE* indexMFT;

    struct descritor LDAA[MAX_FILES_OPEN];
  };

  /*
    Estruturas usadas para a leitura de
    setores, blocos e registros, respectivamente.
  */
  typedef struct {
    unsigned char at[SECTOR_SIZE];
  } SECTOR_T;

  typedef struct {
    unsigned char * at;
  } BLOCK_T;

  typedef struct {
    unsigned char at[SECTOR_SIZE * 2];
  } REGISTER_T;

#endif
