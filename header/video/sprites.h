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

#ifndef _LIB_GAME_SPRITES
#define _LIB_GAME_SPRITES

#include "../SDL.h"
#include "../object/object.h"
#include "surfaces.h"
#include "../common/common.h"
#include "../common/glbitmap.h"
#include "../pack/pack.h"
#include "../data/simple_linked.h"

class lg_object;
class lg_video_handler;

struct state_sprite_stc{
	int state;
	int anim_delay;
	int *frame_seq;                       
	int flags;
};

typedef struct state_sprite_stc state_sprite;

typedef lg_linked_list<state_sprite> state_list;
typedef lg_linked_el<state_sprite> state_list_el;

#define LGSPRITE_ENDOF_SEQ       -1

class lg_sprite: public lg_video_handler
{
	protected:		
		//int w,h;
		int state;
		int flags;
		int cur_frame;
		int num_frames_x, num_frames_y, num_frames;
		state_list l;
		lg_glbitmap *bmp;
		char **maps;
		char *pmap;
		lg_rect *map_rect;
		
	public:
		lg_sprite();
		~lg_sprite();
		
		// overload
		int load_sprites(char *fname, int num_frames_x, int num_frames_y, lg_color *transp_color, int flags);
		int load_sprites(lg_pack_file *pack, char *fname, int num_frames_x, int num_frames_y, lg_color *transp_color, int flags);
		int load_sprites(SDL_Surface *tmp, int num_frames_x, int num_frames_y, lg_color *transp_color, int flags);
		
		virtual int link(void *v);
		
		void add_state(int state, int anim_delay, int flags, int *sequence);
		void next_frame();
		void prev_frame();
		int get_frame();
		state_sprite* get_state_stc ();		
	
		virtual int render();
		virtual char* get_pixel_map ();
		virtual void get_rect_map(lg_rect *rect);		
		virtual void set_state(int state);
		virtual void process_sprites(state_sprite *p) { }

		inline int get_state() {
			return this->state;
		}
};

/*************************************/

class lg_sprite_framebased: public lg_sprite
{
	protected:
		int frame_count;
	public:
		lg_sprite_framebased();
		virtual void set_state(int state);
		virtual void process_sprites(state_sprite *p);
};

/*************************************/

class lg_sprite_msecbased: public lg_sprite
{
	protected:
		int start;
	public:
		lg_sprite_msecbased();
		void anim_begin();
		virtual void set_state(int state);
		virtual void process_sprites(state_sprite *p);
};

/*************************************/

class lg_sprite_valuebased: public lg_sprite
{
	protected:
		int value;
	public:
		lg_sprite_valuebased();

		virtual void set_state(int state);
		virtual void process_sprites(state_sprite *p);

		inline int get_value() {
			return this->value;
		}

		inline void set_value(int v) {
			this->value = v;
		}

		inline void add_value(int v) {
			this->value += v;
		}

		inline void sub_value(int v) {
			this->value -= v;
		}
};

#endif
