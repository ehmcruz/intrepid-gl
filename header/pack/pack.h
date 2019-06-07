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

#ifndef _LIB_GAME_PACK
#define _LIB_GAME_PACK

#ifdef __cplusplus
extern "C" {
#endif

#include "../zlib.h"
#include "../SDL.h"
#include "../SDL_ttf.h"

#define PACK_NAME_LENGTH          30

struct stc_pack_header {
	char fname[PACK_NAME_LENGTH];
	int fsize;
};

typedef struct stc_pack_header lg_pack_header;

struct stc_pack_file {
	unsigned nfiles;
	lg_pack_header *list;
	gzFile fp;
};

typedef struct stc_pack_file lg_pack_file;

lg_pack_file* lg_open_pack (char *fname);
void lg_close_pack (lg_pack_file *pack);
int lg_create_pack (char *name, char **flist);
void* lg_read_pack (lg_pack_file *pack, char *fname, int *size);
SDL_RWops* lg_pack_to_rwops (lg_pack_file *pack, char *fname, void *buffer_used);
SDL_Surface* lg_pack_to_surface (lg_pack_file *pack, char *fname);
TTF_Font* lg_pack_to_font (lg_pack_file *pack, char *fname, int size);

#ifdef __cplusplus
}
#endif

#endif
