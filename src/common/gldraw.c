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
#include <SDL/SDL_opengl.h>

#include "../../header/common/common.h"
#include "../../header/common/gldraw.h"

#define LG_COLOR_TO_OPENGL(COLOR)          ((float)COLOR / 255.0f)

void lg_gldraw_solidCirc (int x, int y, int r, lg_color *c)
{
	float angle, vectorX, vectorY, vectorX1, vectorY1;
	float radius = (float)r;
	int i, ox, oy;

	ox = x + r;
	oy = y + r;

	vectorY1=(float)oy;
	vectorX1=(float)ox;
	
	glColor3f(LG_COLOR_TO_OPENGL(c->r), LG_COLOR_TO_OPENGL(c->g), LG_COLOR_TO_OPENGL(c->b));
	
	glBegin(GL_TRIANGLES);	
	for(i=0;i<=360;i++) {
		angle=(float)(((double)i)/57.29577957795135);	
		vectorX=(float)ox+(radius*(float)sin((double)angle));
		vectorY=(float)oy+(radius*(float)cos((double)angle));		
		glVertex2d((float)ox,(float)oy);
		glVertex2d(vectorX1,vectorY1);
		glVertex2d(vectorX,vectorY);
		vectorY1=vectorY;
		vectorX1=vectorX;	
	}
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);	
}

void lg_gldraw_Circ (int x, int y, int r, lg_color *c)
{
	float angle, vectorX, vectorY;
	float radius = (float)r;
	int ox, oy;
	
	ox = x + r;
	oy = y + r;

	glColor3f(LG_COLOR_TO_OPENGL(c->r), LG_COLOR_TO_OPENGL(c->g), LG_COLOR_TO_OPENGL(c->b));

	glBegin(GL_LINE_STRIP);			
	for(angle=0.0f;angle<=(2.0f*3.14159);angle+=0.01f) {
		vectorX=(float)ox+(radius*(float)sin((double)angle));
		vectorY=(float)oy+(radius*(float)cos((double)angle));		
		glVertex2d(vectorX,vectorY);			
	}
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);	
}

void lg_gldraw_solidRect (lg_rect *rect, lg_color *c)
{
    glBegin( GL_QUADS );
	glColor3f(LG_COLOR_TO_OPENGL(c->r), LG_COLOR_TO_OPENGL(c->g), LG_COLOR_TO_OPENGL(c->b));
    //Draw square
	glVertex3f(rect->x0, rect->y0, 0.0f);
	glVertex3f(rect->xf, rect->y0, 0.0f);
	glVertex3f(rect->xf, rect->yf, 0.0f);
	glVertex3f(rect->x0, rect->yf, 0.0f );
    //End quad   
    glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
}

void lg_gldraw_Rect (lg_rect *rect, lg_color *c)
{
	glBegin( GL_LINES );
	glColor3f(LG_COLOR_TO_OPENGL(c->r), LG_COLOR_TO_OPENGL(c->g), LG_COLOR_TO_OPENGL(c->b));

	glVertex3f(rect->x0, rect->y0, 0.0f);
	glVertex3f(rect->xf, rect->y0, 0.0f);
	
	glVertex3f(rect->xf, rect->y0, 0.0f);
	glVertex3f(rect->xf, rect->yf, 0.0f);

	glVertex3f(rect->xf, rect->yf, 0.0f);
	glVertex3f(rect->x0, rect->yf, 0.0f);

	glVertex3f(rect->x0, rect->yf, 0.0f);
	glVertex3f(rect->x0, rect->y0, 0.0f);
	
    glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
}

void lg_gldraw_line (int x0, int y0, int xf, int yf, lg_color *c)
{
	glBegin( GL_LINES );
	glColor3f(LG_COLOR_TO_OPENGL(c->r), LG_COLOR_TO_OPENGL(c->g), LG_COLOR_TO_OPENGL(c->b));
    //Draw line
	glVertex3f(x0, y0, 0.0f);
	glVertex3f(xf, yf, 0.0f);
    //End quad   
    glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
}

void lg_gldraw_point (int x, int y, lg_color *c, float thickness, int round)
{
	float oldsize;
	glGetFloatv(GL_POINT_SIZE, &oldsize);
	glPointSize(thickness);
	if ( round )
		glEnable( GL_POINT_SMOOTH );
	glBegin( GL_POINTS );
	glColor3f(LG_COLOR_TO_OPENGL(c->r), LG_COLOR_TO_OPENGL(c->g), LG_COLOR_TO_OPENGL(c->b));
	glVertex3f(x, y, 0.0f);
    glEnd();
	if ( round )
		glDisable( GL_POINT_SMOOTH );
	glColor3f(1.0f, 1.0f, 1.0f);
	glPointSize(oldsize);
}

void render_tex_map (char *map, int sx, int sy, int w, int h, int sens, lg_color *c)
{
	int x, y, wm, hm, lpos;

	wm = w / sens;
	if (w%sens) wm++;
	hm = h / sens;
	if (h%sens) hm++;
	
	glBegin( GL_POINTS );
	glColor3f(LG_COLOR_TO_OPENGL(c->r), LG_COLOR_TO_OPENGL(c->g), LG_COLOR_TO_OPENGL(c->b));

	for (y=0; y<hm; y++) {
		for (x=0; x<wm; x++) {
			lpos = y*wm;
			if (map[lpos + x])
				glVertex3f(x+sx, y+sy, 0.0f);
		}
	}
	
    glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
}

