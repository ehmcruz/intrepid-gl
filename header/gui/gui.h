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

#ifndef _LIB_GAME_GUI
#define _LIB_GAME_GUI

#include "../SDL.h"
#include "../object/object.h"

#define RENDER_OPENGL        0x0001         // 0000 0000 0000 0001
#define RENDER_IMAGE         0x0002         // 0000 0000 0000 0010

#define LG_LOADFROM_SDLS     0x0100         // 0000 0001 0000 0000
#define LG_LOADFROM_FILE     0x0200         // 0000 0010 0000 0000
#define LG_LOADFROM_PACK     0x0400         // 0000 0100 0000 0000

class lg_menu;

class lg_input_obj : public lg_object
{		
	public:
		lg_input_obj();

		// Event list

		virtual void set_focus() { }
		virtual void unset_focus() { }
		
		virtual void on_mouse_over(int x, int y) {}
		virtual void on_mouse_out(int x, int y) {}
		
		virtual void on_mouse_click_inDown(Uint8 button, int x, int y) {}
		virtual void on_mouse_click_inUp(Uint8 button, int x, int y) {}
		virtual void on_mouse_click_outDown(Uint8 button, int x, int y) {}
		virtual void on_mouse_click_outUp(Uint8 button, int x, int y) {}
		
		virtual void on_keyb_press(Uint8 *keys) {}

		// end event list

		bool is_focused();
};

class lg_output_obj : public lg_object
{
};

#include "menu.h"

#endif
