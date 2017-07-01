/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Módulo desenvolvido por Douglas Lázaro
  Funções:
    initLDAA, getFreeLDAA, insertLDAA, removeLDAA, searchLDAA

*/

#include "libs.h"

void initLDAA(struct descritor* lista){
  int i;

  for (i = 0; i < MAX_FILE_OPEN; ++i)
  {
    lista[i].flag = 0;
    lista[i].currentPointer = -1;

    lista[i].record.TypeVal = 0;
    strcpy(lista[i].record.name, "");
    lista[i].record.blocksFileSize = 0;
    lista[i].record.bytesFileSize = 0;
    lista[i].record.MFTNumber = 0;
    
  }

}

int getFreeLDAA(struct descritor* lista){
  int i;
  
  for (i = 0; i < MAX_FILE_OPEN; ++i){
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

      lista[i].record.TypeVal = record.TypeVal;
      strcpy(lista[i].record.name, record.name);
      lista[i].record.blocksFileSize = record.blocksFileSize;
      lista[i].record.bytesFileSize = record.bytesFileSize;
      lista[i].record.MFTNumber = record.MFTNumber;
    }

    return TRUE;
}

int removeLDAA(struct descritor* lista, int handle) {
  lista[handle].flag = 0;
  lista[handle].currentPointer = -1;

  lista[handle].record.TypeVal = 0;
  strcpy(lista[handle].record.name, "");
  lista[handle].record.blocksFileSize = 0;
  lista[handle].record.bytesFileSize = 0;
  lista[handle].record.MFTNumber = 0;
  
  if (lista[handle].flag == 0)
    return TRUE;
  else
    return FALSE;
}

int searchLDAA(struct descritor* lista, int handle, int type){
  if(lista[handle].flag == 1 && lista[handle].record.TypeVal == type)
    return TRUE;
  else
    return FALSE;
}