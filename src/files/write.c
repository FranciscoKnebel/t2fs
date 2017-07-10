/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Douglas Lazaro
  Francisco Knebel
*/

#include "libs.h"

int writeFile(int handle, struct descritor descritor, char * buffer, unsigned int size) {
  int return_value = -1;
  int registerIndex = descritor.record.MFTNumber;

  REGISTER_T reg;
  if(readRegister(registerIndex, &reg) != TRUE) {
    return FALSE;
  }

  struct t2fs_4tupla *tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));
  parseRegister(reg.at, tuplas);

  BLOCK_T blockBuffer;
  blockBuffer.at = malloc(sizeof(unsigned char) * constants.BLOCK_SIZE);

  unsigned int i = 0, block;
  unsigned int bytesWritten = 0, bytesLeft = size;
  unsigned int amountOfBlocksRead = 0;

  // Achar tupla, bloco e offset inicial, de acordo com currentPointer.
  unsigned int initialBlock = descritor.currentPointer / constants.BLOCK_SIZE;
  unsigned int initialOffset = descritor.currentPointer % constants.BLOCK_SIZE;
  i = findOffsetTupla(tuplas, initialBlock, &reg);

  while (i < constants.MAX_TUPLAS_REGISTER && bytesLeft > (unsigned int) 0) {
    switch(tuplas[i].atributeType) {
      case REGISTER_MAP:
        amountOfBlocksRead = initialBlock;
        initialBlock = 0;
        while(amountOfBlocksRead < tuplas[i].numberOfContiguosBlocks && bytesLeft > (unsigned int) 0) {
          block = tuplas[i].logicalBlockNumber + amountOfBlocksRead;
          amountOfBlocksRead++;

          if(readBlock(block, &blockBuffer) == FALSE) {
            return FALSE;
          };

          if(bytesLeft <= constants.BLOCK_SIZE) {
            // Escreve dados no buffer do bloco, e depois escreve no disco.
            memcpy(blockBuffer.at + initialOffset, &buffer[bytesWritten], bytesLeft);
            writeBlock(block, &blockBuffer);

            // Chegou no final da escrita
            bytesWritten += bytesLeft;
            bytesLeft = 0;

            // Atualiza descritor e record na LDAA

            // Se o ponteiro estiver antes do final, não deve incrementar o tamanho do arquivo, e sim apenas sobreescrever
            if(descritor.currentPointer < descritor.record.bytesFileSize) {
              unsigned int offBW = descritor.record.bytesFileSize - descritor.currentPointer;
              if(offBW < bytesWritten) {
                descritor.record.bytesFileSize += bytesWritten - offBW;
              }
            } else {
              descritor.record.bytesFileSize += bytesWritten;
            }

            descritor.currentPointer += bytesWritten;
            descritor.record.blocksFileSize = (descritor.record.bytesFileSize / constants.BLOCK_SIZE) + 1;
            updateLDAA(handle, TYPEVAL_REGULAR, descritor);

            // Atualiza record no diretório
            addRecordToDirectory(descritor.record, descritor.name, TRUE);

            return_value = bytesWritten;
          } else {
            // Escreve dados no buffer do bloco, e depois escreve no disco.
            memcpy(blockBuffer.at + initialOffset, &buffer[bytesWritten], constants.BLOCK_SIZE - initialOffset);
            //writeBlock(block, &blockBuffer);


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
        return_value = bytesWritten;
        break;
      default:
        return_value = bytesWritten;
        break;
    }
  }

  return return_value;
}
