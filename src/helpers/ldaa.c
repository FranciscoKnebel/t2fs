/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Módulo desenvolvido por Douglas Lázaro
  Funções:
    initLDAA, getFreeLDAA, insertLDAA, removeLDAA, searchLDAA

*/

#include "libs.h"


struct descritor resetDescritor() {
  struct descritor temp;

  temp.flag = 0;
  temp.currentPointer = -1;

  temp.record.TypeVal = 0;
  strcpy(temp.record.name, "");
  temp.record.blocksFileSize = 0;
  temp.record.bytesFileSize = 0;
  temp.record.MFTNumber = 0;

  return temp;
}

void initLDAA(struct descritor* lista){
  int i;

  for (i = 0; i < MAX_FILES_OPEN; ++i)
  {
    lista[i] = resetDescritor();
  }

}

int getFreeLDAA(struct descritor* lista){
  int i;

  for (i = 0; i < MAX_FILES_OPEN; ++i){
    if (lista[i].flag == 0)
    {
      return i;
    }
  }
  return -1;
}



int insertLDAA(struct descritor* lista, struct t2fs_record record){

    int i = getFreeLDAA(lista);

    if(i < 0)
      return FALSE;
    else
    {
      lista[i].flag = 1;
      lista[i].currentPointer = 0;

      memcpy(&lista[i].record, &record, RECORD_SIZE);
    }

    return TRUE;
}

int removeLDAA(struct descritor* lista, int handle) {
  lista[handle] = resetDescritor();

  return lista[handle].flag == 0;
}

int searchLDAA(struct descritor* lista, int handle, int type){
  return lista[handle].flag == 1 && lista[handle].record.TypeVal == type;
}
