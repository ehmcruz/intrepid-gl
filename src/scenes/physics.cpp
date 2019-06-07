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
#include "../../header/scenes/physics.h"
#include "../../header/object/object.h"
#include "../../header/object/phys_object.h"
#include "../../header/common/common.h"

/*************************************************/

void lg_phy_wall_line::set (int startx, int starty, int endx, int endy, int sens)
{
	lg_point p;
	lg_vector v;
	
	v.set( (float)(endx-startx), (float)(endy-starty), 0.0f );
	p.set( (float)startx, (float)starty, 0.0f );
	this->l.set_point(&p);
	this->l.set_vector(&v);

	this->start_x = startx;
	this->start_y = starty;
	this->end_x = endx;
	this->end_y = endy;

	this->delta_value = sens;
}

bool lg_phy_wall_line::check_coll_rect (lg_physics_obj *obj, lg_rect *result)
{
	int xcoll_start, xcoll_end;
	int x0, y0, posx, posy, xf, yf;
	float px, py, pz, vi, vj, vk;
	int tmp_i, tmp_f;
	lg_video_handler *video;
	lg_rect rect;
	lg_point p;
	lg_vector v;
	int deltax, deltay;
	float deltax_f, deltay_f, aux;
	lg_vector delta;

	obj->get_video_handler(&video);
	video->get_rect_map(&rect);
	obj->get_pos(&posx, &posy);
	x0 = posx+rect.x0;
	xf = posx+rect.xf;
	y0 = posy+rect.y0;
	yf = posy+rect.yf;
	
/*lg_color cc;
cc.r = 255; cc.g=0; cc.b=0;
lg_gldraw_line(this->start_x, this->start_y, end_x,end_y, &cc);
cc.r = 0; cc.g=255; cc.b=0;*/
//lg_gldraw_line(x0, yf, xf,yf, &cc);
//cc.r = 0; cc.g=0; cc.b=255;

	this->l.get_vector(&v);
	this->l.get_point(&p);
	p.get(&px, &py, &pz);
	v.get(&vi, &vj, &vk);

	delta.copy(&v);
	delta.set_module((float)this->delta_value);
	delta.get(&deltax_f, &deltay_f, &aux);
	deltax = (int)deltax_f;
	deltay = (int)deltay_f;
	if (deltax < 0)
		deltax *= -1;
	if (deltay < 0)
		deltay *= -1;

	if (this->end_x > this->start_x) {
		tmp_i = this->start_x;
		tmp_f = this->end_x;
	}
	else {
		tmp_i = this->end_x;
		tmp_f = this->start_x;
	}

	tmp_i += deltax;
	tmp_f -= deltax;
	
	if (check_intersection(x0, xf, tmp_i, tmp_f, &xcoll_start, &xcoll_end)) {
		float alfa_start, alfa_end;
		int ycoll_start, ycoll_end, wall_starty, wall_endy;

		if (vi != 0.0f) {
			alfa_start = ( (float)xcoll_start - px ) / vi;
			alfa_end = ( (float)xcoll_end - px ) / vi;
			wall_starty = (int)(py + alfa_start*vj);
			wall_endy = (int)(py + alfa_end*vj);
		}
		else {
			wall_starty = (int)py;
			wall_endy = (int)(py+vj);
		}
//		wall_starty = 
//lg_gldraw_line(x0, wall_starty, xf,wall_endy, &cc);
//lg_gldraw_line(xcoll_start, wall_starty, xcoll_end,wall_endy, &cc);

		if (wall_endy > wall_starty) {
			tmp_i = wall_starty;
			tmp_f = wall_endy;
		}
		else {
			tmp_i = wall_endy;
			tmp_f = wall_starty;
		}
		
		tmp_i += deltay;
		tmp_f -= deltay;
		
		if (check_intersection(y0, yf, tmp_i, tmp_f, &ycoll_start, &ycoll_end)) {
			result->x0 = xcoll_start;
			result->xf = xcoll_end;
			result->y0 = ycoll_start;
			result->yf = ycoll_end;
			return true;
		}
	}
	return false;
}

void lg_phy_wall_line::get_perpendicular (lg_vector *result)
{
	lg_vector k, wall_vec;
	k.set(0.0f, 0.0f, 1.0f);
	this->l.get_vector(&wall_vec);
	result->prod_vec(&wall_vec, &k);
}

/************************************/

void lg_phy_wall_line_stopAll::check_coll (lg_physics_obj *obj)
{
	lg_vector normal_force, v, projection, *force_handler, *velocity;
	lg_point wall_start, obj_pos;
	lg_rect coll_area;
	int w, h;
	float cosang;

	obj->get_forces_sum_handler(&force_handler);
		
	if (this->check_coll_rect(obj, &coll_area)) {
		int w, h;
					
		this->get_perpendicular(&normal_force);
	
/*		lg_vector aux;
		lg_point obj_center, obj_pos, center_coll;
		int ow, oh, ox, oy;
		float dist;

		center_coll.set( (float)(coll_area.xf - coll_area.x0), (float)(coll_area.yf - coll_area.x0), 0.0f );
		obj->get_pos(&obj_pos);
		obj_center.copy(&obj_pos);
		obj->get_pos(&ox, &oy);
		obj->get_size(&ow, &oh);
		aux.set((float)ow, (float)oh, 0.0f);
		obj_center.add(&aux);
		ow /= 2;
		oh /= 2;
		dist = dist_ptop(&obj_center, &center_coll);
		if (dist < (float)oh) {
			lg_vector coll_vector;
			coll_vector.set_bypoint(&center_coll, &obj_center);
			coll_vector.set_module( (float)oh - dist );
			object_remove_coll(obj, &coll_vector);
		}//////////////
		obj->get_pos(&ox, &oy);
		obj->get_size(&ow, &oh);
		oy += oh-1;	
		if (oy > this->start_y) {
			lg_vector coll_vector;
			coll_vector.set(0.0f, -1.0f, 0.0f);
			coll_vector.set_module( (float)(oy - this->start_y) );
			object_remove_coll(obj, &coll_vector);
		}*/
		
		
		
		// find the direction of normal force
		obj->get_size(&w, &h);
		obj->get_pos(&obj_pos);
		v.set( (float)(w/2), (float)(h/2), 0.0f );
		obj_pos.add(&v);
		this->l.get_point(&wall_start);
		v.set_bypoint(&wall_start, &obj_pos); // v start at wall and ends in the object
		cosang = lg_vector::cos_ang(&v, &normal_force);
		if (cosang < 0.0f)
			normal_force.multiply(-1.0f);
				
		cosang = lg_vector::cos_ang(force_handler, &normal_force);
		if (cosang < 0.0f) {
			projection.proj_vec(force_handler, &normal_force);
			force_handler->add(&projection);
		}
		
		obj->get_speed_handler(&velocity);
		cosang = lg_vector::cos_ang(&normal_force, velocity);
		if (cosang < 0.0f) {
			lg_vector v;
			v.proj_vec(velocity, &normal_force);
			velocity->add(&v);
		}
		
		obj->collision_wall((lg_phy_wall*)this, &normal_force);		
	}
}

/*******************************************/

void lg_phy_wall_line_allowDirection::set_allowed(float i, float j)
{
	this->allowed.set(i, j, 0.0f);
}

void lg_phy_wall_line_allowDirection::check_coll (lg_physics_obj *obj)
{
	lg_vector normal_force, v, projection, *force_handler, *velocity;
	lg_point wall_start, obj_pos;
	int w, h;
	float cosang;
	lg_rect coll_area;
	lg_vector repul, diag;

	obj->get_forces_sum_handler(&force_handler);
		
	if (this->check_coll_rect(obj, &coll_area)) {
		int w, h;
					
		this->get_perpendicular(&normal_force);

		// find the direction of normal force
		obj->get_size(&w, &h);
		obj->get_pos(&obj_pos);
		v.set( (float)(w/2), (float)(h/2), 0.0f );
		obj_pos.add(&v);
		this->l.get_point(&wall_start);
		v.set_bypoint(&wall_start, &obj_pos); // v start at wall and ends in the object
		cosang = lg_vector::cos_ang(&v, &normal_force);
		if (cosang < 0.0f)
			normal_force.multiply(-1.0f);
				
		cosang = lg_vector::cos_ang(force_handler, &normal_force);
		if (cosang < 0.0f) {
			projection.proj_vec(force_handler, &normal_force);
			force_handler->add(&projection);
		}

		obj->get_speed_handler(&velocity);
		cosang = lg_vector::cos_ang(&this->allowed, velocity);
		if (cosang >= 0.0f)
			return;
	
		obj->get_speed_handler(&velocity);
		cosang = lg_vector::cos_ang(&normal_force, velocity);
		if (cosang < 0.0f) {
			lg_vector v;
			v.proj_vec(velocity, &normal_force);
			velocity->add(&v);
		}
		
		obj->collision_wall((lg_phy_wall*)this, &normal_force);
	}
}

/***********************************************/

lg_phy_wall_polygon::lg_phy_wall_polygon()
{
	this->status = 0;
}

lg_phy_wall_polygon::~lg_phy_wall_polygon()
{
	this->destroy();
}

void lg_phy_wall_polygon::start()
{
	this->status = 1;
}

void lg_phy_wall_polygon::add_node (int x, int y)
{
	if (this->status == 1)
		this->status = 2;
	else if (this->status == 2) {
		lg_phy_wall_line_stopAll *w = new lg_phy_wall_line_stopAll;
		w->set(this->lastx, this->lasty, x, y, this->sens);
		w->set_id(this->id);

		this->lines.add_end(w);
	}
	this->lastx = x;
	this->lasty = y;
}

void lg_phy_wall_polygon::end()
{
	this->status = 0;
}

void lg_phy_wall_polygon::destroy()
{
	this->lines.destroy();
}

void lg_phy_wall_polygon::check_coll (lg_physics_obj *obj)
{
	lst_wall_line_sa_el *l;
	lg_phy_wall_line_stopAll *w;
	
	for (l=this->lines.get_first(); l; l=l->get_next()) {
		w = l->get_data();
		w->check_coll(obj);
	}
}

/***********************************************/

lg_physics_scene::lg_physics_scene(): lg_scene()
{

}

lg_physics_scene::~lg_physics_scene()
{
	this->p_objs.destroy();
	this->walls.destroy();
	this->forces.destroy();
}

void lg_physics_scene::add_physics_obj(void *obj_v)
{
	this->p_objs.add_end( (lg_physics_obj*)obj_v );
	this->add_object(obj_v);
}

void lg_physics_scene::add_wall (void *wall)
{
	this->walls.add_end( (lg_phy_wall*)wall );
}

void lg_physics_scene::add_force (void *force_v)
{
	this->forces.add_end( (lg_phy_force*)force_v );
}

void lg_physics_scene::process_physics(float time)
{
	lst_physics_objs_el *l, *l2;
	lg_physics_obj *obj, *obj2, *first;
	lst_forces_el *lf;
	lg_phy_force *force;
	lst_wall_el *lwall;
	lg_phy_wall *wall;
	lg_vector *force_handler, result;

/*	lg_color cc;
	cc.r = 255; cc.g=0; cc.b=0;
	for (wall=this->walls; wall; wall=wall->next) {
		lg_gldraw_line(wall->start_x, wall->start_y, wall->end_x, wall->end_y, &cc);
	}*/

	l = this->p_objs.get_list();
	
	if (l == NULL)
		return;

	first = l->get_data();

	do {
		// process the forces list
		obj = l->get_data();
		obj->get_forces_sum_handler(&force_handler);
		force_handler->set(0.0f, 0.0f, 0.0f);
		for (lf=this->forces.get_first(); lf!=NULL; lf=lf->get_next()) {
			force = lf->get_data();
			force->calc_force( obj, &result );
			force_handler->add(&result);
		}

		// process the collisions between objects
		for (l2 = l->get_next(); l2!=l; l2=l2->get_next()) {
			obj2 = l2->get_data();
			if (obj_check_coll_pixel(obj, obj2))
				obj->collision_obj(obj2);
		}
			
		// process the collisions between objets and walls
		for (lwall=this->walls.get_first(); lwall!=NULL; lwall=lwall->get_next()) {
			wall = lwall->get_data();
			wall->check_coll(obj);
		}
		
		// update the position and velocity
		obj->update_pos(time);
		obj->update_velocity(time);

		l = l->get_next();
	} while(obj != first);
}

/*************************************/

void lg_phy_force_const::set(lg_vector *direction, float module)
{
	this->direction.copy(direction);
	this->module = module;
	this->direction.set_module(module);
}

void lg_phy_force_const::calc_force (lg_physics_obj *obj, lg_vector *result)
{
	float mass;
	mass = obj->get_mass();
	result->copy(&this->direction);
	result->multiply(mass);
}

/****************************************/

void object_remove_coll (lg_physics_obj *obj, lg_vector *coll_vector)
{
	lg_point pos;
	
	obj->get_pos(&pos);
	pos.add(coll_vector);
	obj->set_pos(&pos);
}

