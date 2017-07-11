/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Douglas Lazaro
  Francisco Knebel
*/

#ifndef __files2__
#define	__files2__
  #include "declarations.h"

  /* Create */
  /*-----------------------------------------------------------------------------
  Função: createRecord
    Criação de um novo descritor de arquivo, com alocação dos blocos de dados utilizados
    e criação do registro do arquivo no MFT.

  Entrada:
    pathname: path do arquivo.
    typeVal: Tipo do arquivo. Pode ser TYPEVAL_REGULAR ou TYPEVAL_DIRETORIO.

  Saida:
    Retorna o descritor de arquivo criado.

  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  struct t2fs_record createRecord(char* pathname, BYTE typeVal);

  /*-----------------------------------------------------------------------------
  Função: createNewFile
    Criação de um novo arquivo no disco, com todas as verificações e validações necessárias.
    Adiciona o arquivo criado na Lista de Arquivos Abertos (LDAA).

  Entrada:
    filename: path do arquivo.
    typeVal: Tipo do arquivo. Pode ser TYPEVAL_REGULAR ou TYPEVAL_DIRETORIO.

  Saida:
    Caso tudo ocorrer corretamente, retorna o handle do arquivo criado.
    Caso contrário, retorna um erro associado ao problema gerado, que é sempre < 0.

  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  int createNewFile(char * filename, BYTE typeVal);

  /*-----------------------------------------------------------------------------
  Função: addRecordToDirectory
    Adiciona um descritor de arquivo para um diretório.

  Entrada:
    struct t2fs_record record: descritor de arquivo que será adicionado.
    char * filename: nome do arquivo que será adicionado.
    int updatingRecord: indica se o descritor já existe no diretório.
      Se FALSE, usará o algoritmo first-fit e encaixará o descritor no primeiro local possível.
      Se TRUE, encontrará o record do arquivo dentro do diretório e atualizará seu valor.

  Saida:
    Caso tudo ocorrer corretamente, retorna o índice dentro do diretório onde foi adicionado.
    Caso contrário, retorna um erro associado ao problema gerado, que é sempre < 0.

  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  int addRecordToDirectory(struct t2fs_record record, char * filename, int updatingRecord);


  /* Delete */
  /*-----------------------------------------------------------------------------
  Função: deleteFileFromDisk
    Remove um arquivo do disco.
    Remove o seu descritor do seu diretório pai.
    Remove o registro associado ao arquivo.
    Invalida os blocos utilizados pelo arquivo.

  Entrada:
    struct t2fs_record file: descritor do arquivo que deverá ser removido do disco.
    char* filename: nome do arquivo que deverá ser removido do disco.

  Saida:
    Caso tudo ocorrer corretamente, retorna o índice dentro do diretório onde foi removido.
    Caso contrário, retorna um erro associado ao problema gerado, que é sempre < 0.

  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  int deleteFileFromDisk(struct t2fs_record file, char* filename);

  /* Open */
  /*-----------------------------------------------------------------------------
  Função: openFile
    Abre um arquivo no sistema.

  Entrada:
    char* filename: nome do arquivo que deverá ser aberto.

  Saida:
    Caso tudo ocorrer corretamente, retorna o handle do arquivo aberto.
    Caso contrário, retorna um erro associado ao problema gerado, que é sempre < 0.

  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  int openFile(char* filename);

  /* Close */
  /*-----------------------------------------------------------------------------
  Função: closeFile
    Fecha um arquivo, que estava aberto, no sistema.

  Entrada:
    int handle: handle do arquivo que deverá ser fechado.
    BYTE TypeVal: tipo do arquivo que deverá ser fechado.
      Pode ser TYPEVAL_REGULAR ou TYPEVAL_DIRETORIO.

  Saida:
    Caso tudo ocorrer corretamente, retorna 0.
    Caso contrário, retorna um erro associado ao problema gerado, que é sempre < 0.

  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  int closeFile(int handle, BYTE TypeVal);

  /* Read */
  /*-----------------------------------------------------------------------------
  Função: readFile
    Iterando sobre um arquivo, lê uma quantidade de bytes para um buffer.

  Entrada:
    int handle: handle do arquivo que deverá ser lido.
    struct descritor descritor: descritor, associado junto ao handle, do arquivo atual.
    char * buffer: endereço onde será armazenado os bytes lidos do arquivo.
    unsigned int size: quantidade em bytes que será lida do arquivo.

  Saida:
    Caso tudo ocorrer corretamente, retorna a quantidade de bytes lidos.
    Caso contrário, retorna um erro associado ao problema gerado, que é sempre < 0.

  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  int readFile(int handle, struct descritor descritor, char * buffer, unsigned int size);

  /* Write */
  /*-----------------------------------------------------------------------------
  Função: writeFile
    Iterando sobre um arquivo, escrendo uma quantidade de bytes no arquivo, lendo de um buffer.

  Entrada:
    int handle: handle do arquivo que deverá ser escrito.
    struct descritor descritor: descritor, associado junto ao handle, do arquivo atual.
    char * buffer: endereço onde será lido os bytes que serão escritos no arquivo.
    unsigned int size: quantidade em bytes que será escrita no arquivo.

  Saida:
    Caso tudo ocorrer corretamente, retorna a quantidade de bytes escritos.
    Caso contrário, retorna um erro associado ao problema gerado, que é sempre < 0.

  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  int writeFile(int handle, struct descritor descritor, char * buffer, unsigned int size);

  /* Truncate */
  /*-----------------------------------------------------------------------------
  Função: truncateFile
    Apaga todos os bytes do arquivo, posteriores ao currentPointer do arquivo.
    O arquivo final deverá ter tamanho igual ao currentPointer.

  Entrada:
    FILE2 handle: handle do arquivo que deverá ser truncado.
    struct descritor descritor: descritor, associado junto ao handle, do arquivo atual.

  Saida:
    Caso tudo ocorrer corretamente, retorna 0.
    Caso contrário, retorna um erro associado ao problema gerado, que é sempre < 0.

  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  int truncateFile(FILE2 handle, struct descritor descritor);
  
#endif
