
#ifndef __BITMAP2__
#define	__BITMAP2__


/*------------------------------------------------------------------------
	Retorna a informação de alocação do bloco “blockNumber”,
		da área de bitmap.
Entra:
	blockNumber -> numero do bloco cujo bit deve ser retornado
Retorna:
	Sucesso: valor da alocacao: ZERO (livre) ou UM (alocado)
	Erro: número negativo
------------------------------------------------------------------------*/
int	getBitmap2 (int blockNumber);

/*------------------------------------------------------------------------
	Seta a informação de alocação do bloco “blockNumber”.
	Essa informação é passada no parâmetro “allocated”. 
	Esse parâmetro pode ser:
Entra:
	blockNumber -> numero do bloco cujo bit deve ser setado
	allocated -> valor a ser escrito no bit
		==0 -> coloca bit em LIVRE (ZERO)
		!=0 -> coloca bit em OCUPADO (UM)
Retorna
	Sucesso: ZERO (0)
	Erro: número negativo
------------------------------------------------------------------------*/
int	setBitmap2 (int blockNumber, int allocated);

/*------------------------------------------------------------------------
	Procura, a partir do bloco “0” (zero), na área de bitmap, 
	um bloco no estado indicado por “allocated”.
Entra:
	allocated -> valor procurado
		“0” (zero), se deve procurar por um bloco desalocado (LIVRE)
		outros valores, se deve procurar por um bloco alocado (ALOCADO)
Retorna
	Sucesso
		Achou um bloco: numero do bloco encontrado (número positivo)
		Não achou: ZERO
	Erro: número negativo	
------------------------------------------------------------------------*/
int	searchBitmap2 (int allocated);

#endif
