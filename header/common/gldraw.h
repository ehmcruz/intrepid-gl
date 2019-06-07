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

#ifndef _LIB_GAME_GLDRAW
#define _LIB_GAME_GLDRAW

#include "../SDL.h"
#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

void lg_gldraw_solidCirc (int x, int y, int r, lg_color *c);
void lg_gldraw_Circ (int x, int y, int r, lg_color *c);
void lg_gldraw_solidRect (lg_rect *rect, lg_color *c);
void lg_gldraw_Rect (lg_rect *rect, lg_color *c);
void lg_gldraw_line (int x0, int y0, int xf, int yf, lg_color *c);
void lg_gldraw_point (int x, int y, lg_color *c, float thickness, int round);
void render_tex_map (char *map, int sx, int sy, int w, int h, int sens, lg_color *c);

#ifdef __cplusplus
}
#endif

#endif
