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
#include "../../header/scenes/scenes.h"
#include "../../header/object/object.h"
#include "../../header/common/common.h"

/***************************************************/

lg_object *lg_mouse_obj = NULL;

lg_scene::lg_scene()
{
	this->last_obj = NULL;
	this->background = NULL;
	this->cam_x = this->cam_y = 0;
}

lg_scene::~lg_scene()
{
	this->objects.destroy();
}

void lg_scene::set(lg_scene_param *p)
{
	this->start_x = p->start_x;
	this->start_y = p->start_y;
	this->canvas_w = p->canvas_w;
	this->canvas_h = p->canvas_h;
	this->bits = p->bits;
	
	this->scene_w = p->scene_w;
	this->scene_h = p->scene_h;
}

void lg_scene::set_back(SDL_Surface *s)
{
	if (this->background)
		this->background->destroy();
	else
		this->background = new lg_glbitmap;
	
	this->background->create_texture(s, 0, 0, 0, 255);
	this->scene_w = s->w;
	this->scene_h = s->h;
}

void lg_scene::set_cam_pos(int x, int y)
{
	if(x < 0)
		x = 0;
	
	if(y < 0)
		y = 0;
	
	if (this->canvas_w > this->scene_w)
		x = 0;
	else if((x + this->canvas_w) > this->scene_w)
		x = this->scene_w - this->canvas_w;

	if (this->canvas_h > this->scene_h)
		y = 0;
	else if((y + this->canvas_h) > this->scene_h)
		y = this->scene_h - this->canvas_h;

	this->cam_x = x;
	this->cam_y = y;
}

void lg_scene::add_object(void *v)
{
	lg_object *obj = (lg_object*)v;
	obj->set_scene(this);
	this->objects.add_end( obj );
	this->last_obj = this->objects.get_last();
}

void lg_scene::render()
{
	obj_list_el *o;
	lg_object *obj;
		
	glLoadIdentity();
	glTranslatef( (float)(this->start_x - this->cam_x), (float)(this->start_y - this->cam_y), 0.0f );

	if (this->background) {
		lg_rect r;
		int xf, yf;
		
		xf = this->cam_x + this->canvas_w;
		yf = this->cam_y + this->canvas_h;
		if (xf > this->scene_w)
			xf = this->scene_w;
		if (yf > this->scene_h)
			yf = this->scene_h;
				
		r.x0 = this->cam_x;
		r.xf = xf;
		r.y0 = this->cam_y;
		r.yf = yf;
		this->background->print(&r, this->cam_x, this->cam_y, 0.0f);
	}
	
	for (o=this->objects.get_first(); o!=NULL; o=o->get_next()) {
		obj = o->get_data();
		obj->render();
	}
}

lg_object* lg_scene::get_obj_bypos (int posx, int posy)
{
	obj_list_el *o;
	lg_object *obj;
	lg_object *id;
	
	id = NULL;
	o = this->objects.get_last();
	while (o!=NULL) {
		obj = o->get_data();
		if (obj->check_if_inRange(posx, posy)) {
			id = obj;
			o = NULL;
		}
		else
			o = o->get_prev();
	}
	return id;
}

/****************************/

void lg_get_mouseobj (void *v)
{
	lg_object **o;
	o = (lg_object **)v;
	*o = lg_mouse_obj;
}

void lg_set_mouse (void *v)
{
	if (lg_mouse_obj)
		delete lg_mouse_obj;

	if (v == NULL)
		lg_mouse_obj = NULL;
	else {
		lg_object_param objp;
		
		lg_mouse_obj = new lg_object();
		//lg_mouse_obj->set_video_handler(v);

	    objp.id = 0;
        objp.active = LGOBJ_ACTIVE;
        objp.visible = LGOBJ_VISIBLE;
        objp.x = 0;
        objp.y = 0;
		objp.ang = 0.0f;
        lg_mouse_obj->set(&objp, NULL);  
		lg_mouse_obj->set_video_handler(v);
	}
}

void lg_print_mouse ()
{
	lg_mouse_obj->render();
}

void lg_set_mousepos (int x, int y)
{
	lg_mouse_obj->set_pos(x, y);
}

void lg_set_mousepos_at_scene(void *v)
{
	lg_scene *sc;
	int camx, camy, mousex, mousey;
	sc = (lg_scene*)v;
	sc->get_cam_pos(&camx, &camy);
	lg_mouse_obj->get_pos(&mousex, &mousey);
	lg_mouse_obj->set_pos( mousex+camx, mousey+camy );
}
