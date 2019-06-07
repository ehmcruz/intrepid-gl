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

#include <stdlib.h>
#include <string.h>
#include "../../header/gui/textbox.h"
#include "../../header/video/opengl.h"
#include "../../header/gui/gui-common.h"

lg_textbox::lg_textbox(): lg_input_obj()
{
	this->content = NULL;
	this->maxChars = 0;
	this->focus_pos = 0;
	this->cam_pos = 0;
	this->nChars = 0;
	this->font = NULL;
	this->back = NULL;
	this->text_created = false;
}

lg_textbox::~lg_textbox()
{
	if (this->content)
		free(this->content);
	if (this->back)
		delete this->back;
}

void lg_textbox::set_textbox(lg_textbox_param *p)
{
	this->set(&p->obj, NULL);
	this->maxChars = p->maxChars;
	this->font = p->font;
	lg_cp_color(&this->font_color, &p->font_color);
	this->content = (char*) malloc(p->maxChars);
	if (!this->content)
		return;
	*this->content = 0; // initialize string
	this->borderSize = p->thickness;
	
	if (p->flags & RENDER_OPENGL) {
		this->text_flags |= RENDER_OPENGL;
		if (p->flags & OPENGL_BACK_SQUARE_SOLID)
			this->load_glbox_solid(p->w, p->h, &p->bg_color, &p->border_color, p->thickness);
		else if (p->flags & OPENGL_BACK_SQUARE)
			this->load_glbox(p->w, p->h, &p->border_color, p->thickness);
	}
	else if (p->flags & LG_LOADFROM_SDLS) {
		this->text_flags |= RENDER_IMAGE;
	//	this->load_images((SDL_Surface*)p->bmp, &p->transp_color);
	}
	else if (p->flags & LG_LOADFROM_FILE) {
		this->text_flags |= RENDER_IMAGE;
	//	this->load_images((char*)p->bmp, &p->transp_color);
	}
	else if (p->flags & LG_LOADFROM_PACK) {
		this->text_flags |= RENDER_IMAGE;
	//	this->load_images(p->pack, (char*)p->bmp, &p->transp_color);
	}
}

void lg_textbox::load_glbox_solid(int w, int h, lg_color *bgcolor, lg_color *bordercolor, int thickness)
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

void lg_textbox::load_glbox(int w, int h, lg_color *bordercolor, int thickness)
{
	lg_opengl_Rectangle *r;
	r = new lg_opengl_Rectangle;
	this->set_video_handler(r);
	r->set_w(w);
	r->set_h(h);
	r->set_border_color(bordercolor);
	r->set_thickness(thickness);
}

void lg_textbox::on_mouse_click_inDown(Uint8 button, int x, int y) 
{
	this->focus_pos = this->nChars;
}


void lg_textbox::on_keyb_press(Uint8 *keys)
{
	if (this->is_focused())
		this->write(keys);
}

void lg_textbox::write(Uint8 *keys)
{
	#define ASCII_SPACE         32
	#define ASCII_END           127

	Uint8 l;
	int i;

	if (keys[SDLK_BACKSPACE] == SDL_PRESSED) {
		if (this->focus_pos > 0) {
			for (i=this->focus_pos-1; i<this->nChars; i++)
				this->content[i] = this->content[i+1];
		
			if (this->nChars > 0)
				this->nChars--;
			if (this->focus_pos > 0)
				this->focus_pos--;
		}
	}
	else if (keys[SDLK_RIGHT] == SDL_PRESSED) {
		if (this->focus_pos < this->nChars)
			this->focus_pos++;
	}
	else if (keys[SDLK_LEFT] == SDL_PRESSED) {
		if (this->focus_pos > 0)
			this->focus_pos--;
	}
	else if (keys[SDLK_DELETE] == SDL_PRESSED) {
		if (this->focus_pos < this->nChars) {
			for (i=this->focus_pos; i<this->nChars; i++)
				this->content[i] = this->content[i+1];
			this->nChars--;
		}
	}
	else if (keys[SDLK_HOME] == SDL_PRESSED) {
		this->focus_pos = 0;
	}
	else if (keys[SDLK_END] == SDL_PRESSED) {
		this->focus_pos = this->nChars;
	}
	else {
		for (l=ASCII_SPACE; l<ASCII_END; l++) {
			if (this->nChars<this->maxChars && keys[l] == SDL_PRESSED) {
				for (i=this->nChars+1; i>this->focus_pos; i--)
					this->content[i] = this->content[i-1];
				this->content[ this->focus_pos ] = l;
				this->nChars++;
				this->focus_pos++;
			}
		}
	}
//printf("%s\n", content);
	this->create_text();
	this->set_camera();

	#undef ASCII_SPACE
	#undef ASCII_END
}

void lg_textbox::create_text()
{
	SDL_Color cor = { this->font_color.r, this->font_color.g, this->font_color.b };
	SDL_Surface *tmp;

	if (this->text_created)
		this->text.destroy();

	tmp = TTF_RenderText_Shaded(this->font, this->content, cor, cor);
	if (!tmp) {
		this->textW = this->textH = 0;
		this->text_created = false;
		return;
	}

	this->textW = tmp->w;
	this->textH = tmp->h;

	this->text_created = true;
	this->text.create_texture(tmp, 1, 0, 0, 255);
	SDL_FreeSurface(tmp);
}

void lg_textbox::set_camera()
{
	if (this->focus_pos < this->cam_pos)
		this->cam_pos = this->focus_pos;
	else {
		while (this->get_focusPos() > this->video_handler->get_w()) {
			this->cam_pos++;
		}
	}
}

void lg_textbox::render()
{
	int x, y, xpos;
	int wb, hb, dif;
	lg_rect r;
	
	if (this->obj_flags & LGOBJ_VISIBLE){
		this->video_handler->render();
		this->get_pos(&x, &y);

		this->get_renderArea(&r);
		this->video_handler->get_size(&wb, &hb);	
		dif = (hb - TTF_FontHeight(this->font) )/2;
		
		if (this->text_created)
			this->text.print(&r, x + TEXTBOX_WDIF, y+dif, 0.0f);

		if (this->is_focused()) {
			xpos = this->get_focusPos() + x + this->borderSize + TEXTBOX_WDIF;//printf("%i\n",xpos);
			lg_gldraw_line( xpos, y+dif, xpos, y+hb-dif, &this->font_color );
		}
	}
}

int lg_textbox::get_focusPos()
{
	char *start, *focus;
	int len, h;
	char tmp;

	if (this->nChars == 0)
		return 0;

	start = this->content + this->cam_pos;
	focus = this->content + this->focus_pos;

	tmp = *focus;
	*focus = 0;
	LG_GET_TEXTSIZE(start, this->font, &len, &h);

	*focus = tmp;
	return len;
}

void lg_textbox::get_renderArea (lg_rect *r)
{
	char tmp, *cam;
	int h, len;
	
	r->y0 = 0;
	r->yf = this->textH;

	if (this->cam_pos == 0)
		r->x0 = 0;
	else {
		cam = this->content + this->cam_pos;
		tmp = *cam;
		*cam = 0;
		LG_GET_TEXTSIZE(this->content, this->font, &len, &h);
		r->x0 = len;
		*cam = tmp;
	}

	r->xf = r->x0 + this->video_handler->get_w();
	if (r->xf > this->textW-TEXTBOX_WDIF)
		r->xf = this->textW-TEXTBOX_WDIF;
}

