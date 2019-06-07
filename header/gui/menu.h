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

#ifndef _LIB_GAME_MENU
#define _LIB_GAME_MENU

#include "../data/double_linked.h"
#include "../scenes/scenes.h"

class lg_input_obj;

typedef lg_dblinked_list<lg_input_obj> lst_inObj;
typedef lg_dblinked_el<lg_input_obj> lst_inObj_el;

class lg_menu: public lg_scene
{
	protected:
		lst_inObj input;
		lg_input_obj *focused;

	public:
		lg_menu();
		~lg_menu();

		void add_input(void *obj);
		//void add_output(void *obj);

		/* keyboard state
		 * mouse x
		 * mouse y
		 * event to process
		 */
		void process(Uint8 *keyboard, int mx, int my, SDL_Event *event);		
		lg_input_obj* get_inputobj_bypos (int posx, int posy);

		inline lg_input_obj* get_focused() {
			return this->focused;
		}

		inline void set_focused(lg_input_obj *obj) {
			this->focused = obj;
		}
};
#include "gui.h"
#endif
