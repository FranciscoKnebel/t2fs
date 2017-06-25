/*
	INF01142 - Sistemas Operacionais I
	T2FS - 2017/1

	Display de setores fornecido.

	Modificações por Francisco Knebel
*/

#include "libs.h"

void help() {
	printf ("Testing program - read and write setores do arquivo t2fs_disk.dat\n");
	printf ("   DISPLAY - <setor>\n");
	printf ("\n");
	printf ("   HELP    - ?\n");
	printf ("   FIM     - f\n");
}

int main(int argc, char *argv[])
{
	char	command[128];
	char	*cmd;

	help();
	while (1) {
		printf ("CMD> ");

		scanf("%s", command);

		if (strlen(command)==0)
			continue;

		cmd = strtok(command, " ");

		if (strcmp(cmd,"?")==0) {
			help();
			continue;
		} else if (strcmp(cmd,"f")==0) {
			printf ("Fim.\n");
			break;
		} else {
			// comando d (display)
			unsigned char buffer[SECTOR_SIZE];

			int sector = atoi(cmd);
			int error = read_sector (sector, buffer);
			if (error) {
				printf ("read_sector (%d) error = %d\n", sector, error);
				continue;
			}

			char str[20];
			int linhaBase = SECTOR_SIZE * sector;
			int linha, coluna;
			for (linha=0; linha<16; ++linha) {
		    printf ("%04X  ", linhaBase+16*linha);
		    for (coluna=0; coluna<16; ++coluna) {
					int index = 16*linha+coluna;
					char c = buffer[index];

					if (c>=' ' && c<='z')
						str[coluna]=c;
					else
						str[coluna]=' ';

					printf ("%02X ", c&0xFF);
		    }
		    str[16]='\0';
		    printf (" *%s*\n", str);
			}
			continue;
		}
	}

  return 0;
}
