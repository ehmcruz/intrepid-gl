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

#ifndef _LIB_GAME_AG
#define _LIB_GAME_AG

class lg_point;

class lg_vector
{
	friend class lg_point;
	protected:
		float i;
		float j;
		float k;
	public:
		lg_vector();
	
		void copy (lg_vector *v);
		void set (float ii, float jj, float kk);
		void get (float *ii, float *jj, float *kk);
		void set_bypoint (lg_point *src, lg_point *dest);
		void set_module (float mod);
		void unit ();
		void add (lg_vector *v);
		void subtract (lg_vector *v);
		void multiply (float n);
		void divide (float n);
		float module ();

		static float prod_int (lg_vector *v1, lg_vector *v2);
		static float cos_ang (lg_vector *v1, lg_vector *v2);
		void proj_vec (lg_vector *src, lg_vector *dest);
		void prod_vec (lg_vector *v1, lg_vector *v2);
};

class lg_point
{
	friend class lg_vector;
	protected:
		float x;
		float y;
		float z;
	public:
		lg_point();
	
		void copy (lg_point *p);
		void set (float xx, float yy, float zz);
		void get (float *xx, float *yy, float *zz);

		// overload
		void add (lg_vector *v);
		void subtract (lg_vector *v);
		void add (lg_vector *v, float alfa);
		void subtract (lg_vector *v, float alfa);
};

class lg_line
{
	protected:
		lg_point p;
		lg_vector v;
	public:
		lg_line();
	
		void copy (lg_line *l);

		// overload
		void set_point (float xx, float yy, float zz);
		void set_vector (float ii, float jj, float kk);
		void set_point (lg_point *p);
		void set_vector (lg_vector *v);

		void get_point (lg_point *p);
		void get_vector (lg_vector *v);
};

float dist_ptop (lg_point *p1, lg_point *p2);
bool line_intersection(lg_line *l1, lg_line *l2, lg_point *result);

#endif
