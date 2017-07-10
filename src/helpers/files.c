/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Douglas Lazaro
  Francisco Knebel
*/

#include "libs.h"

int validPath(char* path) {
  int lastWasSlash = FALSE;

  int length = strlen(path);

  // Min Size || Path não começa no root
  if(length == 0 || path[0] != '/') {
    return FALSE;
  }

  // Dupla Barra
  int i;
  for(i = 0; i < length; i++) {
    if(path[i] == '/') {
      if (lastWasSlash == TRUE) {
        return FALSE;
      }

      lastWasSlash = TRUE;
    } else {
      lastWasSlash = FALSE;
    }
  }

  // Max Size
  char* temp = malloc(strlen(path));
  strcpy(temp, path);
  char * pch = strtok(temp, "/");

  while(pch != NULL) {
    if(strlen(pch) > MAX_FILE_NAME_SIZE) {
      return FALSE;
    }

    pch = strtok(NULL, "/");
  }

  return TRUE;
}

int isFileName(char* filename) {
  if(strlen(filename) == 0) {
    return -1;
  }

  char* temp = malloc(strlen(filename));
  strcpy(temp, filename);

  char * pch = strtok(temp, ".");

  if(strcmp(pch, filename)) { //nenhum ponto foi encontrado
    return FALSE; // é um nome de diretório
  }

  return TRUE; // is a directory
}

int getFileDirectory(char* path, char* directory) {
  char ** elements = malloc(sizeof(char) * MAX_FILE_NAME_SIZE);

  int count = parsePath(path, elements);
  if(count >= 2) {
    int i;
    sprintf(directory, "/%s", elements[1]);
    for (i = 2; i < count; i++) {
      sprintf(directory, "%s/%s", directory, elements[i]);
    }
  } else {
    strcpy(directory, "/");
  }

  return TRUE;
}

int parsePath(char* path, char ** elements) {
  if(validPath(path) == FALSE) {
    return FALSE;
  }

  char opPath[MAX_FILE_NAME_SIZE];
  strcpy(opPath, path);

  char * pch = strtok(opPath, "/");

  int i = 0;
  elements[i++] = "/";

  while(pch != NULL) {
    if(strlen(pch) > MAX_FILE_NAME_SIZE) {
      return FALSE;
    }

    elements[i] = (char*) malloc((strlen(pch) + 1) * sizeof(char));

    strcpy(elements[i++], pch);

    pch = strtok(NULL, "/");
  }

  return i -1;
}

int findRecord(struct t2fs_4tupla tupla, char* name, struct t2fs_record * record) {
  BLOCK_T blockBuffer;
  blockBuffer.at = malloc(sizeof(unsigned char) * constants.BLOCK_SIZE);

  struct t2fs_record records[constants.RECORD_PER_BLOCK];
  int returnValue = TRUE;
  int foundFile = FALSE;
  unsigned int amountOfBlocksRead = 0;
  unsigned int currentBlock, i = 0;

  switch (tupla.atributeType) {
    case REGISTER_MAP:
      while(amountOfBlocksRead < tupla.numberOfContiguosBlocks && foundFile != TRUE) {
        currentBlock = tupla.logicalBlockNumber + amountOfBlocksRead;
        amountOfBlocksRead++;

        if(readBlock(currentBlock, &blockBuffer) == FALSE) {
          return FALSE;
        };

        parseDirectory(blockBuffer, records);

        for (i = 0; i < constants.RECORD_PER_BLOCK && foundFile != TRUE; i++) {
          if(strcmp(records[i].name, name) == 0 && (records[i].TypeVal == TYPEVAL_REGULAR || records[i].TypeVal == TYPEVAL_DIRETORIO)) { // FILE NAME FOUND
            memcpy((void*) record, (void*) &records[i], RECORD_SIZE);

            foundFile = TRUE;
          }
        }
      }

      returnValue = foundFile == TRUE ? (int) i-1 : FIND_REGISTER_NOTFOUND; // 0 to RECORD_PER_BLOCK-1

      break;
    case REGISTER_FIM:
      returnValue = FIND_REGISTER_FIM;
      break;
    case REGISTER_ADITIONAL:
      returnValue = FIND_REGISTER_ADITIONAL;
      break;
    case REGISTER_FREE:
    default:
      returnValue = FIND_REGISTER_FREE;
      break;
  }

  return returnValue;
}

int lookup(char* pathname, struct t2fs_record * fileRecord) {
  char ** parsedPath = malloc(sizeof(char) * MAX_FILE_NAME_SIZE);
  unsigned int parseCount = parsePath(pathname, parsedPath);

  if(parseCount == FALSE) {
    return PARSED_PATH_ERROR;
  }

  REGISTER_T root;
  struct t2fs_4tupla * tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));

  if(readRegister(REGISTER_ROOT, &root) != TRUE) {
    return FALSE;
  }
  parseRegister(root.at, tuplas);

  /* ITERAR NA ÁRVORE ATÉ ACHAR FOLHA */
  unsigned int i = 0, j = 1;
  int found = FALSE, endReached = FALSE;

  while(i < constants.MAX_TUPLAS_REGISTER && endReached != TRUE) {
    found = findRecord(tuplas[i], parsedPath[j], fileRecord);

    switch (found) {
      case FIND_REGISTER_FIM:
      case FIND_REGISTER_FREE:
        endReached = TRUE;

        if(j < parseCount) {
          found = DIRECTORY_NOT_FOUND;
        }

        break;
      case FIND_REGISTER_ADITIONAL:
        // ler novo MFT Register, indicado pelo número de bloco em tuplas[i].virtualBlockNumber
        if(readRegister(tuplas[i].virtualBlockNumber, &root) != TRUE) {
          return REGISTER_READ_ERROR;
        }
        free(tuplas);
        tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));

        parseRegister(root.at, tuplas);

        i = 0; // reset i para 0, começar a ler tuplas novamente
        break;
      case FIND_REGISTER_NOTFOUND:
        i++; // não estava na tupla atual, parte para a próxima tupla
        break;
      default:
        // fileRecord é o arquivo procurado.
        // i indica a tupla do diretório.
        // found indica o índice do arquivo dentro da tupla onde foi encontrado.

        if(j >= parseCount) { // Verifica se arquivo é o último procurado.
          endReached = TRUE;
        } else {
          // Não é o último, logo precisa descer mais um nível da árvore, indo para o registro indicado no arquivo encontrado.

          if(fileRecord->TypeVal == TYPEVAL_DIRETORIO) {
            // Se é um diretório, encontra o registro indicado pelo arquivo, e itera para o novo registro
            if(readRegister(fileRecord->MFTNumber, &root) != TRUE) {
              // printf("Erro lendo registro '%d'.\n", fileRecord->MFTNumber);
              return REGISTER_READ_ERROR;
            }
            free(tuplas);
            tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));
            parseRegister(root.at, tuplas);

            i = 0;
            j++;
          } else {
            // Se não é um arquivo de diretório, volta para a leitura do diretório atual
            found = FIND_REGISTER_NOTFOUND; // não encontrou o arquivo correto, mas um diretório de mesmo nome.
            i++;
          }
        }

        break;
    }
  }

  free(tuplas);
  tuplas = NULL;

  return found;
}

struct t2fs_record initRecord(BYTE typeVal, char* name, DWORD blocksFileSize, DWORD bytesFileSize, DWORD MFTNumber) {
  struct t2fs_record record;

  memset(&record, 0, sizeof(record));

  record.TypeVal = typeVal;
  strcpy(record.name, name); // name é o último elemento parseado: /directory/directory/file1 -> name = file1
  record.blocksFileSize = blocksFileSize;
  record.bytesFileSize = bytesFileSize;
  record.MFTNumber = MFTNumber;

  return record;
}

struct t2fs_4tupla initTupla(DWORD atributeType, DWORD VBN, DWORD LBN, DWORD numberOfContiguosBlocks) {
  struct t2fs_4tupla tupla;

  tupla.atributeType = atributeType;
  tupla.virtualBlockNumber = VBN;
  tupla.logicalBlockNumber = LBN;
  tupla.numberOfContiguosBlocks = numberOfContiguosBlocks;

  return tupla;
}

int initNewRegister(int registerIndex, int VBN, int LBN) {
  struct t2fs_4tupla tupla, tuplaFim;

  tupla     = initTupla(REGISTER_MAP, VBN, LBN, 1);
  tuplaFim  = initTupla(REGISTER_FIM, 0, 0, 0);

  REGISTER_T reg;
  memset(reg.at, 0, sizeof(reg.at));
  //readRegister(registerIndex, &reg);

  writeTupla(reg.at, &tupla,    0);
  writeTupla(reg.at, &tuplaFim, 1);

  writeRegister(registerIndex, &reg);

  return 1;
}

int initFileRegister(int registerIndex, int LBN) {
  return initNewRegister(registerIndex, 0, LBN);
}

DIRENT2 initDentry(struct t2fs_record record) {
  DIRENT2 dentry;

  strcpy(dentry.name, record.name);
  dentry.fileType = record.TypeVal;
  dentry.fileSize = record.bytesFileSize;

  return dentry;
}

int findOffsetTupla(struct t2fs_4tupla * tuplas, unsigned int initialBlock, REGISTER_T* reg) {
  unsigned int i = 0, foundStart = FALSE, amountOfBlocksRead = 0;

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
        if(readRegister(tuplas[i].virtualBlockNumber, reg) != TRUE) {
          return FALSE;
        }
        free(tuplas);
        tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));

        parseRegister(reg->at, tuplas);
        i = 0; // reset i para 0, começar a ler tuplas novamente

        break;
        case REGISTER_FIM:
        default:
          // Current pointer está depois do final do arquivo
          return 0;
          break;
    }
  }

  return i;
}
