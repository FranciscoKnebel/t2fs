
/**

    T2 shell, para teste do T2FS - Sistema de arquivos do trabalho 2 de Sistemas Operacionais I

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "t2fs.h"


void cmdMan(void);

void cmdWho(void);
void cmdLs(void);
void cmdMkdir(void);
void cmdRmdir(void);

void cmdOpen(void);
void cmdRead(void);
void cmdClose(void);

void cmdWrite(void);
void cmdCreate(void);
void cmdDelete(void);
void cmdSeek(void);
void cmdTrunc(void);

void cmdCp(void);
void cmdFscp(void);

void cmdExit(void);

static void dump(char *buffer, int size) {
    int base, i;
    char c;
    for (base=0; base<size; base+=16) {
        printf ("%04d ", base);
        for (i=0; i<16; ++i) {
            if (base+i<size) printf ("%02X ", buffer[base+i]);
            else printf ("   ");
        }

        printf (" *");

        for (i=0; i<16; ++i) {
            if (base+i<size) c = buffer[base+i];
            else c = ' ';

            if (c<' ' || c>'z' ) c = ' ';
            printf ("%c", c );
        }
        printf ("*\n");
    }
}

#define	CMD_EXIT	0
#define	CMD_MAN		1
#define	CMD_WHO		2
#define	CMD_DIR		3
#define	CMD_MKDIR	4
#define	CMD_RMDIR	5
#define	CMD_OPEN	6
#define	CMD_READ	7
#define	CMD_CLOSE	8
#define	CMD_WRITE	9
#define	CMD_CREATE	10
#define	CMD_DELETE	11
#define	CMD_SEEK	12
#define	CMD_TRUNCATE	13
#define	CMD_COPY	14
#define	CMD_FS_COPY	15

char helpString[][120] = {
	"            -> finish this shell",
	"[comando]   -> command help",
	"            -> shows T2FS authors",
	"[pahname]   -> list files in [pathname]",
	"[dirname]   -> create [dirname] in T2FS",
	"[dirname]   -> deletes [dirname] from T2FS",
	"[file]      -> open [file] from T2FS",
	"[hdl] [siz] -> read [siz] bytes from file [hdl]",
	"[hdl        -> close [hdl]",
	"[hdl] [str] -> write [str] bytes to file [hdl]",
	"[file]      -> create new [file] in T2FS",
	"[file]      -> deletes [file] from T2FS",
	"[hdl] [pos] -> set CP of [hdl] file on [pos]",
	"[hdl] [siz] -> truncate file [hdl] to [siz] bytes",
	"[src] [dst] -> copy files: [src] -> [dst]",
	"\n    fscp -t [src] [dst]     -> copy HostFS to T2FS"
	"\n    fscp -f [src] [dst]     -> copy T2FS   to HostFS"
};


struct {
	char name[20];
	void (*f)(void);
	int helpId;
} cmdList[] = {
	{ "exit", cmdExit, CMD_EXIT }, { "x", cmdExit, CMD_EXIT }, { "nl", cmdExit, CMD_EXIT },
	{ "man", cmdMan, CMD_MAN }, { "nl", cmdExit, CMD_EXIT },
	{ "who", cmdWho, CMD_WHO }, { "id", cmdWho, CMD_WHO }, { "nl", cmdExit, CMD_EXIT },
	{ "dir", cmdLs, CMD_DIR }, { "ls", cmdLs, CMD_DIR }, { "nl", cmdExit, CMD_EXIT },
	{ "mkdir", cmdMkdir, CMD_MKDIR }, { "md", cmdMkdir, CMD_MKDIR }, { "nl", cmdExit, CMD_EXIT },
	{ "rmdir", cmdRmdir, CMD_RMDIR }, { "rm", cmdRmdir, CMD_RMDIR }, { "nl", cmdExit, CMD_EXIT },

	{ "open", cmdOpen, CMD_OPEN }, { "nl", cmdExit, CMD_EXIT },
	{ "read", cmdRead, CMD_READ }, { "rd", cmdRead, CMD_READ }, { "nl", cmdExit, CMD_EXIT },
	{ "close", cmdClose, CMD_CLOSE }, { "cl", cmdClose, CMD_CLOSE }, { "nl", cmdExit, CMD_EXIT },
	{ "write", cmdWrite, CMD_WRITE }, { "wr", cmdWrite, CMD_WRITE }, { "nl", cmdExit, CMD_EXIT },
	{ "create", cmdCreate, CMD_CREATE }, { "cr", cmdCreate, CMD_CREATE }, { "nl", cmdExit, CMD_EXIT },
	{ "delete", cmdDelete, CMD_DELETE }, { "del", cmdDelete, CMD_DELETE }, { "nl", cmdExit, CMD_EXIT },
	{ "seek", cmdSeek, CMD_SEEK }, { "sk", cmdSeek, CMD_SEEK }, { "nl", cmdExit, CMD_EXIT },
	{ "truncate", cmdTrunc, CMD_TRUNCATE }, { "trunc", cmdTrunc, CMD_TRUNCATE }, { "tk", cmdTrunc, CMD_TRUNCATE }, { "nl", cmdExit, CMD_EXIT },

	{ "cp", cmdCp, CMD_COPY }, { "nl", cmdExit, CMD_EXIT },
	{ "fscp", cmdFscp, CMD_FS_COPY }, { "nl", cmdExit, CMD_EXIT },
	{ "fim", NULL, -1 }
};



int main()
{
    char cmd[256];
    char *token;
    int i;
    int flagAchou, flagEncerrar;

    //token = strtok("who"," \t");
    strcpy(cmd, "man");
    token = strtok(cmd," \t");
    cmdMan();

    flagEncerrar = 0;
    while (1) {
        printf ("T2FS> ");
        gets(cmd);
        if( (token = strtok(cmd," \t")) != NULL ) {
		flagAchou = 0;
		for (i=0; strcmp(cmdList[i].name,"fim")!=0; i++) {
			if (strcmp(cmdList[i].name, token)==0) {
				flagAchou = 1;
				cmdList[i].f();
				if (cmdList[i].helpId==CMD_EXIT) {
					flagEncerrar = 1;
					break;
				}
			}
		}
		if (!flagAchou) printf ("???\n");
        }
	if (flagEncerrar) break;
    }
    return 0;
}

/**
Encerra a opera��o do teste
*/
void cmdExit(void) {
    printf ("bye, bye!\n");
}

/**
Informa os comandos aceitos pelo programa de teste
*/
void cmdMan(void) {
	int i;
	char *token = strtok(NULL," \t");
	int flagVirgula;

	printf ("Testing program for T2FS - v 2017.1.0\n");

	if (token==NULL) {
		flagVirgula = 0;
		for (i=0; strcmp(cmdList[i].name,"fim")!=0; i++) {
			if (strcmp(cmdList[i].name,"nl")==0) {
				printf ("\n");
				flagVirgula = 0;
				continue;
			}
			if (flagVirgula)
				printf (", ");
			printf ("%s", cmdList[i].name);
			flagVirgula = 1;
		}
		printf ("\n");
		return;
	}

	for (i=0; strcmp(cmdList[i].name,"fim")!=0; i++) {
		if (strcmp(cmdList[i].name,token)==0) {
			printf ("%-10s %s\n", cmdList[i].name, helpString[cmdList[i].helpId]);
		}
	}


}


/**
Chama da fun��o identify2 da biblioteca e coloca o string de retorno na tela
*/
void cmdWho(void) {
    char name[256];
    int err = identify2(name, 256);
    if (err) {
        printf ("Erro: %d\n", err);
        return;
    }
    printf ("%s\n", name);
}

/**
Copia arquivo dentro do T2FS
Os parametros s�o:
    primeiro parametro => arquivo origem
    segundo parametro  => arquivo destino
*/
void cmdCp(void) {

    // Pega os nomes dos arquivos origem e destion
    char *src = strtok(NULL," \t");
    char *dst = strtok(NULL," \t");
    if (src==NULL || dst==NULL) {
        printf ("Missing parameter\n");
        return;
    }
    // Abre o arquivo origem, que deve existir
    FILE2 hSrc = open2 (src);
    if (hSrc<0) {
        printf ("Open source file error: %d\n", hSrc);
        return;
    }
    // Cria o arquivo de destino, que ser� resetado se existir
    FILE2 hDst = create2 (dst);
    if (hDst<0) {
        close2(hSrc);
        printf ("Create destination file error: %d\n", hDst);
        return;
    }
    // Copia os dados de source para destination
    char buffer[2];
    while( read2(hSrc, buffer, 1) == 1 ) {
        write2(hDst, buffer, 1);
    }
    // Fecha os arquicos
    close2(hSrc);
    close2(hDst);

    printf ("Files successfully copied\n");
}

/**
Copia arquivo de um sistema de arquivos para o outro
Os parametros s�o:
    primeiro parametro => dire��o da copia
        -t copiar para o T2FS
        -f copiar para o FS do host
    segundo parametro => arquivo origem
    terceiro parametro  => arquivo destino
*/
void cmdFscp(void) {
    // Pega a dire��o e os nomes dos arquivos origem e destion
    char *direcao = strtok(NULL, " \t");
    char *src = strtok(NULL," \t");
    char *dst = strtok(NULL," \t");
    if (direcao==NULL || src==NULL || dst==NULL) {
        printf ("Missing parameter\n");
        return;
    }
    // Valida dire��o
    if (strncmp(direcao, "-t", 2)==0) {
        // src == host
        // dst == T2FS

        // Abre o arquivo origem, que deve existir
        FILE *hSrc = fopen(src, "r+");
        if (hSrc==NULL) {
            printf ("Open source file error\n");
            return;
        }
        // Cria o arquivo de destino, que ser� resetado se existir
        FILE2 hDst = create2 (dst);
        if (hDst<0) {
            fclose(hSrc);
            printf ("Create destination file error: %d\n", hDst);
            return;
        }
        // Copia os dados de source para destination
        char buffer[2];
        while( fread((void *)buffer, (size_t)1, (size_t)1, hSrc) == 1 ) {
            write2(hDst, buffer, 1);
        }
        // Fecha os arquicos
        fclose(hSrc);
        close2(hDst);
    }
    else if (strncmp(direcao, "-f", 2)==0) {
        // src == T2FS
        // dst == host

        // Abre o arquivo origem, que deve existir
        FILE2 hSrc = open2 (src);
        if (hSrc<0) {
            printf ("Open source file error: %d\n", hSrc);
            return;
        }
        // Cria o arquivo de destino, que ser� resetado se existir
        FILE *hDst = fopen(dst, "w+");
        if (hDst==NULL) {
            printf ("Open destination file error\n");
            return;
        }
        // Copia os dados de source para destination
        char buffer[2];
        while ( read2(hSrc, buffer, 1) == 1 ) {
            fwrite((void *)buffer, (size_t)1, (size_t)1, hDst);
        }
        // Fecha os arquicos
        close2(hSrc);
        fclose(hDst);
    }
    else {
        printf ("Invalid copy direction\n");
        return;
    }

    printf ("Files successfully copied\n");
}

/**
Cria o arquivo informado no parametro
Retorna eventual sinaliza��o de erro
Retorna o HANDLE do arquivo criado
*/
void cmdCreate(void) {
    FILE2 hFile;

    char *token = strtok(NULL," \t");
    if (token==NULL) {
        printf ("Missing parameter\n");
        return;
    }

    hFile = create2 (token);
    if (hFile<0) {
        printf ("Error: %d\n", hFile);
        return;
    }

    printf ("File created with handle %d\n", hFile);
}

/**
Apaga o arquivo informado no parametro
Retorna eventual sinaliza��o de erro
*/
void cmdDelete(void) {

    char *token = strtok(NULL," \t");
    if (token==NULL) {
        printf ("Missing parameter\n");
        return;
    }

    int err = delete2(token);
    if (err<0) {
        printf ("Error: %d\n", err);
        return;
    }

    printf ("File %s was deleted\n", token);
}

/**
Abre o arquivo informado no parametro [0]
Retorna sinaliza��o de erro
Retorna HANDLE do arquivo retornado
*/
void cmdOpen(void) {
    FILE2 hFile;

    char *token = strtok(NULL," \t");
    if (token==NULL) {
        printf ("Missing parameter\n");
        return;
    }

    hFile = open2 (token);
    if (hFile<0) {
        printf ("Error: %d\n", hFile);
        return;
    }

    printf ("File opened with handle %d\n", hFile);
}

/**
Fecha o arquivo cujo handle � o parametro
Retorna sinaliza��o de erro
Retorna mensagem de opera��o completada
*/
void cmdClose(void) {
    FILE2 handle;

    char *token = strtok(NULL," \t");
    if (token==NULL) {
        printf ("Missing parameter\n");
        return;
    }

    if (sscanf(token, "%d", &handle)==0) {
        printf ("Invalid parameter\n");
        return;
    }

    int err = close2(handle);
    if (err<0) {
        printf ("Error: %d\n", err);
        return;
    }

    printf ("Closed file with handle %d\n", handle);
}

void cmdRead(void) {
    FILE2 handle;
    int size;

    // get first parameter => file handle
    char *token = strtok(NULL," \t");
    if (token==NULL) {
        printf ("Missing parameter\n");
        return;
    }
    if (sscanf(token, "%d", &handle)==0) {
        printf ("Invalid parameter\n");
        return;
    }

    // get second parameter => number of bytes
    token = strtok(NULL," \t");
    if (token==NULL) {
        printf ("Missing parameter\n");
        return;
    }
    if (sscanf(token, "%d", &size)==0) {
        printf ("Invalid parameter\n");
        return;
    }

    // Alloc buffer for reading file
    char *buffer = malloc(size);
    if (buffer==NULL) {
        printf ("Memory full\n");
        return;
    }

    // get file bytes
    int err = read2(handle, buffer, size);
    if (err<0) {
        printf ("Error: %d\n", err);
        return;
    }
    if (err==0) {
        printf ("Empty file\n");
        return;
    }

    // show bytes read
    dump(buffer, err);
    printf ("%d bytes read from file-handle %d\n", err, handle);

    free(buffer);
}

void cmdWrite(void) {
    FILE2 handle;
    int size;
    int err;

    // get first parameter => file handle
    char *token = strtok(NULL," \t");
    if (token==NULL) {
        printf ("Missing parameter\n");
        return;
    }
    if (sscanf(token, "%d", &handle)==0) {
        printf ("Invalid parameter\n");
        return;
    }

    // get second parameter => string
    token = strtok(NULL," \t");
    if (token==NULL) {
        printf ("Missing parameter\n");
        return;
    }
    size = strlen(token);

    // get file bytes
    err = write2(handle, token, size);
    if (err<0) {
        printf ("Error: %d\n", err);
        return;
    }
    if (err!=size) {
        printf ("Erro: escritos %d bytes, mas apenas %d foram efetivos\n", size, err);
        return;
    }

    printf ("%d bytes writen to file-handle %d\n", err, handle);
}

/**
Cria um novo diretorio
*/
void cmdMkdir(void) {
    // get first parameter => pathname
    char *token = strtok(NULL," \t");
    if (token==NULL) {
        printf ("Missing parameter\n");
        return;
    }
    // change working dir
    int err = mkdir2(token);
    if (err<0) {
        printf ("Error: %d\n", err);
        return;
    }

    printf ("Created new directory\n");
}

/**
Apaga um diretorio
*/
void cmdRmdir(void) {
    // get first parameter => pathname
    char *token = strtok(NULL," \t");
    if (token==NULL) {
        printf ("Missing parameter\n");
        return;
    }
    // change working dir
    int err = rmdir2(token);
    if (err<0) {
        printf ("Error: %d\n", err);
        return;
    }

    printf ("Directory was erased\n");
}

void cmdLs(void) {

    char *token = strtok(NULL," \t");
    if (token==NULL) {
        printf ("Missing parameter\n");
        return;
    }

    // Abre o diret�rio pedido
    DIR2 d;
    d = opendir2(token);
    if (d<0) {
        printf ("Open dir error: %d\n", d);
        return;
    }

    // Coloca diretorio na tela
    DIRENT2 dentry;
    while(1) {
	    int errCode = readdir2(d, &dentry);

	    if (errCode==-END_OF_DIR)
		    break;

	    if (errCode!=0) {
		    printf ("readdir2 error code: %d\n", errCode);
		    break;
	    }

	    printf ("%c %8u %s\n", (dentry.fileType==0x02?'d':'-'), dentry.fileSize, dentry.name);
    }

    closedir2(d);


}


/**
Chama a fun��o truncate2() da biblioteca e coloca o string de retorno na tela
*/
void cmdTrunc(void) {
    FILE2 handle;
    int size;

    // get first parameter => file handle
    char *token = strtok(NULL," \t");
    if (token==NULL) {
        printf ("Missing parameter\n");
        return;
    }
    if (sscanf(token, "%d", &handle)==0) {
        printf ("Invalid parameter\n");
        return;
    }

    // get second parameter => number of bytes
    token = strtok(NULL," \t");
    if (token==NULL) {
        printf ("Missing parameter\n");
        return;
    }
    if (sscanf(token, "%d", &size)==0) {
        printf ("Invalid parameter\n");
        return;
    }

    // posiciona CP na posicao selecionada
    int err = seek2(handle, size);
    if (err<0) {
        printf ("Error seek2: %d\n", err);
        return;
    }

    // trunca
    err = truncate2(handle);
    if (err<0) {
        printf ("Error truncate2: %d\n", err);
        return;
    }

    // show bytes read
    printf ("file-handle %d truncated to %d bytes\n", handle, size );
}

void cmdSeek(void) {
    FILE2 handle;
    int size;

    // get first parameter => file handle
    char *token = strtok(NULL," \t");
    if (token==NULL) {
        printf ("Missing parameter\n");
        return;
    }
    if (sscanf(token, "%d", &handle)==0) {
        printf ("Invalid parameter\n");
        return;
    }

    // get second parameter => number of bytes
    token = strtok(NULL," \t");
    if (token==NULL) {
        printf ("Missing parameter\n");
        return;
    }
    if (sscanf(token, "%d", &size)==0) {
        printf ("Invalid parameter\n");
        return;
    }

    // seek
    int err = seek2(handle, size);
    if (err<0) {
        printf ("Error: %d\n", err);
        return;
    }

    printf ("Seek completado para a posicao %d\n", size);

}
