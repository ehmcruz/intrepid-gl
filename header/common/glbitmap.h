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

#ifndef _LIB_GAME_GLBITMAP
#define _LIB_GAME_GLBITMAP

#include "../SDL.h"
#include "../SDL_opengl.h"
#include "common.h"

class lg_glbitmap
{
	protected:
		int w, h, transparent;
		float max_w, max_h;
		GLuint id;

	public:
		~lg_glbitmap();
		void create_texture(SDL_Surface  *sourceSurface, int flags, 
			Uint8 colorKeyRed, Uint8 colorKeyGreen, Uint8 colorKeyBlue);
		void print(lg_rect *rect, int startx, int starty, float ang);
		void destroy();
};

#endif
