/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Douglas Lazaro
  Francisco Knebel
*/

#include "libs.h"

int readFile(int handle, struct descritor descritor, char * buffer, unsigned int size) {
  if(descritor.currentPointer >= descritor.record.bytesFileSize) {
    // Ponteiro após o final do arquivo.
    return 0;
  }

  int return_value = -1;
  int registerIndex = descritor.record.MFTNumber;
  char * tempBuffer;

  REGISTER_T reg;
  if(readRegister(registerIndex, &reg) != TRUE) {
    return FALSE;
  }

  struct t2fs_4tupla *tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));
  parseRegister(reg.at, tuplas);

  BLOCK_T blockBuffer;
  blockBuffer.at = malloc(sizeof(unsigned char) * constants.BLOCK_SIZE);

  unsigned int i = 0, bytesRead = 0, block;
  unsigned int bytesLeft, cpySize;
  unsigned int amountOfBlocksRead = 0;

  if(size > descritor.record.bytesFileSize) {
    bytesLeft = descritor.record.bytesFileSize;
  } else {
    bytesLeft = size;
  }
  cpySize = bytesLeft;
  tempBuffer = malloc(sizeof(char) * cpySize);

  // Achar tupla, bloco e offset inicial, de acordo com currentPointer.
  unsigned int initialBlock = descritor.currentPointer / constants.BLOCK_SIZE;
  unsigned int initialOffset = descritor.currentPointer % constants.BLOCK_SIZE;
  i = findOffsetTupla(tuplas, initialBlock, &reg);

  while (i < constants.MAX_TUPLAS_REGISTER && bytesLeft > (unsigned int) 0) {
    switch(tuplas[i].atributeType) {
      case REGISTER_MAP:
        amountOfBlocksRead = initialBlock;
        initialBlock = 0;
        while(amountOfBlocksRead < tuplas[i].numberOfContiguosBlocks) {
          block = tuplas[i].logicalBlockNumber + amountOfBlocksRead;
          amountOfBlocksRead++;

          if(readBlock(block, &blockBuffer) == FALSE) {
            return FALSE;
          };

          if(bytesLeft <= constants.BLOCK_SIZE) {
            int bytes = bytesLeft;
             // Caso de borda, se leitura vai estrapolar tamanho do arquivo.
             // Se sim, bytes lidos apenas até o final do arquivo.
            if(initialOffset + bytesLeft > descritor.record.bytesFileSize) {
              bytes = bytesLeft - (initialOffset + bytesLeft) % descritor.record.bytesFileSize;
            }

            memcpy(&tempBuffer[bytesRead], blockBuffer.at + initialOffset, bytes);
            bytesRead += bytes;
            descritor.currentPointer += bytesRead;
            updateLDAA(handle, TYPEVAL_REGULAR, descritor);

            memcpy(buffer, tempBuffer, sizeof(char) * cpySize);
            bytesLeft = 0;
            return_value = bytesRead;
          } else {
            memcpy(&tempBuffer[bytesRead], blockBuffer.at + initialOffset, constants.BLOCK_SIZE);
            bytesRead += constants.BLOCK_SIZE;
            bytesLeft -= constants.BLOCK_SIZE;

            if(tuplas[i+1].atributeType == REGISTER_FIM) {
              descritor.currentPointer += bytesRead;
              updateLDAA(handle, TYPEVAL_REGULAR, descritor);

              memcpy(buffer, tempBuffer, sizeof(char) * cpySize);

              bytesLeft = 0;
              return_value = bytesRead;
            }
          }

          initialOffset = 0;
        }

        if(bytesLeft > 0) {
          i++;
        }

        break;
      case REGISTER_ADITIONAL:
        // Ler novo registro e recomeçar a leitura.
        registerIndex = tuplas[i].virtualBlockNumber;

        if(readRegister(registerIndex, &reg) != TRUE) {
          return FALSE;
        }
        free(tuplas);
        tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));

        parseRegister(reg.at, tuplas);
        i = 0; // reset i para 0, começar a ler tuplas novamente

      case REGISTER_FIM:
        bytesLeft = 0;
        return_value = bytesRead;
        break;
      default:
        return_value = bytesRead;
        break;
    }
  }

  return return_value;
}
