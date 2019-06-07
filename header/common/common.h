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

#ifndef _LIB_GAME_COMMON
#define _LIB_GAME_COMMON

#include "../SDL.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LF_CO_TRANSPARENT		0x01
#define PI_VALUE      3.1415f

#define RADIANS_2_DEGREES(ANG)          ( (180.0f * ANG) / PI_VALUE )

struct lg_colorstc {
	Uint8 r, g, b;
};
typedef struct lg_colorstc lg_color;

struct stc_rect {
	Uint32 x0, y0, xf, yf; 
};

typedef struct stc_rect lg_rect;

SDL_Surface * load_optimage (char *fname);

Uint32 getpixel_1 (SDL_Surface *surface, int x, int y);
Uint32 getpixel_2 (SDL_Surface *surface, int x, int y);
Uint32 getpixel_3 (SDL_Surface *surface, int x, int y);
Uint32 getpixel_4 (SDL_Surface *surface, int x, int y);

//Uint32 getpixel(SDL_Surface *surface, int x, int y);
void map_pixels (char *map, SDL_Surface *s, int x0, int y0, int xf, int yf, int sensitivity, lg_color *transp, float qfactor);

void rotate_map (char *map, char *rmap, int w_bitmap, int h_bitmap, float ang, int sens);

void map_boundaries (SDL_Surface *s, int x0, int y0, int xf, int yf, lg_color *transp, lg_rect *rect);

int coll_detection_pixel (char *m1, int x1_0, int y1_0, int x1_f, int y1_f,
                          char *m2, int x2_0, int y2_0, int x2_f, int y2_f, int sensitivity);

int coll_detection_rect (lg_rect *r1, lg_rect *r2, lg_rect *result);

int coll_detection_round (int center_x1, int center_y1, int r1,
                          int center_x2, int center_y2, int r2);

int check_intersection (int start1, int end1, int start2, int end2, int *result_start, int *result_end);

void lg_cp_color (lg_color *dest, lg_color *src);

#ifdef __cplusplus
}
#endif

#endif
