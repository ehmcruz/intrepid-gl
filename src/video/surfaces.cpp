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
#include <SDL/SDL_ttf.h>
#include <stdlib.h>
#include "../../header/video/surfaces.h"

lg_surface::lg_surface(): lg_video_handler()
{
	this->bmp = NULL;
	this->pmap = NULL;
	this->pmap2 = NULL;	
	this->map_rect = NULL;
}

lg_surface::~lg_surface()
{
	this->destroy();
}

int lg_surface::load_image(char *fname, int mode, lg_color *color)
{
	SDL_Surface *tmp;
	tmp = SDL_LoadBMP(fname);
	if (tmp == NULL)
		return 0;
	this->load_image(tmp, mode, color);
	SDL_FreeSurface(tmp);
	return 1;
}

int lg_surface::load_image(lg_pack_file *pack, char *fname, int mode, lg_color *color)
{
	SDL_Surface *tmp;
	tmp = lg_pack_to_surface(pack, fname);
	if (tmp == NULL)
		return 0;
	this->load_image(tmp, mode, color);
	SDL_FreeSurface(tmp);	
	return 1;
}

int lg_surface::load_image(SDL_Surface *tmp, int mode, lg_color *color)		
{
	int size, i;
	int w2, h2;

	this->destroy();

	this->w = tmp->w;
	this->h = tmp->h;
	this->bmp = new lg_glbitmap;

	this->map_rect = (lg_rect*)malloc(sizeof(lg_rect));

	w2 = this->w / get_sensibility();
	if (this->w % get_sensibility())
		w2++;
	
	h2 = this->h / get_sensibility();
	if (this->h % get_sensibility())
		h2++;
	
	size = w2 * h2;
	
	this->pmap = (char*) malloc (size);
	this->pmap2 = (char*) malloc (size);	
	if (! (this->pmap && this->pmap2 && this->map_rect))
		return 0;
	
	if (mode & LF_CO_TRANSPARENT){
		this->bmp->create_texture(tmp, 1, color->r, color->g, color->b);
		map_pixels(this->pmap, tmp, 0, 0, this->w, this->h, get_sensibility(), color, get_qfactor());
		map_boundaries(tmp, 0, 0, this->w, this->h, color, this->map_rect);
	}
	else {
		this->bmp->create_texture(tmp, 0, 0, 0, 0);
		for (i=0; i<size; i++)
			this->pmap[i] = 1;
		this->map_rect->x0 = 0;
		this->map_rect->xf = this->w;
		this->map_rect->y0 = 0;
		this->map_rect->yf = this->h;
	}
	
	this->linked = false;
	return 1;
}

int lg_surface::link (void *v)
{
	lg_surface *s = (lg_surface*)v;

	this->linkv( (lg_video_handler*) v);
	
	this->bmp = s->bmp;
	this->pmap = s->pmap;
	this->pmap2 = s->pmap2;
	this->map_rect = s->map_rect;
	
	this->linked = true;
	return 1;
}

char* lg_surface::get_pixel_map ()
{
	float ang;
	ang = this->my_obj->get_ang();
	if (ang == 0.0f)
		return this->pmap;
	else {
		rotate_map( this->pmap, this->pmap2, this->w, this->h, ang, get_sensibility() );
		return this->pmap2;
	}
}

void lg_surface::get_rect_map(lg_rect *rect) 
{
	rect->x0 = this->map_rect->x0;
	rect->y0 = this->map_rect->y0;
	rect->xf = this->map_rect->xf;
	rect->yf = this->map_rect->yf;
};

void lg_surface::destroy ()
{
	if (!this->linked) {		
		if (this->bmp)
			delete this->bmp;
		if (this->pmap)
			free(this->pmap);
		if (this->pmap2)
			free(this->pmap2);
		if (this->map_rect)
			free(this->map_rect);
	}
	this->bmp = NULL;
	this->pmap = NULL;
	this->pmap2 = NULL;
	this->map_rect = NULL;
}

int lg_surface::create_text (char *str, TTF_Font *font, lg_color *color)
{
	SDL_Color cor = { color->r, color->g, color->b };
	SDL_Surface *tmp;
	lg_color bgcolor;

	this->destroy();	
	tmp = TTF_RenderText_Shaded(font, str, cor, cor);
	if (!tmp) {
		this->w = this->h = 0;
		return 0;
	}

	bgcolor.r = 0;
	bgcolor.g = 0;
	bgcolor.b = 255;
	this->load_image(tmp, LF_CO_TRANSPARENT, &bgcolor);
	SDL_FreeSurface(tmp);
	return 1;
}

int lg_surface::render ()
{
	int x, y;
	lg_rect r;

	if (!this->bmp)
		return 0;
	
	this->my_obj->get_pos(&x, &y);

	r.x0 = 0;
	r.xf = this->w;
	r.y0 = 0;
	r.yf = this->h;

	this->bmp->print(&r, x, y, this->my_obj->get_ang());
	return 1;
}
