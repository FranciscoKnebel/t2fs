#include "libs.h"

struct t2fs_4tupla initTupla(DWORD atributeType, DWORD VBN, DWORD LBN, DWORD numberOfContiguosBlocks) {
  struct t2fs_4tupla tupla;

  tupla.atributeType = REGISTER_MAP;
  tupla.virtualBlockNumber = VBN;
  tupla.logicalBlockNumber = LBN;
  tupla.numberOfContiguosBlocks = numberOfContiguosBlocks;

  return tupla;
}

int initFileRegister(int registerIndex, int VBN) {
  struct t2fs_4tupla tupla, tuplaFim;

  tupla     = initTupla(REGISTER_MAP, VBN, 0, 1);
  tuplaFim  = initTupla(REGISTER_FIM, 0, 0, 0);

  REGISTER_T reg;
  readRegister(registerIndex, &reg);

  writeTupla(reg.at, &tupla,    0);
  writeTupla(reg.at, &tuplaFim, 1);

  writeRegister(registerIndex, &reg);

  return 1;
}

struct t2fs_record createRecord(char* pathname) {
  struct t2fs_record newFile;

  char ** parsedPath = malloc(sizeof(char) * MAX_FILE_NAME_SIZE);
  int parseCount = parsePath(pathname, parsedPath);

  newFile.TypeVal = TYPEVAL_REGULAR;
  strcpy(newFile.name, parsedPath[parseCount]); //name é o último elemento parseado: /directory/directory/file1 -> name = file1
  newFile.blocksFileSize = 1;
  newFile.bytesFileSize = RECORD_SIZE;

  /* Encontrar registro do MFT livre, escrever registro e associar ao arquivo. */
  int fileVBN = searchBitmap2(BM_LIVRE); // Encontra bloco de dados para o arquivo
  setBitmap2(fileVBN, BM_OCUPADO);

  int registro = searchMFT(MFT_BM_LIVRE);
  initFileRegister(registro, fileVBN); // Inicializa registro no MFT do arquivo

  newFile.MFTNumber = registro; // Associa arquivo ao registro criado

  return newFile;
}

int addRecord(struct t2fs_record record, char * filename) {
  // adicionar record para o diretório

  return 0;
}
