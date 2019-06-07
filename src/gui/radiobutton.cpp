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

#include "../../header/gui/radiobutton.h"
#include <stdlib.h>
#include <string.h>

lg_radioClass::~lg_radioClass()
{
	this->rlist.destroy();
}

void lg_radioClass::set(lg_radiobutton_class_param *p)
{
	memcpy(&this->param, p, sizeof(lg_radiobutton_class_param));
}

void lg_radioClass::add(lg_radiobutton *button, lg_radiobutton_param *bp)
{
	lg_checkbox_param p;

	p.flags = this->param.flags;
	p.size = this->param.size;
	p.thickness = this->param.thickness;
	p.bmp = this->param.bmp;
	p.pack = this->param.pack;
	lg_cp_color(&p.bg_color, &this->param.bg_color);
	lg_cp_color(&p.border_color, &this->param.border_color);
	lg_cp_color(&p.transp_color, &this->param.transp_color);

	memcpy(&p.obj, &bp->obj, sizeof(lg_object_param));
	
	button->set_checkbox(&p);
	this->rlist.add_end(button);
}

void lg_radioClass::change(lg_radiobutton *clicked)
{
	lst_radiobutton_el *l;
	lg_radiobutton *b;

	for (l=this->rlist.get_first(); l!=NULL; l=l->get_next()) {
		b = l->get_data();
		if (b->get_id() != clicked->get_id())
			b->set_checked(0);
	}
}

void lg_radioClass::uncheck_all()
{
	lst_radiobutton_el *l;
	lg_radiobutton *b;

	for (l=this->rlist.get_first(); l!=NULL; l=l->get_next()) {
		b = l->get_data();
		b->set_checked(0);
	}
}

/***************************************/

lg_radiobutton::lg_radiobutton() : lg_checkbox()
{
	this->my_class = NULL;
}

void lg_radiobutton::set_radio(lg_radiobutton_param *p, lg_radioClass *radio_class)
{
	this->value = p->value;
	this->my_class = radio_class;
	radio_class->add(this, p);
}

void lg_radiobutton::change()
{
	this->checked = 1;
	this->my_class->change(this);
	if (this->check_flags & RENDER_IMAGE) 
		this->s->set_state( (int) this->checked);
}

