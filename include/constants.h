#ifndef __constants__
#define	__constants__

  #define FALSE 0
  #define TRUE 1

  /* ESSAS CONSTANTES PRECISAM VIRAR VARIAVEIS GLOBAIS */
  /* LIDAS DO BLOCO DE BOOT */
  #define SECTOR_PER_BLOCK 4

  #define BLOCK_SIZE SECTOR_PER_BLOCK * SECTOR_SIZE
  #define MFT_BLOCK_SIZE 2048
  #define DISK_SECTORS 32768
  #define TOTAL_BLOCKS DISK_SECTORS - (1 + MFT_BLOCK_SIZE)

  #define DATA_BLOCK MFT_BLOCK_SIZE + 1
  /* */

  /* Disco */
  #define SECTOR_SIZE 256


  #define BOOT_BLOCK 0
  #define MFT_BLOCK 1

#endif
