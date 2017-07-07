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
  strcpy(temp.name, "");

  temp.record.TypeVal = 0;
  strcpy(temp.record.name, "");
  temp.record.blocksFileSize = 0;
  temp.record.bytesFileSize = 0;
  temp.record.MFTNumber = 0;

  return temp;
}

void initLDAA(){
  int i;

  for (i = 0; i < MAX_FILES_OPEN; ++i)
  {
    config.LDAA[i] = resetDescritor();
  }

}

int getFreeLDAA(){
  int i;

  for (i = 0; i < MAX_FILES_OPEN; ++i){
    if (config.LDAA[i].flag == 0)
    {
      return i;
    }
  }
  return -1;
}

int isFreeLDAA(){
  if(getFreeLDAA() != -1)
    return TRUE;
  else
    return FALSE;
}

int insertLDAA(struct t2fs_record record, char* pathname){

    int i = getFreeLDAA();

    if(i < 0)
      return -1;

    config.LDAA[i].flag = 1;
    config.LDAA[i].currentPointer = 0;
    strcpy(config.LDAA[i].name, pathname);

    memcpy(&config.LDAA[i].record, &record, RECORD_SIZE);

    return i;
}

int removeLDAA(int handle) {
  config.LDAA[handle] = resetDescritor();

  return config.LDAA[handle].flag == 0;
}

int searchLDAA(int handle, int type, struct descritor* descritor){
  if (config.LDAA[handle].flag == 1 && config.LDAA[handle].record.TypeVal == type){
    *descritor = config.LDAA[handle];
    return TRUE;
  }
  else
    return FALSE;
}
int updateLDAA(int handle, int type, struct descritor descritor){
  struct descritor temp;

  if (searchLDAA(handle, type, &temp) == TRUE)
  {
    memcpy(&config.LDAA[handle], &descritor, sizeof(struct descritor));
    return 0;
  }
  else
    return -1;
}

int findByNameLDAA(char * name) {
  int i, return_value = -1;

  for (i = 0; i < MAX_FILES_OPEN; i++) {
    if (config.LDAA[i].flag == 1 && strcmp(config.LDAA[i].name, name) == 0){
      return_value = i;
      break;
    }
  }

  return return_value;
}

int canAddToLDAA(char * name) {
  int hasSpace = isFreeLDAA() == TRUE;
  int fileIsNotOpen = findByNameLDAA(name) < 0;

  return hasSpace && fileIsNotOpen;
}
