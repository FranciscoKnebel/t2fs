/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Douglas Lazaro
  Francisco Knebel
*/

#include "libs.h"

int readFile(int handle, struct descritor descritor, char * buffer, int size) {
  int return_value = -1;
  int registerIndex = descritor.record.MFTNumber;
  char * tempBuffer = malloc(sizeof(char) * size);

  REGISTER_T reg;
  if(readRegister(registerIndex, &reg) != TRUE) {
    return FALSE;
  }

  struct t2fs_4tupla *tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));
  parseRegister(reg.at, tuplas);

  BLOCK_T blockBuffer;
  blockBuffer.at = malloc(sizeof(unsigned char) * constants.BLOCK_SIZE);

  int i = 0, bytesRead = 0, bytesLeft = size, amountOfBlocksRead = 0, block, allocated, isDone = FALSE, fileBlocksCounter = 0;
  while (i < constants.MAX_TUPLAS_REGISTER && isDone != TRUE) {
    if(bytesRead < size) {
      switch(tuplas[i].atributeType) {
        case REGISTER_MAP:
          while(amountOfBlocksRead < tuplas[i].numberOfContiguosBlocks) {
            block = tuplas[i].logicalBlockNumber + amountOfBlocksRead;
            amountOfBlocksRead++;

            if(readBlock(block, &blockBuffer) == FALSE) {
              return FALSE;
            };

            // Se bytesLeft for menor ou igual que o tamanho do bloco
              // copia todos os bytes para o blockBuffer.
              // zera bytesLeft.
              // atualiza bytesRead.
              // atualiza current pointer na LDAA
              // retorna bytesRead.
            // Se for maior
              // copia BLOCK_SIZE bytes para o buffer.
              // atualiza bytesRead e bytesLeft
              // parte para o próximo bloco


            /*memcpy(tempBuffer, &blockBuffer, bytesLeft - bytesRead);
            bytesRead += size;

            updateLDAA(handle, TYPEVAL_REGULAR, descritor);*/
          }

          fileBlocksCounter += amountOfBlocksRead;
          amountOfBlocksRead = 0;
          i++;

          break;
        case REGISTER_ADITIONAL:
        case REGISTER_FIM:
          return_value = REGISTER_FIM;
          break;
        default:
          break;
      }
    } else {
      return_value = bytesRead;
    }
  }

  /*
    Sa�da:	Se a opera��o foi realizada com sucesso, a fun��o retorna o n�mero de bytes lidos.
    Se o valor retornado for menor do que "size", ent�o o contador de posi��o atingiu o final do arquivo.

    Problemas: retornar negativo
  */

  return return_value;
}
