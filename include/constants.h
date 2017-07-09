/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Douglas Lazaro
  Francisco Knebel

  Declarações de constantes usadas por todo o T2FS.
*/

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

  #define REGISTER_READ_ERROR -5

  /* Record */
  #define RECORD_SIZE 64
  #define RECORD_TYPE 0
  #define RECORD_NAME 1
  #define RECORD_BLOCK_FILE_SIZE 52
  #define RECORD_BYTES_FILE_SIZE 56
  #define RECORD_MFT_NUMBER 60

  /* Arquivo */
  #define MAX_FILE_NAME_SIZE 51

  /* Lookup */
  #define FIND_REGISTER_NOTFOUND -1
  #define FIND_REGISTER_FIM -2
  #define FIND_REGISTER_ADITIONAL -3
  #define FIND_REGISTER_FREE -4
  #define FILE_NOT_FOUND -1

  /* LDAA */
  #define MAX_FILES_OPEN 40
  #define NOT_FOUND_LDAA -1
  #define ERRO_REMOVE_LDAA -2

  /* Bitmap */
  #define BM_LIVRE 0
  #define BM_OCUPADO 1
  #define BM_ERROR -100

  /* MFT Bitmap */
  #define MFT_BM_LIVRE 0
  #define MFT_BM_OCUPADO 1
  #define MFT_BM_ERROR -100

  /* API */
  #define FOUND_FILE_ERROR -1
  #define EOF_OFFSET -1
  #define DIRECTORY_NOT_FOUND -7
  #define FILE_LIMIT_REACHED -8
  #define PARSED_PATH_ERROR -9
  #define RECORD_WRITE_ERROR -10
  #define READDIR_ERROR -11

  /* Add to Directory */
  #define ADDDIR_CONTIGUOUS 200
  #define ADDDIR_ADITIONAL 201
  #define ADDDIR_MAP 202

#endif
