/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Douglas Lazaro
  Francisco Knebel
*/

#ifndef __parse__
#define	__parse__
  #include "declarations.h"

  /*-----------------------------------------------------------------------------
  Função: parseBootBlock
    Parsing do bloco de boot para preenchimento da estrutura BootBlock.

  Entrada:
    Buffer com valor lido diretamente do disco, no bloco de boot.

  Saida:
    Retorna estrutura convertida do bloco de boot.

  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  struct BootBlock parseBootBlock(unsigned char* bootBlock);

  /*-----------------------------------------------------------------------------
  Função: parseRegister
    Parsing de um buffer de registro e conversão para lista de t2fs_4tuplas.

  Entrada:
    buffer: valor lido diretamente do registro.
    tuplas: endereço de memória onde será armazenado a lista de MAX_TUPLAS_REGISTER-1 tuplas.

  Saida:
    Retorna o registro convertido em múltiplas t2fs_4tupla.

  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  int parseRegister(unsigned char* buffer, struct t2fs_4tupla * tuplas);

  /*-----------------------------------------------------------------------------
  Função: parseRecord
    Parsing de um buffer de bloco e conversão para um t2fs_record.

  Entrada:
    buffer: valor lido diretamente de um bloco, do bloco de dados.
    record: endereço onde será armazenado o descritor t2fs_record do arquivo requisitado.
    offset: offset do endereço inicial do buffer até o descritor requisitado.

  Saida:
    Retorna o descritor t2fs_record do arquivo.

  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  int parseRecord(BLOCK_T blockBuffer, struct t2fs_record * record, int offset);

  /*-----------------------------------------------------------------------------
  Função: parseDirectory
    Parsing de um buffer de bloco e conversão para lista de t2fs_records.

  Entrada:
    buffer: valor lido diretamente de um bloco, do bloco de dados.
    records: endereço onde será armazenado a lista de descritores t2fs_record dos arquivos.

  Saida:
    Retorna o bloco convertido em múltiplos t2fs_record.

  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  int parseDirectory(BLOCK_T block, struct t2fs_record* records);

#endif
