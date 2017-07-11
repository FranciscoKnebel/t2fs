/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Douglas Lazaro
  Francisco Knebel
*/

#include "libs.h"

int readDirectory(DIR2 handle, struct descritor descritor, DIRENT2 *dentry) {
  int return_value = -1;

  BLOCK_T blockBuffer;
  blockBuffer.at = malloc(sizeof(unsigned char) * constants.BLOCK_SIZE);

  REGISTER_T reg;
  if(readRegister(descritor.record.MFTNumber, &reg) != TRUE) {
    return REGISTER_READ_ERROR;
  }

  struct t2fs_4tupla *tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));
  parseRegister(reg.at, tuplas);

  unsigned int initialBlock = descritor.currentPointer / constants.BLOCK_SIZE;
  unsigned int recordIndex = (descritor.currentPointer % constants.BLOCK_SIZE) / RECORD_SIZE;
  unsigned int i = findOffsetTupla(tuplas, initialBlock, &reg);

  unsigned int currentBlock = tuplas[i].logicalBlockNumber + initialBlock;
  struct t2fs_record records[constants.RECORD_PER_BLOCK];
  //printf("CP: %d, IB: %d, RI: %d, i: %d, CB: %d\n", descritor.currentPointer, initialBlock, recordIndex, i, currentBlock);

  if(readBlock(currentBlock, &blockBuffer) == FALSE) {
    return FALSE;
  };

  parseDirectory(blockBuffer, records);

  struct t2fs_record file = records[recordIndex];
  DIRENT2 dirEntry_temp;

  // Verifica se chegou ao final
  int k = 0, foundFile = FALSE;
  if(records[recordIndex].TypeVal != TYPEVAL_DIRETORIO && records[recordIndex].TypeVal != TYPEVAL_REGULAR) {
    // Verifica no restante do bloco se há mais arquivos
    for(k = recordIndex+1; k < constants.RECORD_PER_BLOCK && foundFile != TRUE; k++) {
      if(records[k].TypeVal == TYPEVAL_DIRETORIO || records[k].TypeVal == TYPEVAL_REGULAR) {
        strcpy(dirEntry_temp.name, file.name);
        dirEntry_temp.fileType = file.TypeVal;
        dirEntry_temp.fileSize = file.bytesFileSize;

        memcpy(dentry, &dirEntry_temp, sizeof(DIRENT2));
        foundFile = TRUE;
        return_value = 0;
      }
    }

    // Verificar nos blocos contiguos se há mais arquivos

    // Verifica nas tuplas seguintes se há mais arquivos
      // precisa achar alguma tupla seguinte que é map.
      // PS: SE tuplas[i+1] for FIM, não tem.

    if(foundFile != TRUE) {
      return -END_OF_DIR;
    }
  } else {
    strcpy(dirEntry_temp.name, file.name);
    dirEntry_temp.fileType = file.TypeVal;
    dirEntry_temp.fileSize = file.bytesFileSize;

    memcpy(dentry, &dirEntry_temp, sizeof(DIRENT2));
    return_value = 0;
  }

  // Atualiza currentPointer na LDAA, para próximo arquivo.
  descritor.currentPointer += RECORD_SIZE;
  updateLDAA(handle, TYPEVAL_DIRETORIO, descritor);

  return return_value;
}
