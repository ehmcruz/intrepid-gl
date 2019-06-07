// +----------------------------------------------------------------------+
// |Intrepid 2D game library                                              |
// |                                                                      |
// |2007                                                                  |
// +----------------------------------------------------------------------+
// | Under GPL License                                                    |
// +----------------------------------------------------------------------+
// | This program is free software; you can redistribute it and/or modify |
// |   it under the terms of the GNU General Public License as published  |
// |   by the Free Software Foundation; either version 2 of the License,  |
// |   or (at your option) any later version.                             |
// |                                                                      |
// |   This program is distributed in the hope that it will be useful,    |
// |   but WITHOUT ANY WARRANTY; without even the implied warranty of     |
// |   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the      |
// |   GNU General Public License for more details.                       |
// |                                                                      |
// |   You should have received a copy of the GNU General Public License  |
// |   along with this program; if not, write to the Free Software        |
// |   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA           |
// |   02111-1307  USA                                                    |
// +----------------------------------------------------------------------+

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../header/zlib.h"
#include "../../header/pack/pack.h"

void usage();
void create_pack_list(char *buffer, char *fname);

void usage()
{
	printf("Usage: ./packer file_name_list pack_name\n\n");
}

void create_pack_list(char *buffer, char *fname)
{
	char *b;
	int n, tam, i;
	char **flist, **l, *name_start;

	n = 1;
	for (b=buffer; *b; b++) {
		if (*b == 10)
			n++;
		else if (*b == 13);
	}

	// n stores an approach of the number of file_names
	
	flist = (char**) malloc( n * sizeof(char*) );
	if (!flist) {
		printf("Out of memory");
		return;
	}

	for (i=0; i<n; i++)
		flist[i] = NULL;

	l = flist;
	name_start = buffer;
	tam = 0;
	
	for (b=buffer; *b; b++) {
		if (*b == 10) {
			if (tam > 0) {
				*l = (char*) malloc( tam + 1);
				if (!*l) {
					printf("Out of memory");
					return;
				}
			
				memcpy(*l, name_start, tam);
				*(*l + tam) = 0; // end of string

				tam = 0;
				l++;
			}
			
			name_start = b+1;
		}
		else if (*b == 13)
			name_start = b+1;
		else
			tam++;
	}


	// create pack
	lg_create_pack(fname, flist);
	
	// display file list

	i = 0;
	for (l=flist; *l!=NULL; l++) {
		printf("%i: %s\n", i, *l);
		free(*l);
		i++;
	}
	
	free(flist);
}

int main (int argc, char **argv)
{
	FILE *fp;
	char *buffer;
	int size;
	
	if (argc != 3) {
		usage();
		return 1;
	}
	
	// load the file into memory
	
	fp = fopen(argv[1], "rb");
	if (!fp) {
		printf("Error openning %s", argv[1]);
		return 2;
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	buffer = (char*) malloc(size+1);
	if (!buffer) {
		printf("Out of memory");
		return 3;
	}
	rewind(fp);
	fread(buffer, 1, size, fp);
	fclose(fp);
	buffer[size] = 0; // end of string

	create_pack_list(buffer, argv[2]);
	
	free(buffer);
	return 0;
}

