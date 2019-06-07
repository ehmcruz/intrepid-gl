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

#ifndef _LIB_GAME_OPENGLVH
#define _LIB_GAME_OPENGLVH

#include "../SDL.h"

#include "../object/object.h"
#include "../common/gldraw.h"
#include "../common/common.h"

class lg_object;
class lg_video_handler;

/***************************/

class lg_opengl_point: public lg_video_handler
{
	protected:
		lg_color color;
		float size;
		bool round;
		
	public:
		lg_opengl_point();
		
		virtual int render ();
		
		inline void set_thickness(float size) {
			this->size = size;
		}

		inline void set_color(lg_color *color) {
			lg_cp_color(&this->color, color);
		}

		inline void set_roundness(bool round) {
			this->round = round;
		}
};

/***************************/

class lg_opengl_polygon: public lg_video_handler
{
	protected:
		lg_color border, back;
		int border_thickness;

	public:
		lg_opengl_polygon();

		inline void set_border_color(lg_color *color) {
			lg_cp_color(&this->border, color);
		}

		inline void set_back_color(lg_color *color) {
			lg_cp_color(&this->back, color);
		}
		
		inline void get_border_color(lg_color *color) {
			lg_cp_color(color, &this->border);
		}

		inline void get_back_color(lg_color *color) {
			lg_cp_color(color, &this->back);
		}

		inline void set_thickness(int t) {
			this->border_thickness = t;
		}
		
		inline int get_thickness() {
			return this->border_thickness;
		}
};

/***************************/

class lg_opengl_solidRectangle: public lg_opengl_polygon
{		
	public:
		lg_opengl_solidRectangle();
		
		virtual int render ();

		inline void set_w(int w) {
			this->w = w;
		}
		
		inline void set_h(int h) {
			this->h = h;
		}
};
/***************************/

class lg_opengl_Rectangle: public lg_opengl_polygon
{	
	public:
		lg_opengl_Rectangle();
		
		virtual int render ();

		inline void set_w(int w) {
			this->w = w;
		}
		
		inline void set_h(int h) {
			this->h = h;
		}
};

/***************************/

class lg_opengl_border_rectangle: public lg_opengl_polygon
{
	public:
		lg_opengl_border_rectangle();
		
		virtual int render ();

		inline void set_w(int w) {
			this->w = w;
		}
		
		inline void set_h(int h) {
			this->h = h;
		}
};

/***************************/

class lg_opengl_solidCirc: public lg_opengl_polygon
{
	protected:
		int r;
		
	public:
		lg_opengl_solidCirc();
		
		virtual int render ();

		inline void set_r(int r) {
			this->r = r;
			this->w = this->h = 2*r;
		}
};

/***************************/

class lg_opengl_border_circ: public lg_opengl_polygon
{
	protected:
		int r;
		
	public:
		lg_opengl_border_circ();
		
		virtual int render ();
	
		inline void set_r(int r) {
			this->r = r;
			this->w = this->h = 2*r;
		}
};

#endif
