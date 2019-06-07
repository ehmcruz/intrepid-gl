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

#include <math.h>
#include "../../header/common/ag.h"

lg_vector::lg_vector ()
{
	this->i = 0;
	this->j = 0;
	this->k = 0;
}

void lg_vector::copy (lg_vector *v)
{
	this->i = v->i;
	this->j = v->j;
	this->k = v->k;
}

void lg_vector::set (float ii, float jj, float kk)
{
	this->i = ii;
	this->j = jj;
	this->k = kk;
}

void lg_vector::get (float *ii, float *jj, float *kk)
{
	*ii = this->i;
	*jj = this->j;
	*kk = this->k;
}

void lg_vector::set_bypoint (lg_point *src, lg_point *dest)
{
	this->i = dest->x - src->x;
	this->j = dest->y - src->y;
	this->k = dest->z - src->z;
}

void lg_vector::set_module (float mod)
{
	this->divide( this->module() );
	this->multiply(mod);
}

void lg_vector::unit ()
{
	this->divide( this->module() );
}

void lg_vector::add (lg_vector *v)
{
	this->i += v->i;
	this->j += v->j;
	this->k += v->k;
}

void lg_vector::subtract (lg_vector *v)
{
	this->i -= v->i;
	this->j -= v->j;
	this->k -= v->k;
}

void lg_vector::multiply (float n)
{
	this->i *= n;
	this->j *= n;
	this->k *= n;
}

void lg_vector::divide (float n)
{
	this->i /= n;
	this->j /= n;
	this->k /= n;
}

float lg_vector::module ()
{
	float m;
	m = sqrt( lg_vector::prod_int(this, this) );	
	return m;
}

void lg_vector::proj_vec (lg_vector *src, lg_vector *dest)
{
	float p, m;
	
	p = lg_vector::prod_int(src, dest);
	if (p<0)
		p *= -1;
	m = dest->module();
	m *= m;
	p /= m;
	this->copy(dest);
	this->multiply(p);
}

void lg_vector::prod_vec (lg_vector *v1, lg_vector *v2)
{
	/*
	      i j k
	   v1 i j k
	   v2 i j k
	*/
	
	this->i = v1->j*v2->k - v2->j*v1->k;
	this->j = v2->i*v1->k - v1->i*v2->k;
	this->k = v1->i*v2->j - v2->i*v1->j;
}

float lg_vector::prod_int (lg_vector *v1, lg_vector *v2)
{
	float p;
	p = v1->i*v2->i + v1->j*v2->j + v1->k*v2->k;
	return p;
}

float lg_vector::cos_ang (lg_vector *v1, lg_vector *v2)
{
	float p;
	p = lg_vector::prod_int(v1, v2);
	p /= v1->module();
	p /= v2->module();
	return p;
}

/*************************************************/

lg_point::lg_point ()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

void lg_point::copy (lg_point *p)
{
	this->x = p->x;
	this->y = p->y;
	this->z = p->z;
}

void lg_point::set (float xx, float yy, float zz)
{
	this->x = xx;
	this->y = yy;
	this->z = zz;
}

void lg_point::get (float *xx, float *yy, float *zz)
{
	*xx = this->x;
	*yy = this->y;
	*zz = this->z;
}

// add and subtract are overloaded

void lg_point::add (lg_vector *v)
{
	this->x += v->i;
	this->y += v->j;
	this->z += v->k;
}

void lg_point::subtract (lg_vector *v)
{
	this->x -= v->i;
	this->y -= v->j;
	this->z -= v->k;
}

void lg_point::add (lg_vector *v, float alfa)
{
	this->x += v->i*alfa;
	this->y += v->j*alfa;
	this->z += v->k*alfa;
}

void lg_point::subtract (lg_vector *v, float alfa)
{
	this->x -= v->i*alfa;
	this->y -= v->j*alfa;
	this->z -= v->k*alfa;
}

/*************************************************/

lg_line::lg_line ()
{
	this->p.set(0, 0, 0);
	this->v.set(0, 0, 0);
}

void lg_line::copy (lg_line *l)
{
	this->p.copy(&l->p);
	this->v.copy(&l->v);
}

// overload

void lg_line::set_point (float xx, float yy, float zz)
{
	this->p.set(xx, yy, zz);
}

void lg_line::set_vector (float ii, float jj, float kk)
{
	this->v.set(ii, jj, kk);
}

void lg_line::set_point (lg_point *p)
{
	this->p.copy(p);
}

void lg_line::set_vector (lg_vector *v)
{
	this->v.copy(v);
}

void lg_line::get_point (lg_point *p)
{
	p->copy(&this->p);
}

void lg_line::get_vector (lg_vector *v)
{
	v->copy(&this->v);
}

/*************************************************/

float dist_ptop (lg_point *p1, lg_point *p2)
{
	float p1_x, p1_y, p1_z, p2_x, p2_y, p2_z, a1, a2, a3, d;

	p1->get(&p1_x, &p1_y, &p1_z);
	p2->get(&p2_x, &p2_y, &p2_z);

	a1 = p1_x - p2_x;
	a1 *= a1;

	a2 = p1_y - p2_y;
	a2 *= a2;

	a3 = p1_z - p2_z;
	a3 *= a3;

	d = sqrt(a1 + a2 + a3);
	return d;
}
