/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Módulo desenvolvido por Francisco Knebel
  Funções:
    initTupla, initFileRegister, createRecord, addRecord
*/

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

  memset(&newFile, 0, sizeof(newFile));

  newFile.TypeVal = TYPEVAL_REGULAR;
  strcpy(newFile.name, parsedPath[parseCount]); //name é o último elemento parseado: /directory/directory/file1 -> name = file1
  newFile.blocksFileSize = 1;
  newFile.bytesFileSize = 0;

  /* Encontrar registro do MFT livre, escrever registro e associar ao arquivo. */
  int fileVBN = searchBitmap2(BM_LIVRE); // Encontra bloco de dados para o arquivo
  setBitmap2(fileVBN, BM_OCUPADO);

  int registro = searchMFT(MFT_BM_LIVRE);
  initFileRegister(registro, fileVBN); // Inicializa registro no MFT do arquivo
  setMFT(registro, MFT_BM_OCUPADO);    // seta registro como ocupado no bitmap.

  newFile.MFTNumber = registro; // Associa arquivo ao registro criado

  return newFile;
}

int addToDirectory(DWORD directoryMFTNumber, struct t2fs_record record) {
  int return_value;

  REGISTER_T reg;
  if(readRegister(directoryMFTNumber, &reg) != TRUE) {
    return FALSE;
  }

  struct t2fs_4tupla *tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));
  parseRegister(reg.at, tuplas);

  struct t2fs_record records[constants.RECORD_PER_BLOCK];
  BLOCK_T blockBuffer;
  blockBuffer.at = malloc(sizeof(unsigned char) * constants.BLOCK_SIZE);
  int i = 0, foundSpaceToAdd = FALSE;
  while (i < constants.MAX_TUPLAS_REGISTER && foundSpaceToAdd != TRUE) {
    switch (tuplas[i].atributeType) {
      case REGISTER_MAP:
        if(readBlock(tuplas[i].logicalBlockNumber, &blockBuffer) == FALSE) {
          return FALSE;
        };

        parseDirectory(blockBuffer, records);

        int j;
        for (j = 0; j < constants.RECORD_PER_BLOCK && foundSpaceToAdd != TRUE; j++) {
          if(records[j].TypeVal != TYPEVAL_REGULAR && records[j].TypeVal != TYPEVAL_DIRETORIO) {
            // ADICIONAR RECORD PARA O DIRETÓRIO
            if(writeRecord(tuplas[i].logicalBlockNumber, j, record) == FALSE) {
              return RECORD_WRITE_ERROR;
            };

            foundSpaceToAdd = TRUE;
            return_value = i;
          }
        }

        if(foundSpaceToAdd != TRUE) {
          i++;
        }

        break;
      case REGISTER_FIM:
        // CHEGOU AO FIM, CRIAR NOVO BLOCO
        // VERIFICAR SE POSSÌVEL ADICIONAR MAIS UM BLOCO CONTIGUO PARA A TUPLA
        // SE SIM,
        /*
          numberOfContiguosBlocks +1 e atualizar valor da tupla no disco
          setBitmap2 para o novo bloco ocupado
          adicionar record para o novo bloco
        */
        // SE NÂO,
        /*
          Verificar se i+1 == máximo de tuplas (significa que a tupla fica no final do registro)
          Se sim,
            tupla[i] vira REGISTER_ADITIONAL
            encontrar registro livre com o Bitmap MFT.
            passar registro para tupla[i]
            setar registro como ocupado.

            criar novo registro, verificando blocos livres e setando ocupação.
            no novo bloco da primeira nova tupla, adicionar o record.
          Se não
            setar tupla[i+1] como REGISTER_FIM
            tupla[i] vira REGISTER_MAP
            encontra bloco de dados livre e seta como ocupado.
            adiciona o record no novo bloco
        */
        break;
      case REGISTER_ADITIONAL: // LER NOVO REGISTRO E RECOMEÇAR LEITURA
        if(readRegister(tuplas[i].virtualBlockNumber, &reg) != TRUE) {
          return FALSE;
        }
        free(tuplas);
        tuplas = malloc(constants.MAX_TUPLAS_REGISTER * sizeof(struct t2fs_4tupla));

        parseRegister(reg.at, tuplas);
        i = 0; // reset i para 0, começar a ler tuplas novamente

        break;
      case REGISTER_FREE:
      default:
        return_value = FIND_REGISTER_FREE;
        i++;
        break;
    }
  }

  return return_value;
}

int addRecordToDirectory(struct t2fs_record record, char * filename) {
  struct t2fs_record directory;
  char * directoryname = malloc(strlen(filename));
  int return_value = FALSE;

  getFileDirectory(filename, directoryname);

  if(strcmp("/", directoryname) == 0) { // adicionar para a root
    return_value = addToDirectory(REGISTER_ROOT, record);

    return return_value;
  }

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
      return_value = addToDirectory(directory.MFTNumber, record);

      break;
  }

  return return_value;
}
