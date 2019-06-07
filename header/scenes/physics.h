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

#ifndef _LIB_GAME_PHYSICS
#define _LIB_GAME_PHYSICS

#include "../SDL.h"
#include "../data/simple_linked.h"
#include "../data/circ_linked.h"
#include "../common/ag.h"
#include "../common/common.h"
#include "../common/gldraw.h"
#include "scenes.h"

/****************************************/

class lg_physics_obj;

class lg_phy_wall
{
	protected:
		int id;
	public:
		virtual void check_coll (lg_physics_obj *obj) {  }
		
		inline void set_id(int id) {
			this->id = id;
		}
		
		inline int get_id() {
			return this->id;
		}
};

typedef lg_linked_list<lg_phy_wall> lst_wall;
typedef lg_linked_el<lg_phy_wall> lst_wall_el;

#include "../object/phys_object.h"

class lg_phy_wall_line: public lg_phy_wall
{
	protected:
		lg_line l;
		int start_x, start_y, end_x, end_y;
		int delta_value;
	public:
		void set(int startx, int starty, int endx, int endy, int sens);
		void get_perpendicular (lg_vector *result);
		bool check_coll_rect (lg_physics_obj *obj, lg_rect *result);

		virtual void check_coll (lg_physics_obj *obj) { }
};

/********************/

class lg_phy_wall_line_stopAll: public lg_phy_wall_line
{
	public:
		virtual void check_coll (lg_physics_obj *obj);
};

/********************/

class lg_phy_wall_line_allowDirection: public lg_phy_wall_line
{
	protected:
		lg_vector allowed;
	public:
		void set_allowed(float i, float j);
		virtual void check_coll (lg_physics_obj *obj);
};

/*****************************/

typedef lg_linked_list<lg_phy_wall_line_stopAll> lst_wall_line_sa;
typedef lg_linked_el<lg_phy_wall_line_stopAll> lst_wall_line_sa_el;

class lg_phy_wall_polygon: public lg_phy_wall
{
	protected:
		lst_wall_line_sa lines;
		int lastx, lasty;
		Uint8 status;
		int sens;
	public:
		lg_phy_wall_polygon();
		~lg_phy_wall_polygon();	
		void start();
		void end();
		void destroy();
		void add_node(int x, int y);
		virtual void check_coll (lg_physics_obj *obj);		

		inline void set_sens (int sens) {
			this->sens = sens;
		}
};

/*********************/

typedef lg_circ_linked_list<lg_physics_obj> lst_physics_objs;
typedef lg_circ_linked_el<lg_physics_obj> lst_physics_objs_el;

/****************************************/

class lg_phy_force
{
	public:
		virtual void calc_force (lg_physics_obj *obj, lg_vector *result) {  }
};

class lg_phy_force_const: public lg_phy_force
{
	protected:
		lg_vector direction;
		float module;
	public:
		void set(lg_vector *direction, float module);
		virtual void calc_force (lg_physics_obj *obj, lg_vector *result);
};

typedef lg_linked_list<lg_phy_force> lst_forces;
typedef lg_linked_el<lg_phy_force> lst_forces_el;

/****************************************/

class lg_physics_scene : public lg_scene
{
	protected:
		lst_wall walls;
		lst_physics_objs p_objs;
		lst_forces forces;

	public:
		lg_physics_scene();
		~lg_physics_scene();
		void add_physics_obj(void *obj_v);
		void add_force(void *force_v);
		void add_wall(void *wall);
		void process_physics(float time);
};

/******************************/

void object_remove_coll (lg_physics_obj *obj, lg_vector *coll_vector);

#endif
