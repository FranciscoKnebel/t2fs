/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/1

  Douglas Lazaro
  Francisco Knebel
*/

#ifndef __helperutil__
#define	__helperutil__

  /*-----------------------------------------------------------------------------
  Função: initConfig
    Inicialização do T2FS, para inicializar constantes e estruturas que serão
    utilizadas para execução do sistema. É chamado apenas na primeira chamada da API.

  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/
  int initConfig();


  /*-----------------------------------------------------------------------------
  Função: convertTwoBytes, convertFourBytes
    Recebe um buffer de char little endian, de qualquer tamanho,
    e retorna uma valor invertido desses bytes.

  Entra:
    unsignedchar* buffer: string do arquivo
    int index: offset do início do buffer, até onde a leitura será efetuada.
    char* str: onde será armazenado o valor convertido, em forma de string.

  Saída:
    Retorna o valor em forma de número inteiro.

  Responsável: Francisco Knebel
  -----------------------------------------------------------------------------*/

  int convertTwoBytes(unsigned char* buffer, int index, char* str);

  int convertFourBytes(unsigned char* buffer, int index, char* str);

#endif
