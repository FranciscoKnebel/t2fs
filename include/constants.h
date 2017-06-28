#ifndef __constants__
#define	__constants__

  #define FALSE 0
  #define TRUE 1

  /* Disco */
  #define SECTOR_SIZE 256

  /* Registro */
  #define REGISTER_BOOT_BLOCK 0
  #define REGISTER_ROOT 1
  #define REGISTER_REGULAR 4

  #define REGISTER_FIM 0
  #define REGISTER_MAP 1
  #define REGISTER_ADITIONAL 2
  #define REGISTER_FREE -1

  /* Record */
  #define RECORD_SIZE 64
  #define RECORD_TYPE 0
  #define RECORD_NAME 1
  #define RECORD_BLOCK_FILE_SIZE 52
  #define RECORD_BYTES_FILE_SIZE 56
  #define RECORD_MFT_NUMBER 60

  /* Arquivo */
  #define FILENAME_SIZE_LIMIT 51

#endif
