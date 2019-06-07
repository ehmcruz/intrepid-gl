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

#include "../../header/gui/button.h"
#include <stdlib.h>

lg_button::lg_button() : lg_input_obj()
{	
	this->clicked = false;
	this->s = NULL;
	this->button_flags = 0;
	this->pol = NULL;
}

void lg_button::set_button(lg_button_param *p)
{
	this->set(&p->obj, NULL);

	lg_cp_color(&this->OUTbg_color, &p->OUTbg_color);
	lg_cp_color(&this->OUTborder_color, &p->OUTborder_color);
	lg_cp_color(&this->OUTfont_color, &p->OUTfont_color);

	lg_cp_color(&this->OVERbg_color, &p->OVERbg_color);
	lg_cp_color(&this->OVERborder_color, &p->OVERborder_color);
	lg_cp_color(&this->OVERfont_color, &p->OVERfont_color);

	lg_cp_color(&this->CLICKbg_color, &p->CLICKbg_color);
	lg_cp_color(&this->CLICKborder_color, &p->CLICKborder_color);
	lg_cp_color(&this->CLICKfont_color, &p->CLICKfont_color);

	if (p->flags & RENDER_OPENGL) {
		this->button_flags |= RENDER_OPENGL;
		this->render_text(p->text, p->font, &p->OUTfont_color);
		
		if (p->flags & OPENGL_BACK_SQUARE_SOLID)
			this->load_glbox_solid(p->w, p->h, &p->OUTbg_color, &p->OUTborder_color, p->thickness);
		else if (p->flags & OPENGL_BACK_SQUARE)
			this->load_glbox(p->w, p->h, &p->OUTborder_color, p->thickness);
		else if (p->flags & OPENGL_BACK_CIRC_SOLID)
			this->load_glcirc_solid(p->w, &p->OUTbg_color, &p->OUTborder_color, p->thickness);
		this->pol = (lg_opengl_polygon*)this->video_handler;
	}
	else if (p->flags & LG_LOADFROM_SDLS) {
		this->button_flags |= RENDER_IMAGE;
		this->load_images((SDL_Surface*)p->bmp, &p->transp_color);
	}
	else if (p->flags & LG_LOADFROM_FILE) {
		this->button_flags |= RENDER_IMAGE;
		this->load_images((char*)p->bmp, &p->transp_color);
	}
	else if (p->flags & LG_LOADFROM_PACK) {
		this->button_flags |= RENDER_IMAGE;
		this->load_images(p->pack, (char*)p->bmp, &p->transp_color);
	}
}

void lg_button::render_text(char *content, TTF_Font *font, lg_color *font_color)
{
	SDL_Color cor = { font_color->r, font_color->g, font_color->b };
	SDL_Surface *tmp;

	tmp = TTF_RenderText_Shaded(font, content, cor, cor);
	if (!tmp) {
		this->textW = this->textH = 0;
		return;
	}

	this->textW = tmp->w;
	this->textH = tmp->h;

	this->text.create_texture(tmp, 1, 0, 0, 255);
	SDL_FreeSurface(tmp);
}

#define initialize_image  int transp;                                              \
                          int spr_uncheck[]  = {0, LGSPRITE_ENDOF_SEQ};	           \
                          int spr_check[]    = {1, LGSPRITE_ENDOF_SEQ};            \
                          this->s = new lg_sprite;                                 \
						  transp = (transp_color != NULL);                         \
                          this->s->add_state(0,0,0,spr_uncheck);                   \
                          this->s->add_state(1,0,0,spr_check);

#define set_vhandler      this->set_video_handler(this->s);

void lg_button::load_images(SDL_Surface *s, lg_color *transp_color)
{
	initialize_image
	this->s->load_sprites(s, 2, 1, transp_color, transp);
	set_vhandler
}

void lg_button::load_images(char *fname, lg_color *transp_color)
{
	initialize_image
	this->s->load_sprites(fname, 2, 1, transp_color, transp);
	set_vhandler
}

void lg_button::load_images(lg_pack_file *pack, char *fname, lg_color *transp_color)
{
	initialize_image
	this->s->load_sprites(pack, fname, 2, 1, transp_color, transp);
	set_vhandler
}

void lg_button::on_mouse_click_inDown(Uint8 button, int x, int y) 
{
	this->clicked = true;
	this->pol->set_back_color(&this->CLICKbg_color);
	this->pol->set_border_color(&this->CLICKborder_color);
}

void lg_button::on_mouse_over(int x, int y)
{
	this->pol->set_back_color(&this->OVERbg_color);
	this->pol->set_border_color(&this->OVERborder_color);
}

void lg_button::on_mouse_out(int x, int y)
{
	this->pol->set_back_color(&this->OUTbg_color);
	this->pol->set_border_color(&this->OUTborder_color);
}

void lg_button::on_keyb_press(Uint8 *keys)
{
//	if (this->is_focused() && keys[SDLK_SPACE]==SDL_PRESSED)		
//		this->change();
}

void lg_button::load_glbox_solid(int w, int h, lg_color *bgcolor, lg_color *bordercolor, int thickness)
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

void lg_button::load_glbox(int w, int h, lg_color *bordercolor, int thickness)
{
	lg_opengl_Rectangle *r;
	r = new lg_opengl_Rectangle;
	this->set_video_handler(r);
	r->set_w(w);
	r->set_h(h);
	r->set_border_color(bordercolor);
	r->set_thickness(thickness);
}

void lg_button::load_glcirc_solid(int r, lg_color *bgcolor, lg_color *bordercolor, int thickness)
{
	lg_opengl_border_circ *c;
	c = new lg_opengl_border_circ;
	this->set_video_handler(c);
	c->set_r(r);
	c->set_back_color(bgcolor);
	c->set_border_color(bordercolor);
	c->set_thickness(thickness);
}

void lg_button::render()
{
	if (this->obj_flags & LGOBJ_VISIBLE) {
		this->video_handler->render();
		if (this->button_flags & RENDER_OPENGL){
			int x, y, dif, w, h;
			lg_rect r;
			
			this->get_pos(&x, &y);
			this->get_size(&w, &h);
			
			r.x0 = 0;
			r.y0 = 0;
			r.xf = this->textW;
			r.yf = this->textH;

			x += (w - this->textW)/2;
			y += (h - this->textH)/2;

			this->text.print(&r, x, y, 0.0f);
		}
	}
}

