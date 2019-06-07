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

#include <SDL/SDL.h>
#include <stdlib.h>
#include "../../header/object/object.h"
#include "../../header/video/sprites.h"
#include "../../header/video/surfaces.h"
#include "../../header/common/common.h"

lg_sprite::lg_sprite():lg_video_handler()
{
	this->w = this->h = this->state = 0;
	this->cur_frame = 0;
	this->bmp = NULL;
	this->maps = NULL;
	this->pmap = NULL;
	this->map_rect = NULL;
}

lg_sprite::~lg_sprite()
{
	state_list_el *p;
	state_sprite *s;

	//free the state list	
	for (p=this->l.get_first(); p!=NULL; p=p->get_next()) {
		s = p->get_data();
		free(s->frame_seq);
		free(s);
	}
	this->l.destroy();

	if (this->linked)
		return;
	
	if (this->maps) {
		int i;
		for (i=0; i<this->num_frames; i++)
			free(this->maps[i]);
		free(this->maps);
	}

	if (this->map_rect)
		free(this->map_rect);

	if (this->pmap)
		free(this->pmap);
			
	//free the surface
	if (this->bmp)
    	delete this->bmp;
}

int lg_sprite::load_sprites(char *fname, int num_frames_x, int num_frames_y, lg_color *transp_color, int flags)
{
	SDL_Surface *tmp;
	tmp = SDL_LoadBMP(fname);
	if (!tmp)
		return 0;
	this->load_sprites(tmp, num_frames_x, num_frames_y, transp_color, flags);
	SDL_FreeSurface(tmp);
	return 1;
}

int lg_sprite::load_sprites(lg_pack_file *pack, char *fname, int num_frames_x, int num_frames_y, lg_color *transp_color, int flags)
{
	SDL_Surface *tmp;
	tmp = lg_pack_to_surface(pack, fname);
	if (!tmp)
		return 0;
	this->load_sprites(tmp, num_frames_x, num_frames_y, transp_color, flags);
	SDL_FreeSurface(tmp);
	return 1;
}

int lg_sprite::load_sprites(SDL_Surface *tmp, int num_frames_x, int num_frames_y, lg_color *transp_color, int flags)
{
	int i, j, size, num_frames, pos;
	int w2, h2;
	
	num_frames = num_frames_x * num_frames_y;
	this->bmp = new lg_glbitmap;
	
	this->flags = flags;	
	this->w = tmp->w / num_frames_x;
	this->h = tmp->h / num_frames_y;
	this->num_frames_x = num_frames_x;
	this->num_frames_y = num_frames_y;
	this->num_frames = num_frames;

	this->maps = (char **)malloc(num_frames * sizeof(char*));
	this->map_rect = (lg_rect*)malloc(num_frames * sizeof(lg_rect));

	w2 = this->w / get_sensibility();
	if (this->w % get_sensibility())
		w2++;
	
	h2 = this->h / get_sensibility();
	if (this->h % get_sensibility())
		h2++;
		
	size = w2 * h2;
	this->pmap = (char *)malloc(size);

	if (!this->maps || !this->map_rect || !this->pmap)
		return 0;

	for (i=0; i<num_frames; i++) {
		this->maps[i] = (char *)malloc(size);
		if (!this->maps[i])
			return 0;
	}
	
	if (this->flags & LF_CO_TRANSPARENT) {
		this->bmp->create_texture(tmp, 1, transp_color->r, transp_color->g, transp_color->b);
		for (i=0; i<num_frames_y; i++) {
			for (j=0; j<num_frames_x; j++) {
				pos = i*num_frames_x + j;
				map_pixels(this->maps[pos], tmp, j*this->w, i*this->h, (j+1)*this->w, (i+1)*this->h, get_sensibility(), transp_color, get_qfactor() );
				map_boundaries(tmp, j*this->w, i*this->h, (j+1)*this->w, (i+1)*this->h, transp_color, this->map_rect+pos);
				(this->map_rect+pos)->x0 -= j*this->w;
				(this->map_rect+pos)->xf -= j*this->w;
				(this->map_rect+pos)->y0 -= i*this->h;
				(this->map_rect+pos)->yf -= i*this->h;
			}
		}
	}
	else {
		this->bmp->create_texture(tmp, 0, 0, 0, 0);
		for (i=0; i<num_frames; i++) {
			for (j=0; j<size; j++)
				this->maps[i][j] = 1;
			(this->map_rect+i)->x0 = 0;
			(this->map_rect+i)->xf = this->w;
			(this->map_rect+i)->y0 = 0;
			(this->map_rect+i)->yf = this->h;
		}
	}
	
	this->linked = false;
	return 1;
}

int lg_sprite::link(void *v)
{
	lg_sprite *s = (lg_sprite*)v;

	this->linkv( (lg_video_handler*) v);
	
	this->bmp = s->bmp;
	this->maps = s->maps;
	this->pmap = s->pmap;
	this->map_rect = s->map_rect;

	this->num_frames_x = s->num_frames_x;
	this->num_frames_y = s->num_frames_y;
	this->num_frames = s->num_frames;
	this->flags = s->flags;
	
	this->linked = true;
	return 1;
}

void lg_sprite::add_state(int state, int anim_delay, int flags, int *sequence)
{
	int i;
	state_sprite *t;
	
	for (i=0; sequence[i]!=LGSPRITE_ENDOF_SEQ; i++);

	t = (state_sprite*) malloc(sizeof(state_sprite));
	if (!t)
		return;
	
	t->state = state;
	t->anim_delay = anim_delay;
	t->flags = flags;

	t->frame_seq = (int*) malloc(sizeof(int)*(i+1));
	if (!t->frame_seq)
		return;
	
	t->frame_seq[i] = LGSPRITE_ENDOF_SEQ;
	for ( i--; i>=0; i--)
		t->frame_seq[i] = sequence[i];

	this->l.add_end(t);
}

void lg_sprite::set_state(int state)
{
	this->state = state;
	this->cur_frame = 0;
}

state_sprite* lg_sprite::get_state_stc ()
{
	state_list_el *p;
	state_sprite *sp;

	// dosn't check if the state exist
	
	sp = NULL;
	p = this->l.get_first();
	while (p!=NULL) {
		sp = p->get_data();
		if (sp->state == this->state)
			p = NULL;
		else
			p = p->get_next();
	}

	return sp;
}

int lg_sprite::render()
{
	int x, y, frame, sprite_x, sprite_y;
	state_sprite *sp;
	lg_rect area;

	if (!this->bmp)
		return 0;
	
	sp = this->get_state_stc();

	if (sp!=NULL) {
		this->process_sprites(sp);
		frame = sp->frame_seq[this->cur_frame];
		if (frame != LGSPRITE_ENDOF_SEQ){
			this->my_obj->get_pos(&x, &y);
			sprite_x = (frame % this->num_frames_x) * this->w;
			sprite_y = (frame / this->num_frames_x) * this->h;
			area.x0 = sprite_x;
			area.xf = sprite_x + this->w;
			area.y0 = sprite_y;
			area.yf = sprite_y + this->h;

			this->bmp->print(&area, x, y, this->my_obj->get_ang() );
		}
	}

	return 1;
}

void lg_sprite::next_frame()
{
	state_sprite *sp;
	sp = this->get_state_stc();

	if (sp!=NULL){
		//check if we got to the last frame
		if (sp->frame_seq[this->cur_frame] == LGSPRITE_ENDOF_SEQ)
			this->cur_frame = 0;
		else{
			this->cur_frame++;
			if (sp->frame_seq[this->cur_frame] == LGSPRITE_ENDOF_SEQ)
				this->cur_frame = 0;
		}
	}
}

int lg_sprite::get_frame()
{
	int frame;
	state_sprite *sp;

	sp = this->get_state_stc();
	frame = sp->frame_seq[this->cur_frame];

	return frame;
}

char* lg_sprite::get_pixel_map ()
{
	float ang;
	int frame;
	
	ang = this->my_obj->get_ang();
	frame = this->get_frame();
	
	if (ang == 0.0f)
		return this->maps[frame];
	else {
		rotate_map( this->maps[frame], this->pmap, this->w, this->h, ang, get_sensibility() );
		return this->pmap;
	}
}

void lg_sprite::get_rect_map(lg_rect *rect) 
{
	lg_rect *tmp;
	tmp = this->map_rect + this->get_frame();
	rect->x0 = tmp->x0;
	rect->y0 = tmp->y0;
	rect->xf = tmp->xf;
	rect->yf = tmp->yf;
};

/****************************************************/

lg_sprite_framebased::lg_sprite_framebased(): lg_sprite()
{
	this->frame_count = 0;
}

void lg_sprite_framebased::set_state(int state)
{
	this->state = state;
	this->frame_count = 0;
	this->cur_frame = 0;
}

void lg_sprite_framebased::process_sprites(state_sprite *p)
{
	if (this->frame_count < p->anim_delay)
		this->frame_count++;
	else {
		this->frame_count = 0;
		this->next_frame();
	}
}

/****************************************************/

lg_sprite_msecbased::lg_sprite_msecbased(): lg_sprite()
{
	this->start = 0;
}

void lg_sprite_msecbased::set_state(int state)
{
	this->state = state;
	this->start = SDL_GetTicks();
	this->cur_frame = 0;
}

void lg_sprite_msecbased::anim_begin()
{
	this->start = SDL_GetTicks();
}

void lg_sprite_msecbased::process_sprites(state_sprite *p)
{
	int now, elapsed, n;
	now = SDL_GetTicks();
	elapsed = now - this->start;
	if (elapsed >= p->anim_delay) {
		n = elapsed / p->anim_delay;
		while (n-- > 0)
			this->next_frame();
		this->start = now - (elapsed % p->anim_delay);
	}
}

/****************************************************/

lg_sprite_valuebased::lg_sprite_valuebased(): lg_sprite()
{
	this->value = 0;
}

void lg_sprite_valuebased::set_state(int state)
{
	this->state = state;
	this->value = 0;
	this->cur_frame = 0;
}

void lg_sprite_valuebased::process_sprites(state_sprite *p)
{
	if (this->value >= p->anim_delay) {
		int n;
		n = this->value / p->anim_delay;
		while (n-- > 0)
			this->next_frame();
		this->value %= p->anim_delay;
	}
}

