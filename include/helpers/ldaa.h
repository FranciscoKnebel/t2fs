#ifndef __ldaa__
#define	__ldaa__
  #include "declarations.h"

/* Estrutura do decritor de arquivos abertos */
  struct descritor {
  	int flag;
  	DWORD currentPointer;
  	struct t2fs_record record;
  };

/*-----------------------------------------------------------------------------
Função: Usada para iniciar a LDAA
	Essa função preenche o a LDAA com decritores vazios.

Entra: void

Saída: void

Responsável: Douglas Lázaro
-----------------------------------------------------------------------------*/
  void initLDAA();

/*-----------------------------------------------------------------------------
Função: Usada para testar se a LDAA tem espaço livre

Entra: void

Saída: Sucesso -> TRUE
	   Erro -> FALSE

Responsável: Douglas Lázaro	   
-----------------------------------------------------------------------------*/
  int getFreeLDAA(); //test_ldaa
  int isFreeLDAA();

/*-----------------------------------------------------------------------------
Função: Usada para inserir descritor na LDAA
	Essa função insere na LDAA um descritor com o valor do t2fs_record passado como parametro.
Entra: t2fs_record record -> registro de arquivo ou diretório aberto

Saída: Sucesso -> retorna o inteiro correspondente ao índice da LDAA onde foi inserido o record (HANDLE)
	   Erro -> valor negativo

Responsável: Douglas Lázaro	   
-----------------------------------------------------------------------------*/
  int insertLDAA(struct t2fs_record record);

/*-----------------------------------------------------------------------------
Função: Usada para remover descritor na LDAA
	Essa função faz um reset no descritor de índice igual ao handle passado como parâmetro.
Entra: int handle -> índice do arquivo na LDAA

Saída: Sucesso -> TRUE
	   Erro -> FALSE

Responsável: Douglas Lázaro	   
-----------------------------------------------------------------------------*/
  int removeLDAA(int handle);

/*-----------------------------------------------------------------------------
Função: Usada para pesquisar na LDAA
	Essa função pesquisa na LDAA se determinado descritor se encontra na lista.

Entra:  int Handle -> índice do arquivo na LDAA
		int Type -> tipo do arquivo (1 = file | 2 = diretory)

Saída:  Sucesso -> TRUE e struct descritor
		Erro -> valor negativo 

Responsável: Douglas Lázaro		
-----------------------------------------------------------------------------*/
  int searchLDAA(int handle, int type, struct descritor* descritor);

#endif
