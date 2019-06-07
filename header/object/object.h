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

#ifndef _LIB_GAME_OBJECT
#define _LIB_GAME_OBJECT

#include "../SDL.h"

#include "../common/ag.h"
#include "../common/common.h"

class lg_object;
class lg_sprite;
class lg_surface;
class lg_scene;

struct lg_object_paramstc {
	int id;
	int active;
	int visible;
	float ang;
	float x;
	float y;
};

typedef struct lg_object_paramstc lg_object_param;

class lg_video_handler
{
	protected:
		int w,h;
		lg_object *my_obj;
		bool linked;

	public:
		lg_video_handler();
		void set_obj(lg_object *obj);
		bool get_render_area(lg_rect *inter);		
		virtual int render () { }
		void get_size(int *w, int *h);
		virtual char* get_pixel_map() { }
		virtual void get_rect_map(lg_rect *rect) { }
		virtual int link(void *v) { }
		int linkv(lg_video_handler *v);

		inline int get_w() {
			return this->w;
		}

		inline int get_h() {
			return this->h;
		}
};

#define LGOBJ_VISIBLE          0x01
#define LGOBJ_INVISIBLE        0x00

#define LGOBJ_ACTIVE           0x02
#define LGOBJ_INATIVE          0x00

int get_sensibility();
void set_sensibility(int s);
float get_qfactor();
void set_qfactor(float q);

class lg_object
{
	protected:
		float ang;		
		int id;
		/*
		 *     Bit 0: visible
		 *     Bit 1: active
		 */
		int obj_flags;
		lg_point pos;
		
		lg_video_handler *video_handler;
		lg_scene *my_scene;

	public:		
		//Functions
		lg_object();
		~lg_object();

		void set (lg_object_param *p, lg_scene *my_scene);
		
		//overload
		void set_pos(int x, int y);
		void set_pos(float x, float y);
        void set_pos(lg_point *p);
		
        //overload
		void get_pos(int *x, int *y);
		void get_pos(float *x, float *y);
		void get_pos(lg_point *p);

		void get_video_handler (void *v);
		void set_video_handler (void *v);

		void get_scene(void *v);

		virtual void render();
		virtual int check_if_inRange(int posx, int posy);
		virtual void pos_changed() { }
		
		inline void set_ang (float ang) {
			this->ang = ang;
		}
		
		inline float get_ang() {
			return this->ang;
		}
		
		inline void get_size(int *w, int *h) {
			this->video_handler->get_size(w, h);
		}
		
		inline int get_id() {
			return this->id;
		}

		inline void set_scene(void *scene) {
			this->my_scene = (lg_scene*)scene;
		}
};

bool obj_check_coll_pixel (void *v1, void *v2);

#include "../video/sprites.h"
#include "../video/surfaces.h"
#include "../scenes/scenes.h"

#endif
