#ifndef __helpermft__
#define	__helpermft__

/*-----------------------------------------------------------------------------
Função: Usada para iniciar a indexMFT
	Essa função preenche o a indexMFT com o valor
	"1" se o está ocupado na área de MFT do disco
	OU
	"0" se o está livre na área de MFT do disco.

Entra: void

Saída: void

Responsável: Douglas Lázaro
-----------------------------------------------------------------------------*/
  void initMFT();

/*-----------------------------------------------------------------------------
Função: Usada para printar indexMFT
Entra: int begin -> índice inicial
	   int end -> índice final

Saída: print dos índices do indexMFT entre begin e end.

Responsável: Douglas Lázaro
-----------------------------------------------------------------------------*/
  void printMFT(int begin, int end);

/*------------------------------------------------------------------------
	Retorna a informação de alocação do índice MFT “registerIndex”,
		da área de bitmap.
Entra:
	registerIndex -> numero do índice MFT cujo bit deve ser retornado
Retorna:
	Sucesso: valor da alocacao: ZERO (livre) ou UM (alocado)
	Erro: número negativo

Responsável: Douglas Lázaro
------------------------------------------------------------------------*/
  int getMFT(int registerIndex);

/*------------------------------------------------------------------------
	Seta a informação de alocação do índice MFT “registerIndex”.
	Essa informação é passada no parâmetro “allocated”.
	Esse parâmetro pode ser:
Entra:
	registerIndex -> numero do índice MFT cujo bit deve ser setado
	allocated -> valor a ser escrito no bit
		==0 -> coloca bit em LIVRE (ZERO)
		!=0 -> coloca bit em OCUPADO (UM)
Retorna
	Sucesso: TRUE (1)
	Erro: número negativo

Responsável: Douglas Lázaro
------------------------------------------------------------------------*/
  int setMFT(int registerIndex, int allocated);

/*------------------------------------------------------------------------
	Procura, a partir do índice MFT “0” (zero), no vetor de índices do MFT,
	um índice MFT no estado indicado por “allocated”.
Entra:
	allocated -> valor procurado
		“0” (zero), se deve procurar por um índice MFT desalocado (LIVRE)
		outros valores, se deve procurar por um índice MFT alocado (ALOCADO)
Retorna
	Sucesso: número do índice MFT encontrado (número >= 0)
	Erro: número negativo

Responsável: Douglas Lázaro
------------------------------------------------------------------------*/
  int searchMFT(int allocated);

#endif
