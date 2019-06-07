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

#ifndef _LIB_GAME_CHECKBOX
#define _LIB_GAME_CHECKBOX

#include "../SDL.h"
#include "gui.h"
#include "../object/object.h"
#include "../video/sprites.h"
#include "../video/opengl.h"

#define OPENGL_BACK_SQUARE          0x0100
#define OPENGL_BACK_SQUARE_SOLID    0x0200
#define OPENGL_BACK_CIRC            0x0400
#define OPENGL_BACK_CIRC_SOLID      0x0800

struct lg_checkbox_param_stc {
	lg_object_param obj;
	int flags;

	// if opengl
	int size, thickness;
	lg_color bg_color, border_color;

	// if sdl/file/pack
	void *bmp;
	lg_color transp_color;

	// if pack
	lg_pack_file *pack;
};

typedef struct lg_checkbox_param_stc lg_checkbox_param;

class lg_checkbox : public lg_input_obj
{
	protected:
		bool checked;
		lg_sprite *s;
		int check_flags;
		lg_color signcolor;

	public:
		lg_checkbox();

		void set_checkbox(lg_checkbox_param *p);
		
		virtual void on_mouse_click_inDown(Uint8 button, int x, int y);
		virtual void on_keyb_press(Uint8 *keys);
		virtual void change();	
		virtual void render();
		
		void load_glbox_solid(int w, int h, lg_color *bgcolor, lg_color *bordercolor, int thickness);
		void load_glbox(int w, int h, lg_color *bordercolor, int thickness);
		void load_glcirc_solid(int r, lg_color *bgcolor, lg_color *bordercolor, int thickness);		
		
		//Overload
		void load_images(SDL_Surface *s, lg_color *transp_color);
		void load_images(char *fname, lg_color *transp_color);
		void load_images(lg_pack_file *pack, char *fname, lg_color *transp_color);

		inline void set_flags ( int flags ){
			this->check_flags = flags;
		}
		
		inline bool get_checked() {
			return this->checked;
		}

		inline void set_checked(int v) {
			this->checked = v;
		}
};

#endif

