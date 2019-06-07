#include "../../header/video/opengl.h"

lg_opengl_point::lg_opengl_point(): lg_video_handler()
{
	this->size = 1.0f;
	this->round = false;
	this->w = this->h = 1;
}

int lg_opengl_point::render()
{
	int x, y;
	this->my_obj->get_pos(&x, &y);
	lg_gldraw_point(x, y, &this->color, size, (int)round);
}

/************************/

lg_opengl_polygon::lg_opengl_polygon(): lg_video_handler()
{

}

/************************/

lg_opengl_solidRectangle::lg_opengl_solidRectangle() : lg_opengl_polygon()
{
}

int lg_opengl_solidRectangle::render()
{
	int x, y;
	lg_rect rectangle;
	this->my_obj->get_pos(&x, &y);
	rectangle.x0 = x;
	rectangle.y0 = y;
	rectangle.xf = x + this->w;
	rectangle.yf = y + this->h;
	lg_gldraw_solidRect(&rectangle, &this->back);
}

/************************/

lg_opengl_Rectangle::lg_opengl_Rectangle() : lg_opengl_polygon()
{
}

int lg_opengl_Rectangle::render()
{
	int x, y;
	lg_rect rectangle;
	this->my_obj->get_pos(&x, &y);
	rectangle.x0 = x;
	rectangle.y0 = y;
	rectangle.xf = x + this->w;
	rectangle.yf = y + this->h;
	lg_gldraw_Rect(&rectangle, &this->border);
}

/************************/

lg_opengl_border_rectangle::lg_opengl_border_rectangle() : lg_opengl_polygon()
{
	this->border_thickness = 1;
}

int lg_opengl_border_rectangle::render()
{
	int x, y;
	lg_rect rectangle;
	
	this->my_obj->get_pos(&x, &y);
	
	rectangle.x0 = x;
	rectangle.y0 = y;
	rectangle.xf = x + this->w;
	rectangle.yf = y + this->h;
	lg_gldraw_solidRect(&rectangle, &this->border);

	rectangle.x0 += this->border_thickness;
	rectangle.y0 += this->border_thickness;
	rectangle.xf -= this->border_thickness;
	rectangle.yf -= this->border_thickness;
	lg_gldraw_solidRect(&rectangle, &this->back);
}

/************************/

lg_opengl_solidCirc::lg_opengl_solidCirc() : lg_opengl_polygon()
{
}

int lg_opengl_solidCirc::render()
{
	int x, y;
	this->my_obj->get_pos(&x, &y);
	lg_gldraw_solidCirc(x, y, this->r, &this->back);
}

/************************/

lg_opengl_border_circ::lg_opengl_border_circ() : lg_opengl_polygon()
{
}

int lg_opengl_border_circ::render()
{
	int x, y;
	this->my_obj->get_pos(&x, &y);
	lg_gldraw_solidCirc(x, y, this->r, &this->border);	
	lg_gldraw_solidCirc(x + this->border_thickness, y + this->border_thickness, this->r - this->border_thickness, &this->back);
}

