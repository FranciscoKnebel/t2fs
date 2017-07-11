/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Douglas Lazaro
  Francisco Knebel
*/

#ifndef __helperfiles__
#define	__helperfiles__
  #include "declarations.h"

  /*-----------------------------------------------------------------------------
  Função: validPath
    Recebe uma string e verifica se a mesma é uma path válida.

  Entra:
    char* path: string que deverá ser validada

  Saída:
    TRUE se tudo ok.
    FALSE se há algum caractere ou regra inválida.

  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  int validPath(char* path);

  /*-----------------------------------------------------------------------------
  Função: getFileDirectory
    Recebe uma path de um arquivo e retorna a path para o seu diretório pai.

  Entra:
    char* path: string do arquivo
    char* directory: endereço onde a string do diretório será armazenada.

  Saída: Retorna TRUE se tudo ok.

  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  int getFileDirectory(char* path, char* directory);

  /*-----------------------------------------------------------------------------
  Função: parsePath
    Recebe uma string qualquer e retorna os elementos dessa path, até o arquivo final.

  Entra:
    char* path: string que deverá ser separada
    char** elements: endereço onde as strings dos elementos serão armazenados.

  Saída:
    Se tudo ocorreu corretamente, retorna a quantidade de elementos
    Caso a path seja inválida, retorna FALSE.

  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  int parsePath(char* path, char ** elements);

  /*-----------------------------------------------------------------------------
  Função: lookup
    Função essencial para a busca de descritores de arquivos.
    Partindo dessa função que as demais são chamadas.
    Recebe uma path e retorna o descritor de arquivo associado.

  Entra:
    char* pathname: path do arquivo procurado.
    struct t2fs_record* fileRecord: endereço onde será armazenado o descritor do arquivo.

  Saída:
    Se tudo ocorreu corretamente, retorna o índice dentro do diretório onde o descritor está armazenado,
    e em fileRecord está presente o descritor procurado.

    Caso ocorreu algum erro de leitura, retorna um número negativo, associado ao tipo de erro.
    Os tipos de erro estão todos no arquivo de header constants.h

  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  int lookup(char* pathname, struct t2fs_record * fileRecord);

  /*-----------------------------------------------------------------------------
  Funções: initRecord, initTupla, initDentry, initNewRegister, initFileRegister
    Inicializações dessas estruturas, recebendo os elementos necessários para criação.
    Favor olhar a implementação de cada função para verificar o funcionamento.

  Entra:
    Elementos necessários para criação de cada elemento.

  Saída:
    No caso do initNewRegister e initFileRegister, o retorno é desprezível.
    Nos demais casos, retorna a estrutura preenchida.

  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  struct t2fs_record initRecord(BYTE typeVal, char* name, DWORD blocksFileSize, DWORD bytesFileSize, DWORD MFTNumber);

  struct t2fs_4tupla initTupla(DWORD atributeType, DWORD VBN, DWORD LBN, DWORD numberOfContiguosBlocks);

  int initNewRegister(int registerIndex, int VBN, int LBN);

  int initFileRegister(int registerIndex, int LBN);

  DIRENT2 initDentry(struct t2fs_record record);

  /*-----------------------------------------------------------------------------
  Funções: findOffsetTupla
    Encontra em qual tupla deverá ser inicializado as operações, de acordo com um bloco inicial.

  Entra:
    struct t2fs_4tupla * tuplas: tuplas que devem ser iteradas.
    unsigned int initialBlock: bloco in
    REGISTER_T* reg: buffer do registro atual. Utilizado no caso de REGISTER_ADITIONAL

  Saída:
    Retorna o índice procurado.

  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  int findOffsetTupla(struct t2fs_4tupla * tuplas, unsigned int initialBlock, REGISTER_T* reg);

#endif
