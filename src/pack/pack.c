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

/*************************************/

int lg_create_pack (char *name, char **flist)
{
	gzFile fp;
	FILE *tmp;
	lg_pack_header header;
	void *buffer;
	char **fname;
	unsigned n;
	int fsize;

	fp = gzopen(name, "wb");
	if (!fp)
		return 1;

	n = 0;
	for (fname=flist; *fname; fname++)
		n++;

	if (!n) {
		gzclose(fp);
		return 4;
	}

	// first, write number of files in the pack
	gzwrite(fp, &n, sizeof(unsigned));

	// now, write the header
	for (fname=flist; *fname; fname++) {
		tmp = fopen(*fname, "rb");
		if (!tmp)
			return 2;
		fseek(tmp, 0, SEEK_END);
		header.fsize = ftell(tmp);
		fclose(tmp);
		strcpy(header.fname, *fname);
		gzwrite(fp, &header, sizeof(lg_pack_header));		
	}
	
	// now, last but not least, the content
	for (fname=flist; *fname; fname++) {
		tmp = fopen(*fname, "rb");
		if (!tmp)
			return 2;

		fseek(tmp, 0, SEEK_END);
		fsize = ftell(tmp);
		rewind(tmp);
		buffer = malloc( fsize );
		if (!buffer)
			return 3;
		fread(buffer, 1, fsize, tmp);

		gzwrite(fp, buffer, fsize);

		fclose(tmp);
		free(buffer);
	}

	gzclose(fp);
	return 0;
}

/*************************************/

lg_pack_file* lg_open_pack (char *fname)
{
	lg_pack_file *pack;
	unsigned bytes;
	
	pack = (lg_pack_file*) malloc( sizeof(lg_pack_file) );
	pack->fp = gzopen(fname, "rb");

	// read number of files
	gzread(pack->fp, &pack->nfiles, sizeof(unsigned));

	// read the header
	bytes = pack->nfiles * sizeof(lg_pack_header);
	pack->list = (lg_pack_header*) malloc( bytes );
	gzread(pack->fp, pack->list, bytes);

	return pack;
}

/*************************************/

void lg_close_pack (lg_pack_file *pack)
{
	gzclose(pack->fp);
	free(pack->list);
	free(pack);
}

/*************************************/

void* lg_read_pack (lg_pack_file *pack, char *fname, int *size)
{
	unsigned i;
	int ac, fsize;
	lg_pack_header *h;
	void *buffer;

	// ac will store the initial position of the file in the pack
	ac = sizeof(unsigned) + pack->nfiles*sizeof(lg_pack_header);
	fsize = -1;
	
	for (i=0; i<pack->nfiles; i++) {
		h = pack->list + i;
		if ( !strcmp(h->fname, fname)) {
			i = pack->nfiles; // go out the loop
			fsize = h->fsize;
		}
		else
			ac += h->fsize;
	}

	if (fsize != -1) { // found flag
		gzseek(pack->fp, ac, SEEK_SET);
		buffer = malloc(fsize);
		if (!buffer)
			return NULL;
		gzread(pack->fp, buffer, fsize);
		*size = fsize;
	}
	else
		buffer = NULL;
	
	return buffer;
}

/*************************************/

SDL_RWops* lg_pack_to_rwops (lg_pack_file *pack, char *fname, void *buffer_used)
{
	void *buffer;
	int size;
	SDL_RWops *rw;
	
	buffer = lg_read_pack(pack, fname, &size);
	if (!buffer)
		return NULL;

	rw = SDL_RWFromMem(buffer, size);
//	free(buffer);
	*( (char**)buffer_used ) = (char*)buffer;
	return rw;
}

/*************************************/

SDL_Surface* lg_pack_to_surface (lg_pack_file *pack, char *fname)
{
	SDL_RWops *rw;
	SDL_Surface *s;
	void *buffer;

	rw = lg_pack_to_rwops(pack, fname, &buffer);
	if (!rw)
		return NULL;

	s = SDL_LoadBMP_RW(rw, 0);
	SDL_FreeRW(rw);
	free(buffer);
	return s;
}

/*************************************/

TTF_Font* lg_pack_to_font (lg_pack_file *pack, char *fname, int size)
{
	SDL_RWops *rw;
	TTF_Font *f;
	void *buffer;

	rw = lg_pack_to_rwops(pack, fname, &buffer);
	if (!rw)
		return NULL;

	f = TTF_OpenFontRW(rw, 1, size);
//	SDL_FreeRW(rw);
//	free(buffer);
	return f;
}

