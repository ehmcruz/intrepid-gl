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

#ifndef _LIB_GAME_RADIOBUTTON
#define _LIB_GAME_RADIOBUTTON

#include "../data/simple_linked.h"
#include "../SDL.h"
#include "gui.h"
#include "checkbox.h"
#include "../object/object.h"
#include "../video/sprites.h"
#include "../video/opengl.h"

struct lg_radiobutton_class_param_stc {
	int id;
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

typedef struct lg_radiobutton_class_param_stc lg_radiobutton_class_param;

struct lg_radiobutton_param_stc {
	lg_object_param obj;
	int value;
};

typedef struct lg_radiobutton_param_stc lg_radiobutton_param;

class lg_radiobutton;

typedef lg_linked_list<lg_radiobutton> lst_radiobutton;
typedef lg_linked_el<lg_radiobutton> lst_radiobutton_el;

class lg_radioClass
{	
	protected:
		int id;
		lst_radiobutton rlist;
		lg_radiobutton_class_param param;
		
	public:
		~lg_radioClass();
		void set(lg_radiobutton_class_param *p);
		void add(lg_radiobutton *button, lg_radiobutton_param *bp);
		void change(lg_radiobutton *clicked);
		void uncheck_all();

		inline int get_id() {
			return this->id;
		}
};

/*************************************/

class lg_radiobutton : public lg_checkbox
{
	protected:
		int value;
		lg_radioClass *my_class;

	public:
		lg_radiobutton();
		void set_radio(lg_radiobutton_param *p, lg_radioClass *radio_class);
		virtual void change();

		inline int get_value() {
			return this->value;
		}
};

#endif

