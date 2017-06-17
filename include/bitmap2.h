
#ifndef __BITMAP2__
#define	__BITMAP2__


/*------------------------------------------------------------------------
	Retorna a informa��o de aloca��o do bloco �blockNumber�,
		da �rea de bitmap.
Entra:
	blockNumber -> numero do bloco cujo bit deve ser retornado
Retorna:
	Sucesso: valor da alocacao: ZERO (livre) ou UM (alocado)
	Erro: n�mero negativo
------------------------------------------------------------------------*/
int	getBitmap2 (int blockNumber);

/*------------------------------------------------------------------------
	Seta a informa��o de aloca��o do bloco �blockNumber�.
	Essa informa��o � passada no par�metro �allocated�. 
	Esse par�metro pode ser:
Entra:
	blockNumber -> numero do bloco cujo bit deve ser setado
	allocated -> valor a ser escrito no bit
		==0 -> coloca bit em LIVRE (ZERO)
		!=0 -> coloca bit em OCUPADO (UM)
Retorna
	Sucesso: ZERO (0)
	Erro: n�mero negativo
------------------------------------------------------------------------*/
int	setBitmap2 (int blockNumber, int allocated);

/*------------------------------------------------------------------------
	Procura, a partir do bloco �0� (zero), na �rea de bitmap, 
	um bloco no estado indicado por �allocated�.
Entra:
	allocated -> valor procurado
		�0� (zero), se deve procurar por um bloco desalocado (LIVRE)
		outros valores, se deve procurar por um bloco alocado (ALOCADO)
Retorna
	Sucesso
		Achou um bloco: numero do bloco encontrado (n�mero positivo)
		N�o achou: ZERO
	Erro: n�mero negativo	
------------------------------------------------------------------------*/
int	searchBitmap2 (int allocated);

#endif
