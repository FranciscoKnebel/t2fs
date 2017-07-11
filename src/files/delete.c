/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Douglas Lazaro
  Francisco Knebel
*/

#include "libs.h"

int removeFileFromDirectory(DWORD directoryMFTNumber, struct t2fs_record file) {
  int return_value;
  int registerIndex = directoryMFTNumber;

  REGISTER_T reg;
  if(readRegister(registerIndex, &reg) != TRUE) {
    return FALSE;
  }

  struct t2fs_4tupla *tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));
  parseRegister(reg.at, tuplas);

  struct t2fs_record records[constants.RECORD_PER_BLOCK];
  BLOCK_T blockBuffer;
  blockBuffer.at = malloc(sizeof(unsigned char) * constants.BLOCK_SIZE);

  unsigned int i = 0, amountOfBlocksRead = 0, removedFile = FALSE, block;

  while (i < constants.MAX_TUPLAS_REGISTER && removedFile != TRUE) {
    switch (tuplas[i].atributeType) {
      case REGISTER_MAP:
        while(amountOfBlocksRead < tuplas[i].numberOfContiguosBlocks && removedFile != TRUE) {
          block = tuplas[i].logicalBlockNumber + amountOfBlocksRead;
          amountOfBlocksRead++;

          if(readBlock(block, &blockBuffer) == FALSE) {
            return FALSE;
          };

          parseDirectory(blockBuffer, records);

          unsigned int j;
          for (j = 0; j < constants.RECORD_PER_BLOCK && removedFile != TRUE; j++) {
            if(strcmp(records[j].name, file.name) == 0 && (records[j].TypeVal == TYPEVAL_REGULAR || records[j].TypeVal == TYPEVAL_DIRETORIO)) {
              // Achou arquivo válido com o mesmo nome
              file.TypeVal = TYPEVAL_INVALIDO;

              if(writeRecord(tuplas[i].logicalBlockNumber, j, file) == FALSE) {
                return RECORD_WRITE_ERROR;
              };

              removedFile = TRUE;
              return_value = i;
            }
          }
        }

        amountOfBlocksRead = 0;

        if(removedFile != TRUE) {
          i++;
        }

      break;
    case REGISTER_FIM:
      // CHEGOU AO FIM DAS TUPLAS E NÃO ACHOU
      return REGISTER_FIM;
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
    default:
      return_value = FIND_REGISTER_FREE;
      i++;
      break;
    }
  }

  return return_value;
}

int removeFileFromMFT(struct t2fs_record file) {
  int registerIndex = file.MFTNumber;
  int check, return_value;
  BLOCK_T blockBuffer;
  blockBuffer.at = malloc(sizeof(unsigned char) * constants.BLOCK_SIZE);

  REGISTER_T reg;
  if(readRegister(registerIndex, &reg) != TRUE) {
    return FALSE;
  }

  struct t2fs_4tupla *tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));
  parseRegister(reg.at, tuplas);


  /* Desalocando registro do MFT usado. */
  check = setMFT(registerIndex, MFT_BM_LIVRE);
  if (check < 0) {
    return MFT_BM_ERROR;
  }

  // Marca registro como livre, para uso das funções de MFT.
  freeRegister(registerIndex);

  /* Desalocar blocos indicados pelas tuplas */
  unsigned int i = 0, amountOfBlocksRead = 0, block;
  while (i < constants.MAX_TUPLAS_REGISTER) {
    switch (tuplas[i].atributeType) {
      case REGISTER_MAP:
        while(amountOfBlocksRead < tuplas[i].numberOfContiguosBlocks) {
          block = tuplas[i].logicalBlockNumber + amountOfBlocksRead;
          amountOfBlocksRead++;

          check = setBitmap2(block, BM_LIVRE);
          if (check < 0) {
            return BM_ERROR;
          }
        }

        amountOfBlocksRead = 0;
        i++;
        break;
      case REGISTER_ADITIONAL:
        registerIndex = tuplas[i].virtualBlockNumber;

        if(readRegister(registerIndex, &reg) != TRUE) {
          return FALSE;
        }

        /* Desalocando registro do MFT usado. */
        check = setMFT(registerIndex, MFT_BM_LIVRE);
        if (check < 0) {
          return MFT_BM_ERROR;
        }

        free(tuplas);
        tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));

        parseRegister(reg.at, tuplas);
        i = 0; // reset i para 0, começar a ler tuplas novamente

        // Marca registro como livre, para uso das funções de MFT.
        freeRegister(registerIndex);

        break;
      case REGISTER_FIM:
        return REGISTER_FIM;
        break;
      default:
        return_value = tuplas[i].atributeType;
        i++;
        break;
    }
  }

  return return_value;
}

int deleteFileFromDisk(struct t2fs_record file, char* filename) {
  /* Remover record do diretório */
  char * directoryname = malloc(strlen(filename));
  int return_value = -1;

  getFileDirectory(filename, directoryname);

  if(strcmp("/", directoryname) == 0) { // remover da root
    return_value = removeFileFromDirectory(REGISTER_ROOT, file);
  } else {
    struct t2fs_record directory;

    return_value = lookup(directoryname, &directory);

    switch (return_value) {
      case REGISTER_READ_ERROR:
        printf("Erro crítico na leitura de um registro.\n");
        break;
      case FIND_REGISTER_ADITIONAL:
        printf("ERRO! Valor de retorno de lookup nunca deve ser FIND_REGISTER_ADITIONAL.\n");
        break;
      case FIND_REGISTER_NOTFOUND:
      case FIND_REGISTER_FIM:
      case FIND_REGISTER_FREE:
        printf("Diretório '%s' não encontrado.\n", directoryname);
        break;
      default:
        return_value = removeFileFromDirectory(directory.MFTNumber, file);
        break;
    }
  }

  /* Desalocar blocos e registros do arquivo */
  removeFileFromMFT(file);

  return return_value;
}
