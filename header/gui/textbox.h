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

#ifndef _LIB_GAME_TEXTBOX
#define _LIB_GAME_TEXTBOX

#include "../SDL.h"
#include "../SDL_ttf.h"
#include "gui.h"
#include "../video/surfaces.h"
#include "../common/common.h"
#include "../common/gldraw.h"
#include <string.h>

#define OPENGL_BACK_SQUARE          0x0100
#define OPENGL_BACK_SQUARE_SOLID    0x0200

#define TEXTBOX_WDIF     2

struct lg_textbox_paramstc {
	lg_object_param obj;
	TTF_Font *font;
	int flags;
	int maxChars;
	lg_color font_color;

	// if opengl
	int w, h;
	int thickness;
	lg_color bg_color, border_color;

	// if sdl/file/pack
	void *img1, *img2, *img3;
	
	lg_pack_file *pack;
};

typedef struct lg_textbox_paramstc lg_textbox_param;

class lg_textbox : public lg_input_obj
{
	protected:
		char *content;
		TTF_Font *font;
		lg_surface *back;
		lg_glbitmap text;
		int textW, textH;
		lg_color font_color;
		int nChars, maxChars;
		int text_flags;
		int focus_pos, cam_pos;
		int borderSize;
		bool text_created;

	public:
		lg_textbox();
		~lg_textbox();

		void set_textbox(lg_textbox_param *p);
		void write(Uint8 *keys);

		void load_glbox_solid(int w, int h, lg_color *bgcolor, lg_color *bordercolor, int thickness);
		void load_glbox(int w, int h, lg_color *bordercolor, int thickness);

		int get_focusPos();		
		void set_camera();		
		void create_text();
		void get_renderArea(lg_rect *r);

		virtual void on_mouse_click_inDown(Uint8 button, int x, int y);
		virtual void on_keyb_press(Uint8 *keys);
		virtual void render();

		inline char* get_value() {
			return this->content;
		}

		inline void set_value(char *c) {
			strcpy(this->content, c);
		}
};

#endif

