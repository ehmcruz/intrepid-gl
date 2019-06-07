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
#include "../../header/common/glbitmap.h"
#include "../../header/common/common.h"

int power_of_two(int input);

int power_of_two(int input)
{
	int value = 1;

	while ( value < input )
		value <<= 1;
	
	return value;
}

lg_glbitmap::~lg_glbitmap()
{
	this->destroy();
}

void lg_glbitmap::destroy()
{
	glDeleteTextures( 1, &this->id );
}


void lg_glbitmap::create_texture(SDL_Surface  *sourceSurface, int flags, 
			Uint8 colorKeyRed, Uint8 colorKeyGreen, Uint8 colorKeyBlue)
{
	Uint32 colorkey;
	SDL_Surface *alphaImage;
	int w2, h2;
	GLuint textureID;
	SDL_Rect area;
	
	w2 = power_of_two(sourceSurface->w);
	h2 = power_of_two(sourceSurface->h);
	
	this->w = sourceSurface->w;
	this->h = sourceSurface->h;
	
	this->max_w = (float)this->w / (float)w2;
	this->max_h = (float)this->h / (float)h2;
	
	if (flags & LF_CO_TRANSPARENT)
		this->transparent = 1;
	else
		this->transparent = 0;

	/* Create the target alpha surface with correct color component ordering */
	alphaImage = SDL_CreateRGBSurface( SDL_SWSURFACE, w2,
	h2, 32 /* bits */,
	#if SDL_BYTEORDER == SDL_LIL_ENDIAN // OpenGL RGBA masks 
                               0x000000FF, 
                               0x0000FF00, 
                               0x00FF0000, 
                               0xFF000000
	#else
                               0xFF000000,
                               0x00FF0000, 
                               0x0000FF00, 
                               0x000000FF
	#endif
	);

	// Set up so that colorkey pixels become transparent :

	colorkey = SDL_MapRGBA( alphaImage->format, colorKeyRed, colorKeyGreen, colorKeyBlue, 0 ) ;
	SDL_FillRect( alphaImage, 0, colorkey ) ;

	colorkey = SDL_MapRGBA( sourceSurface->format, colorKeyRed, colorKeyGreen, colorKeyBlue, 0 ) ;
	SDL_SetColorKey( sourceSurface, SDL_SRCCOLORKEY, colorkey ) ;
	
 	// Copy the surface into the GL texture image : 
	area.x = 0 ;
	area.y = 0 ; 
	area.w = sourceSurface->w ;
	area.h = sourceSurface->h ;
	SDL_BlitSurface( sourceSurface, &area, alphaImage, &area ) ;

	// Create an OpenGL texture for the image 
	glGenTextures( 1, &textureID ) ;
	glBindTexture( GL_TEXTURE_2D, textureID ) ;

	/* Prepare the filtering of the texture image */
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ) ;
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST ) ;
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT ) ;
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT ) ;
	
	// Map the alpha surface to the texture
  	glTexImage2D( GL_TEXTURE_2D,
               0,
               GL_RGBA,
               w2, h2,
               0,
               GL_RGBA,
               GL_UNSIGNED_BYTE,
               alphaImage->pixels ) ;
	// No longer needed :
	SDL_FreeSurface( alphaImage ) ; 
 
	this->id = textureID ;
}

void lg_glbitmap::print(lg_rect *rect, int startx, int starty, float ang)
{
	#define PIXEL2GL_W(MAX, VALUE)          ( ( (float) VALUE / (float) MAX ) * this->max_w)
	#define PIXEL2GL_H(MAX, VALUE)          ( ( (float) VALUE / (float) MAX ) * this->max_h)

	float obj_x0, obj_y0, obj_xf, obj_yf, obj_w, obj_h;
	float tex_x0, tex_y0, tex_xf, tex_yf;

	obj_w = (float)( rect->xf - rect->x0 ) / 2.0f;
	obj_h = (float)( rect->yf - rect->y0 ) / 2.0f;

	obj_x0 = -1*obj_w;;
	obj_xf = obj_w;
	obj_y0 = -1*obj_h;
	obj_yf = obj_h;
	
	tex_x0 = PIXEL2GL_W(this->w, rect->x0);
	tex_xf = PIXEL2GL_W(this->w, rect->xf);
	tex_y0 = PIXEL2GL_H(this->h, rect->y0);
	tex_yf = PIXEL2GL_H(this->h, rect->yf);
	
	//fprintf(fp, "tex_xf=%f,\ntex_yf=%f,\n",tex_xf, tex_yf);
	
	glPushMatrix();
	glTranslatef( (float)startx+obj_w, (float)starty+obj_h, 0.0f );
	
	if (this->transparent)
	{
		glEnable( GL_BLEND );
    	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	if (ang != 0.0f)
		glRotatef( RADIANS_2_DEGREES(ang) , 0.0f, 0.0f, 1.0f);
	
	glEnable (GL_TEXTURE_2D); /* enable texture mapping */
    glBindTexture (GL_TEXTURE_2D, this->id); /* bind to our texture, has id of 13 */
  
    //Start quad
    glBegin( GL_QUADS );
    
    //Draw square
	glTexCoord2f (tex_x0, tex_y0); 
	glVertex3f(obj_x0, obj_y0, 0.0f);

	glTexCoord2f (tex_xf, tex_y0); 
	glVertex3f(obj_xf, obj_y0, 0.0f);

	glTexCoord2f (tex_xf,tex_yf); 
	glVertex3f(obj_xf, obj_yf, 0.0f);

    glTexCoord2f (tex_x0, tex_yf);
	glVertex3f(obj_x0, obj_yf, 0.0f );

    //End quad   
    glEnd();
	
	if (this->transparent)
		glDisable( GL_BLEND );
	
	glDisable( GL_TEXTURE_2D );
	glPopMatrix();
	#undef PIXEL2GL_W
	#undef PIXEL2GL_H	
}

