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

#include "../../header/gui/checkbox.h"
#include <stdlib.h>

lg_checkbox::lg_checkbox() : lg_input_obj()
{	
	this->checked = 0;
	this->s = NULL;
	this->check_flags = 0;
}

void lg_checkbox::set_checkbox(lg_checkbox_param *p)
{
	this->set(&p->obj, NULL);
		
	if (p->flags & RENDER_OPENGL) {
		lg_cp_color(&this->signcolor, &p->border_color);
		this->check_flags |= RENDER_OPENGL;
		if (p->flags & OPENGL_BACK_SQUARE_SOLID)
			this->load_glbox_solid(p->size, p->size, &p->bg_color, &p->border_color, p->thickness);
		else if (p->flags & OPENGL_BACK_SQUARE)
			this->load_glbox(p->size, p->size, &p->border_color, p->thickness);
		else if (p->flags & OPENGL_BACK_CIRC_SOLID)
			this->load_glcirc_solid(p->size, &p->bg_color, &p->border_color, p->thickness);

	}
	else if (p->flags & LG_LOADFROM_SDLS) {
		this->check_flags |= RENDER_IMAGE;
		this->load_images((SDL_Surface*)p->bmp, &p->transp_color);
	}
	else if (p->flags & LG_LOADFROM_FILE) {
		this->check_flags |= RENDER_IMAGE;
		this->load_images((char*)p->bmp, &p->transp_color);
	}
	else if (p->flags & LG_LOADFROM_PACK) {
		this->check_flags |= RENDER_IMAGE;
		this->load_images(p->pack, (char*)p->bmp, &p->transp_color);
	}
}

void lg_checkbox::change()
{
	this->checked ^= 1;
	if (this->check_flags & RENDER_IMAGE) 
		this->s->set_state( (int) this->checked);
}

#define initialize_image  int transp;                                              \
                          int spr_uncheck[]  = {0, LGSPRITE_ENDOF_SEQ};	           \
                          int spr_check[]    = {1, LGSPRITE_ENDOF_SEQ};            \
                          this->s = new lg_sprite;                                 \
						  transp = (transp_color != NULL);                         \
                          this->s->add_state(0,0,0,spr_uncheck);                   \
                          this->s->add_state(1,0,0,spr_check);

#define set_vhandler      this->set_video_handler(this->s);

void lg_checkbox::load_images(SDL_Surface *s, lg_color *transp_color)
{
	initialize_image
	this->s->load_sprites(s, 2, 1, transp_color, transp);
	set_vhandler
}

void lg_checkbox::load_images(char *fname, lg_color *transp_color)
{
	initialize_image
	this->s->load_sprites(fname, 2, 1, transp_color, transp);
	set_vhandler
}

void lg_checkbox::load_images(lg_pack_file *pack, char *fname, lg_color *transp_color)
{
	initialize_image
	this->s->load_sprites(pack, fname, 2, 1, transp_color, transp);
	set_vhandler
}

void lg_checkbox::on_mouse_click_inDown(Uint8 button, int x, int y) 
{
	this->change();
}

void lg_checkbox::on_keyb_press(Uint8 *keys)
{
	if (this->is_focused() && keys[SDLK_SPACE]==SDL_PRESSED)		
		this->change();
}

void lg_checkbox::load_glbox_solid(int w, int h, lg_color *bgcolor, lg_color *bordercolor, int thickness)
{
	lg_opengl_border_rectangle *r;
	r = new lg_opengl_border_rectangle;
	this->set_video_handler(r);
	r->set_w(w);
	r->set_h(h);
	r->set_back_color(bgcolor);
	r->set_border_color(bordercolor);
	r->set_thickness(thickness);
}

void lg_checkbox::load_glbox(int w, int h, lg_color *bordercolor, int thickness)
{
	lg_opengl_Rectangle *r;
	r = new lg_opengl_Rectangle;
	this->set_video_handler(r);
	r->set_w(w);
	r->set_h(h);
	r->set_border_color(bordercolor);
	r->set_thickness(thickness);
}

void lg_checkbox::load_glcirc_solid(int r, lg_color *bgcolor, lg_color *bordercolor, int thickness)
{
	lg_opengl_border_circ *c;
	c = new lg_opengl_border_circ;
	this->set_video_handler(c);
	c->set_r(r);
	c->set_back_color(bgcolor);
	c->set_border_color(bordercolor);
	c->set_thickness(thickness);
}

void lg_checkbox::render()
{
	if (this->obj_flags & LGOBJ_VISIBLE){
		this->video_handler->render();
		if (this->check_flags & RENDER_OPENGL && this->checked){
			int x,y,w,h;
			this->get_pos(&x, &y);
			this->get_size(&w, &h);
			lg_gldraw_point(x + (w/2), y + (h/2), &this->signcolor, (float)w/2, 1);
		}
	}
}

