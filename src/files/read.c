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
  unsigned int bytesLeft;
  unsigned int amountOfBlocksRead = 0;

  if(size > descritor.record.bytesFileSize) {
    bytesLeft = descritor.record.bytesFileSize;
  } else {
    bytesLeft = size;
  }
  tempBuffer = malloc(sizeof(char) * bytesLeft);

  // Achar tupla e bloco inicial, de acordo com currentPointer.
  unsigned int initialBlock = descritor.currentPointer / constants.BLOCK_SIZE;
  unsigned int initialOffset = descritor.currentPointer % constants.BLOCK_SIZE;
  unsigned int foundStart = FALSE;
  i = 0;
  while (i < constants.MAX_TUPLAS_REGISTER && foundStart != TRUE) {
    switch(tuplas[i].atributeType) {
      case REGISTER_MAP:
        amountOfBlocksRead += tuplas[i].numberOfContiguosBlocks;

        if(initialBlock < amountOfBlocksRead) {
          foundStart = TRUE;
        } else {
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

        break;
        case REGISTER_FIM:
        default:
          // Current pointer está depois do final do arquivo
          return 0;
          break;
    }
  }

  while (i < constants.MAX_TUPLAS_REGISTER && bytesLeft > (unsigned int) 0) {
    switch(tuplas[i].atributeType) {
      case REGISTER_MAP:
        amountOfBlocksRead = 0;
        while(amountOfBlocksRead < tuplas[i].numberOfContiguosBlocks) {
          block = tuplas[i].logicalBlockNumber + amountOfBlocksRead;
          amountOfBlocksRead++;

          if(readBlock(block, &blockBuffer) == FALSE) {
            return FALSE;
          };

          if(bytesLeft <= constants.BLOCK_SIZE) {
            memcpy(&tempBuffer[bytesRead], blockBuffer.at + initialOffset, bytesLeft);
            bytesRead += bytesLeft;
            bytesLeft = 0;

            descritor.currentPointer += bytesRead;
            updateLDAA(handle, TYPEVAL_REGULAR, descritor);

            memcpy(buffer, tempBuffer, sizeof(char) * size);
            free(tempBuffer);

            return_value = bytesRead;
          } else {
            memcpy(&tempBuffer[bytesRead], blockBuffer.at + initialOffset, constants.BLOCK_SIZE);
            bytesRead += bytesLeft;
            bytesLeft -= constants.BLOCK_SIZE;
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
