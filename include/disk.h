#ifndef __disk__
#define	__disk__
  #include "declarations.h"

  /*-----------------------------------------------------------------------------
  Função: readSector
    Leitura de um setor e armazenamento em um buffer.

  Entrada:
    sector: número do setor que será lido.
    buffer: endereço onde o buffer do setor lido será armazenado.

  Saida:
    Retorna TRUE se o setor foi lido corretamente.
    Retorna FALSE se houve um problema de leitura ou se o número de setor é inválido.

  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  int readSector(int sector, SECTOR_T* buffer);

  /*-----------------------------------------------------------------------------
  Função: readBlock
    Leitura de um bloco e armazenamento em um buffer.

  Entrada:
    block: número do block que será lido.
    buffer: endereço onde o buffer do block lido será armazenado.

  Saida:
    Retorna TRUE se o bloco foi lido corretamente.
    Retorna FALSE se houve um problema de leitura ou se o número do bloco é inválido.

  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  int readBlock(int block, BLOCK_T* buffer);

  /*-----------------------------------------------------------------------------
  Função: readBootBlock
    Leitura especial do bloco de boot. É a primeira leitura efetuada do T2FS,
    e será usado para definir os tamanhos dos elementos do disco, utilizados
    em outros métodos.

  Entrada:
    buffer: endereço onde o buffer do bloco de boot será armazenado.

  Saida:
    Retorna TRUE se o bloco foi lido corretamente.
    Retorna FALSE se houve um problema de leitura.

  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  int readBootBlock(SECTOR_T* buffer);

  /*-----------------------------------------------------------------------------
  Função: readRegister
    Leitura de um registro do MFT.

  Entrada:
    registerIndex: indice, de 0 até MAX_REGISTERS, do registro procurado.
    reg: endereço onde o buffer do registro será armazenado.

  Saida:
    Retorna TRUE se o registro foi lido corretamente.
    Retorna FALSE se houve um problema de leitura.
    Retorna -1 se registerIndex é inválido.

  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  int readRegister(int registerIndex, REGISTER_T* reg);

  /*-----------------------------------------------------------------------------
  Função: readRecord
    Leitura de um descritor de arquivo.

  Entrada:
    block:

  Saida:


  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  int readRecord(int block, int index, struct t2fs_record * record);

  int writeSector(int sector, SECTOR_T* buffer);

  int writeBlock(int block, BLOCK_T* buffer);

  int writeRegister(int registerIndex, REGISTER_T* reg);

  int writeRecord(int block, int index, struct t2fs_record record);

  void* writeTupla(unsigned char* buffer, struct t2fs_4tupla* tupla, int index);

#endif
