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

#ifndef _LIB_GAME_SCENES
#define _LIB_GAME_SCENES

#include "../SDL.h"
#include "../data/double_linked.h"
#include "../object/object.h"
#include "../video/sprites.h"
#include "../common/common.h"
#include "../common/glbitmap.h"

class lg_object;

typedef lg_dblinked_list<lg_object> obj_list;
typedef lg_dblinked_el<lg_object> obj_list_el;

struct stc_lg_scene_param {
	int canvas_w, canvas_h;
	int scene_w, scene_h;
	int start_x, start_y;
	int bits;
};
typedef struct stc_lg_scene_param lg_scene_param;

class lg_scene
{
	protected:
		obj_list objects;
		obj_list_el *last_obj;
		
		lg_glbitmap *background;
		int canvas_w, canvas_h, bits;
		int scene_w, scene_h;
		int cam_x, cam_y;
		int start_x, start_y;
		
	public:					
		lg_scene();
		~lg_scene();
		void set(lg_scene_param *p);
		void set_back(SDL_Surface *s);		
		void set_cam_pos(int x, int y);
		void add_object(void *v);
		void render();
		lg_object* get_obj_bypos (int posx, int posy);

		inline void get_cam_pos(int *x, int *y) {
			*x = this->cam_x;
			*y = this->cam_y;
		}

		inline void get_pos(int *x, int *y) {
			*x = this->start_x;
			*y = this->start_y;
		}
		
		inline void get_scene_size(int *w, int *h) {
			*w = this->scene_w;
			*h = this->scene_h;
		}
		
		inline void get_canvas_size(int *w, int *h) {
			*w = this->canvas_w;
			*h = this->canvas_h;
		}
};

void lg_set_mousepos_at_scene(void *v);
void lg_get_mouseobj(void *v);
void lg_set_mouse (void *v);
void lg_print_mouse ();
void lg_set_mousepos (int x, int y);

#endif
