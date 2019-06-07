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
#include "../../header/object/phys_object.h"

lg_physics_obj::lg_physics_obj():lg_object()
{
	this->coll_checked = false;
}

void lg_physics_obj::set_phy(lg_phyobject_param *p)
{
	this->mass = p->mass;
	this->shape = p->shape;
	this->set(&p->obj, NULL);
}

void lg_physics_obj::update_pos(float time)
{
	lg_vector delta_s, delta2;
	float t2;

	// delta_s = (V_ini * t) + (a * t^2)/2
	
	t2 = time*time;
	t2 /= 2;
	
	delta_s.copy(&this->velocity);
	delta_s.multiply(time);

	delta2.copy(&this->forces_sum);
	delta2.add(&this->my_force);
	delta2.divide(this->mass); // now i got acelleration
	delta2.multiply(t2);

	delta_s.add(&delta2);			
	this->pos.add(&delta_s);
}

void lg_physics_obj::update_velocity(float time)
{
	lg_vector delta_v;

	// delta_v = a * t
	
	delta_v.copy(&this->forces_sum);
	delta_v.add(&this->my_force);
	delta_v.divide(this->mass); // now i got acelleration	
	delta_v.multiply(time);
	
	this->velocity.add(&delta_v);
}

