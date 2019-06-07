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

// configure standards

int SENSIBILITY = 5;
float QFACTOR = 0.5f;

#include "../../header/SDL.h"
#include "../../header/video/surfaces.h"
#include "../../header/scenes/scenes.h"
#include "../../header/object/object.h"
#include "../../header/common/ag.h"

/******************************/

int get_sensibility()
{
	return SENSIBILITY;
}

void set_sensibility(int s)
{
	SENSIBILITY = s;
}

float get_qfactor()
{
	return QFACTOR;
}

void set_qfactor(float q)
{
	QFACTOR = q;
}

/******************************/

lg_video_handler::lg_video_handler ()
{
	this->my_obj = NULL;
	this->linked = false;
	this->w = this->h = 0;	
}

void lg_video_handler::set_obj(lg_object *obj)
{
	this->my_obj = obj;
}

void lg_video_handler::get_size(int *w, int *h)
{
	*w = this->w;
	*h = this->h;
}

int lg_video_handler::linkv(lg_video_handler *v)
{
	this->w = v->w;
	this->h = v->h;
}

bool lg_video_handler::get_render_area(lg_rect *inter)
{
	lg_scene *my_scene;
	int canvas_w, canvas_h, cam_x, cam_y, obj_x, obj_y, obj_w, obj_h;
	bool coll;
	lg_rect canvas, obj;
	
	this->my_obj->get_scene(&my_scene);
	my_scene->get_cam_pos(&cam_x, &cam_y);
	my_scene->get_canvas_size(&canvas_w, &canvas_h);
	
	this->my_obj->get_pos( &obj_x, &obj_y );
	this->get_size( &obj_w, &obj_h );

	canvas.x0 = cam_x;
	canvas.xf = cam_x + canvas_w;
	canvas.y0 = cam_y;
	canvas.yf = cam_y + canvas_h;

//	coll = coll_detection_rect
	
}

/////////////////////////

lg_object::lg_object()
{
	this->video_handler = NULL;
}

void lg_object::set(lg_object_param *p, lg_scene *my_scene)
{
	this->pos.set(p->x, p->y, 0);
	this->id = p->id;
	this->ang = p->ang;
	this->my_scene = my_scene;

	this->obj_flags = 0;
	this->obj_flags |= p->visible;
	this->obj_flags |= p->active;
}

lg_object::~lg_object()
{

}

/**********/
// overload

void lg_object::set_pos(int x, int y)
{
	this->pos.set((float)x, (float)y, 0);
}

void lg_object::set_pos(float x, float y)
{
	this->pos.set(x, y, 0);
}

void lg_object::set_pos(lg_point *p)
{
	this->pos.copy(p);
}
/***********/

void lg_object::get_pos(int *x, int *y)
{
	float xx, yy, zz;
	this->pos.get(&xx, &yy, &zz);
	*x = (int)xx;
	*y = (int)yy;
}

void lg_object::get_pos(float *x, float *y)
{
    float z;
	this->pos.get(x, y, &z);
}

void lg_object::get_pos(lg_point *p)
{
	p->copy(&this->pos);
}

/*********/

void lg_object::render()
{
	if ((this->obj_flags & LGOBJ_VISIBLE) && this->video_handler)
		this->video_handler->render();
}

void lg_object::get_video_handler (void *v)
{
    lg_video_handler **h;
    h = (lg_video_handler **)v;
	*h = this->video_handler;
}

void lg_object::set_video_handler (void *v)
{
	this->video_handler = (lg_video_handler *)v;
	this->video_handler->set_obj(this);
}

void lg_object::get_scene(void *v)
{
	lg_scene **sc;
	sc = (lg_scene**)v;
	*sc = this->my_scene;
}

int lg_object::check_if_inRange(int posx, int posy)
{
	int x, y, w, h;
	this->get_pos(&x, &y);
	this->get_size(&w, &h);
	if (posx >= x && posx <= x+w && posy >= y && posy <= y+h)
		return 1;
	return 0;
}

/*************************************************/

bool obj_check_coll_pixel (void *v1, void *v2)
{
	char *m1, *m2;
	int x1, x2, y1, y2, w1, w2, h1, h2;
	bool coll;
	lg_video_handler *video1, *video2;

	lg_object *o1, *o2;
	o1 = (lg_object*)v1;
	o2 = (lg_object*)v2;

	o1->get_video_handler(&video1);
	o2->get_video_handler(&video2);
	
	m1 = video1->get_pixel_map();
	m2 = video2->get_pixel_map();
	o1->get_pos(&x1, &y1);
	o2->get_pos(&x2, &y2);
	o1->get_size(&w1, &h1);
	o2->get_size(&w2, &h2);
	coll = coll_detection_pixel(m1, x1, y1, x1+w1, y1+h1,
	                            m2, x2, y2, x2+w2, y2+h2, SENSIBILITY);
	return coll;
}

