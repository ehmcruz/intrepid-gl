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
#include <math.h>
#include "../../header/common/common.h"

void lg_cp_color (lg_color *dest, lg_color *src)
{
	dest->r = src->r;
	dest->g = src->g;
	dest->b = src->b;
}

SDL_Surface * load_optimage (char *fname)
{
	SDL_Surface *tmp, *s;
	tmp = SDL_LoadBMP(fname);
	s = SDL_DisplayFormat(tmp);
	SDL_FreeSurface(tmp);
	return s;
}


Uint32 getpixel_1 (SDL_Surface *surface, int x, int y)
{ 
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x;
	return *p;
}

Uint32 getpixel_2 (SDL_Surface *surface, int x, int y)
{
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x*2;
	return *(Uint16 *)p;
}


Uint32 getpixel_3 (SDL_Surface *surface, int x, int y)
{
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x*3;
	if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
		return p[0] << 16 | p[1] << 8 | p[2];
	else
		return p[0] | p[1] << 8 | p[2] << 16;
}

Uint32 getpixel_4 (SDL_Surface *surface, int x, int y)
{
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x*4;
	return *(Uint32 *)p;
}

void map_pixels (char *map, SDL_Surface *s, int x0, int y0, int xf, int yf, int sensitivity, lg_color *transp, float qfactor)
{
	int x, y, p, q, z, w, m1, m2;
	int minq;
	int trans;
	Uint32 (*getpixel)(SDL_Surface *surface, int x, int y);
	int bpp = s->format->BytesPerPixel;
	trans = SDL_MapRGB(s->format, transp->r, transp->g, transp->b);

	switch (bpp) {
		case 1:
			getpixel = getpixel_1;
			break;
		case 2:
			getpixel = getpixel_2;
			break;
		case 3:
			getpixel = getpixel_3;
			break;
		case 4:
			getpixel = getpixel_4;
	}
	
	if (qfactor == 0.0f)
		minq = 1;
	else {
		float total;
		total = (float)(sensitivity*sensitivity);
		minq = (int)( total * qfactor );
		if (minq < 1)
			minq = 1;
	}
	for (y=y0; y<yf; y+=sensitivity) {
		for (x=x0; x<xf; x+=sensitivity) {
			q = 0;
			m1 = x+sensitivity;
			if (m1 > xf)
				m1 = xf;
			m2 = y+sensitivity;
			if (m2 > yf)
				m2 = yf;
			for (z=x; z<m1; z++)
				for (w=y; w<m2; w++) {
					p = (*getpixel)(s, z, w);
					if (p!=trans)
						q++;
				}
			
			if (q < minq)
				*map = 0;
			else
				*map = 1;
			map++;
		}
	}
}

int coll_detection_pixel (char *m1, int x1_0, int y1_0, int x1_f, int y1_f,
                          char *m2, int x2_0, int y2_0, int x2_f, int y2_f, int sensitivity)
{
	char test;
	int startx, starty, endx, endy;
	int m1_startx, m1_starty, m1_endx, m1_endy;
	int m2_startx, m2_starty, m2_endx, m2_endy;
	int i, j, w1, h1, w2, h2, w, h;
	int m1_coord, m2_coord;
	int lpos1, lpos2;

	if (x1_0 < x2_0) {
		if (x1_f >= x2_0) { // x collision
			startx = x2_0;
			if (x1_f < x2_f)
				endx = x1_f;
			else
				endx = x2_f;
		}
		else // no collision
			return 0;
	}
	else {
		if (x2_f >= x1_0) { // x collision
			startx = x1_0;
			if (x2_f < x1_f)
				endx = x2_f;
			else
				endx = x1_f;
		}
		else // no collision
			return 0;
	}

	if (y1_0 < y2_0) {
		if (y1_f >= y2_0) { // y collision
			starty = y2_0;
			if (y1_f < y2_f)
				endy = y1_f;
			else
				endy = y2_f;
		}
		else // no collision
			return 0;
	}
	else {
		if (y2_f >= y1_0) { // y collision
			starty = y1_0;
			if (y2_f < y1_f)
				endy = y2_f;
			else
				endy = y1_f;
		}
		else // no collision
			return 0;
	}

	// now we know startxy and endxy of collision
	m1_startx = (startx - x1_0) / sensitivity;
	m1_starty = (starty - y1_0) / sensitivity;
	m1_endx = (endx - x1_0) / sensitivity;
	m1_endy = (endy - y1_0) / sensitivity;

	m2_startx = (startx - x2_0) / sensitivity;
	m2_starty = (starty - y2_0) / sensitivity;
	m2_endx = (endx - x2_0) / sensitivity;
	m2_endy = (endy - y2_0) / sensitivity;

	w1 = (x1_f - x1_0) / sensitivity;
	h1 = (y1_f - y1_0) / sensitivity;
	w2 = (x2_f - x2_0) / sensitivity;
	h2 = (y2_f - y2_0) / sensitivity;
	
	if ((x1_f - x1_0) % sensitivity)
		w1++;
	if ((y1_f - y1_0) % sensitivity)
		h1++;
	if ((x2_f - x2_0) % sensitivity)
		w2++;
	if ((y2_f - y2_0) % sensitivity)
		h2++;

	w = (endx - startx) / sensitivity;
	h = (endy - starty) / sensitivity;
	
	for (i=0; i<h; i++) {
		lpos1 = w1*(i+m1_starty) + m1_startx;
		lpos2 = w2*(i+m2_starty) + m2_startx;
		for (j=0; j<w; j++) {
			m1_coord = lpos1 + j;
			m2_coord = lpos2 + j;
			test = m1[m1_coord] & m2[m2_coord];
			if (test)
				return 1;
		}
	}

	return 0;
}

int coll_detection_rect (lg_rect *r1, lg_rect *r2, lg_rect *result)
{
	if (r1->x0 < r2->x0) {
		if (r1->xf >= r2->x0) { // x collision
			result->x0 = r2->x0;
			if (r1->xf < r2->xf)
				result->xf = r1->xf;
			else
				result->xf = r2->xf;
		}
		else // no collision
			return 0;
	}
	else {
		if (r2->xf >= r1->x0) { // x collision
			result->x0 = r1->x0;
			if (r2->xf < r1->xf)
				result->xf = r2->xf;
			else
				result->xf = r1->xf;
		}
		else // no collision
			return 0;
	}

	if (r1->y0 < r2->y0) {
		if (r1->yf >= r2->y0) { // y collision
			result->y0 = r2->y0;
			if (r1->yf < r2->yf)
				result->yf = r1->yf;
			else
				result->yf = r2->yf;
		}
		else // no collision
			return 0;
	}
	else {
		if (r2->yf >= r1->y0) { // y collision
			result->y0 = r1->y0;
			if (r2->yf < r1->yf)
				result->yf = r2->yf;
			else
				result->yf = r1->yf;
		}
		else // no collision
			return 0;
	}
	return 1;
}

int coll_detection_round (int center_x1, int center_y1, int r1,
                          int center_x2, int center_y2, int r2)
{
	int f1, f2;
	f1 = center_x1 - center_x2;
	f2 = center_y1 - center_y2;
	f1 *= f1;
	f2 *= f2;
	if (sqrt(f1+f2) <= r1+r2)
		return 1;
	return 0;
}

void rotate_map (char *map, char *rmap, int w_bitmap, int h_bitmap, float ang, int sens)
{
	#define MAP(X, Y)             map[Y*w + X]
	#define RMAP(X, Y)            rmap[Y*w + X]		
	#define ROTATE_X(X, Y)        (int) ((float)X*cos_v - (float)Y*sin_v)
	#define ROTATE_Y(X, Y)        (int) ((float)X*sin_v + (float)Y*cos_v)
	
	int x0, y0, transx, transy, rx, ry, x, y, tam, i;
	int w, h;
	float sin_v, cos_v;
	
	w = w_bitmap / sens;
	if (w_bitmap % sens)
		w++;
	h = h_bitmap / sens;
	if (h_bitmap % sens)
		h++;

	tam = w*h;
	for (i=0; i<tam; i++)
		rmap[i] = 0;
	
	x0 = w/2;
	y0 = h/2;
	sin_v = (float) sin( (double)ang );
	cos_v = (float) cos( (double)ang );

	for (y=0; y<h; y++) {
		for (x=0; x<w; x++) {
			transx = x-x0;
			transy = y-y0;
			rx = ROTATE_X(transx, transy);
			ry = ROTATE_Y(transx, transy);
			transx = rx+x0;
			transy = ry+y0;
			if (transx >= 0 && transx < w && transy >= 0 && transy < h)
				RMAP(transx, transy) = MAP(x, y);
		}
	}

	#undef MAP
	#undef RMAP
	#undef ROTATE_X
	#undef ROTATE_Y
}

void map_boundaries (SDL_Surface *s, int x0, int y0, int xf, int yf, lg_color *transp, lg_rect *rect)
{
	int x, y, p;
	int trans;
	Uint32 (*getpixel)(SDL_Surface *surface, int x, int y);
	int bpp = s->format->BytesPerPixel;
	trans = SDL_MapRGB(s->format, transp->r, transp->g, transp->b);

	switch (bpp) {
		case 1:
			getpixel = getpixel_1;
			break;
		case 2:
			getpixel = getpixel_2;
			break;
		case 3:
			getpixel = getpixel_3;
			break;
		case 4:
			getpixel = getpixel_4;
	}

	xf--;
	yf--;

	// from left to right
	rect->x0 = x0;
	for (x = x0	; x <= xf ; x++)
		for (y = y0 ; y <= yf ; y++){
			p = (*getpixel)(s, x, y);
			if (p != trans){
				rect->x0 = x;
				y = yf;
				x = xf;
			}
		}

	// from right to left
	rect->xf = xf;
	for (x = xf	; x >= x0 ; x--)
		for (y = yf ; y >= y0 ; y--){
			p = (*getpixel)(s, x, y);
			if (p != trans){
				rect->xf = x;
				y = y0;
				x = x0;
			}
		}

	// from top to bottom
	rect->y0 = y0;
	for (y = y0	; y <= yf ; y++)
		for (x = x0 ; x <= xf ; x++){
			p = (*getpixel)(s, x, y);
			if (p != trans){
				rect->y0 = y;
				y = yf;
				x = xf;
			}
		}

	// from the bottom up.
	rect->yf = yf;
	for (y = yf	; y >= y0 ; y--)
		for (x = xf ; x >= x0 ; x--){
			p = (*getpixel)(s, x, y);
			if (p != trans){
				rect->yf = y;
				y = y0;
				x = x0;
			}
		}

}

/******************/

int check_intersection (int start1, int end1, int start2, int end2, int *result_start, int *result_end)
{
	int coll;
	
	if (start1 < start2) {
		if (end1 < start2)
			coll = 0;
		else if (end1 < end2) {
			*result_start = start2;
			*result_end = end1;
			coll = 1;
		}
		else {
			*result_start = start2;
			*result_end = end2;
			coll = 1;
		}
	}
	else {
		if (end2 < start1)
			coll = 0;
		else if (end2 < end1) {
			*result_start = start1;
			*result_end = end2;
			coll = 1;
		}
		else {
			*result_start = start1;
			*result_end = end1;
			coll = 1;
		}
	}

	return coll;
}
