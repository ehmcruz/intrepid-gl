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

#ifndef _LIB_GAME_SURFACES
#define _LIB_GAME_SURFACES

#include "../SDL.h"
#include "../SDL_ttf.h"

#include "../object/object.h"
#include "../common/common.h"
#include "../common/glbitmap.h"
#include "../pack/pack.h"

class lg_object;
class lg_video_handler;

class lg_surface: public lg_video_handler
{
	protected:
		lg_glbitmap *bmp;
		char *pmap;
		char *pmap2;		
		lg_rect *map_rect;

	public:
		lg_surface();
		~lg_surface();
		
		// overload
		int load_image(char *fname, int mode, lg_color *color);
		int load_image(lg_pack_file *pack, char *fname, int mode, lg_color *color);
		int load_image(SDL_Surface *tmp, int mode, lg_color *color);

		virtual int link(void *v);
		
		void destroy ();
		int create_text (char *str, TTF_Font *font, lg_color *color);		
		virtual int render ();
		virtual char* get_pixel_map ();
		virtual void get_rect_map(lg_rect *rect);	
};

#endif
