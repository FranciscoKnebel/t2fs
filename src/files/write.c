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

  int allocated, fileBlocksCounter = 0;
  unsigned int i = 0, block;
  unsigned int bytesWritten = 0, bytesLeft = size;
  unsigned int amountOfBlocksRead = 0;

  // Achar tupla, bloco e offset inicial, de acordo com currentPointer.
  unsigned int bytesWrittenToBlock = 0;
  unsigned int initialBlock = descritor.currentPointer / constants.BLOCK_SIZE;
  unsigned int initialOffset = descritor.currentPointer % constants.BLOCK_SIZE;
  i = findOffsetTupla(tuplas, initialBlock, &reg);

  while (i < constants.MAX_TUPLAS_REGISTER && bytesLeft > (unsigned int) 0) {
    switch(tuplas[i].atributeType) {
      case REGISTER_MAP:
        amountOfBlocksRead = initialBlock;
        initialBlock = 0;

        bytesWrittenToBlock = initialOffset;
        while(amountOfBlocksRead < tuplas[i].numberOfContiguosBlocks && bytesLeft > (unsigned int) 0) {
          block = tuplas[i].logicalBlockNumber + amountOfBlocksRead;

          if(readBlock(block, &blockBuffer) == FALSE) {
            return FALSE;
          };

          if(bytesLeft <= constants.BLOCK_SIZE) {
            // Escreve dados no buffer do bloco, e depois escreve no disco.
            memcpy(&blockBuffer.at[initialOffset], &buffer[bytesWritten], bytesLeft);
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
            memcpy(&blockBuffer.at[initialOffset], &buffer[bytesWritten], constants.BLOCK_SIZE);
            writeBlock(block, &blockBuffer);

            bytesWritten += constants.BLOCK_SIZE;
            bytesLeft -= constants.BLOCK_SIZE;
          }

          // Verificação se escreveu até o final do bloco. Se sim, incrementa o contador.
          bytesWrittenToBlock += bytesWritten;
          if(bytesWrittenToBlock >= constants.BLOCK_SIZE) {
            bytesWrittenToBlock = 0;
            amountOfBlocksRead++;
          }

          initialOffset = 0;
        }

        fileBlocksCounter += amountOfBlocksRead;

        if(bytesLeft > 0) {
          if(tuplas[i+1].atributeType == REGISTER_FIM) {
            // verificar se é possivel criar bloco contiguo na tupla atual
            block = tuplas[i].logicalBlockNumber + tuplas[i].numberOfContiguosBlocks;
            allocated = getBitmap2(block);

            if(allocated < 0) {
              return BM_ERROR;
            }

            if(allocated == BM_LIVRE) {
              // Aloca bloco contíguo, atualizando o registro
              tuplas[i].numberOfContiguosBlocks += 1;
              writeTupla(reg.at, &tuplas[i], i);
              setBitmap2(block, BM_OCUPADO);

              resetBlock(block);

              writeRegister(registerIndex, &reg);

              // Loop de tuplas começara novamente, partindo do novo bloco.
              initialBlock = tuplas[i].numberOfContiguosBlocks - 1;
            } else {
              // próxima tupla está no final do registro
              if(i+1 == constants.MAX_TUPLAS_REGISTER - 1) {
                //próxima tupla vira uma REGISTER_ADITIONAL

                /* Encontra indice para o novo registro. */
                int novoRegisterIndex = searchMFT(MFT_BM_LIVRE);
                int check = setMFT(novoRegisterIndex, MFT_BM_OCUPADO);
                if (check < 0) {
                  return MFT_BM_ERROR;
                }

                tuplas[i+1] = initTupla(REGISTER_ADITIONAL, novoRegisterIndex, 0, 0);
                writeTupla(reg.at, &tuplas[i+1], i+1);
                writeRegister(registerIndex, &reg);

                /* Operações no novo registro */
                int fileLBN;

                fileLBN = searchBitmap2(BM_LIVRE); // Encontra bloco de dados para o arquivo
                check = setBitmap2(fileLBN, BM_OCUPADO);
                if (check < 0) {
                  return BM_ERROR;
                }

                // Inicializa o novo registro.
                initNewRegister(novoRegisterIndex, fileBlocksCounter, fileLBN);

                i = 0; // Reinicia o loop, no novo registro.
                if(readRegister(registerIndex, &reg) != TRUE) {
                  return FALSE;
                }
                parseRegister(reg.at, tuplas);
              } else { // próxima tupla vira um REGISTER_MAP
                int newLBN = searchBitmap2(BM_LIVRE); // Encontra bloco de dados para o arquivo

                // alocar novo bloco, setar como ocupado
                int check = setBitmap2(newLBN, BM_OCUPADO);
                if (check < 0) {
                  return BM_ERROR;
                }
                resetBlock(newLBN);

                /* ATUALIZAÇÃO DO REGISTRO */
                readRegister(registerIndex, &reg);

                tuplas[i+1] = initTupla(REGISTER_MAP, fileBlocksCounter, newLBN, 1);
                writeTupla(reg.at, &tuplas[i+1], i+1);

                tuplas[i+2] = initTupla(REGISTER_FIM, 0, 0, 0);
                writeTupla(reg.at, &tuplas[i+2], i+2);

                // Atualização do registro com novos valores.
                writeRegister(registerIndex, &reg);

                i++; // próxima iteração do loop ira escrever no novo bloco
              }
            }
          } else {
            // indica que o arquivo tem outras tuplas já alocadas
            i++;
          }
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
