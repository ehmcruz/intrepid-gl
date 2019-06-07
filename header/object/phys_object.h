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

#include "../SDL.h"
#include "../common/common.h"
#include "../common/ag.h"
#include "object.h"
#include "../scenes/physics.h"

#ifndef _LIB_GAME_PHYS_OBJECT
#define _LIB_GAME_PHYS_OBJECT

#define LG_SHAPE_CIRC       0x0001
#define LG_SHAPE_RECT       0x0002
#define LG_SHAPE_PIXEL      0x0004

struct lg_phyobject_paramstc {
	lg_object_param obj;
	float mass;
	int shape;
};

typedef struct lg_phyobject_paramstc lg_phyobject_param;

class lg_physics_obj : public lg_object
{
	protected:
		lg_vector velocity;
		lg_vector my_force;
		lg_vector forces_sum;
		float mass;
		int shape;
		bool coll_checked;
		
	public:
		lg_physics_obj();

		virtual void collision_wall (lg_phy_wall *wall, lg_vector *normal_force) { }
		virtual void collision_obj (lg_physics_obj *obj) { }

		void set_phy(lg_phyobject_param *p);
		void update_pos(float time);
		void update_velocity(float time);

		inline float get_mass() {
			return this->mass;
		}

		inline void get_speed_handler(lg_vector **v) {
			*v = &this->velocity;
		}

		inline void get_myforce_handler(lg_vector **v) {
			*v = &this->my_force;
		}

		inline void get_forces_sum_handler(lg_vector **v) {
			*v = &this->forces_sum;
		}
};

#endif
