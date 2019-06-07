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

#include "../../header/gui/menu.h"

lg_menu::lg_menu() : lg_scene()
{
	this->focused = NULL;
}

lg_menu::~lg_menu()
{
	this->input.destroy();
}

void lg_menu::add_input(void *obj)
{
	this->input.add_end( (lg_input_obj*)obj );
	this->add_object(obj);
}

lg_input_obj* lg_menu::get_inputobj_bypos (int posx, int posy)
{
	lst_inObj_el *o;
	lg_input_obj *obj;
	lg_input_obj *id;
	
	id = NULL;
	o = this->input.get_last();
	while (o!=NULL) {
		obj = o->get_data();//printf("%i\n", obj->get_id());
		if (obj->check_if_inRange(posx, posy)) {
			id = obj;
			o = NULL;
		}
		else
			o = o->get_prev();
	}
	return id;
}

void lg_menu::process(Uint8 *keyboard, int mx, int my, SDL_Event *event)
{
	lg_input_obj *mouseObj, *obj, *lastFocused;
	lst_inObj_el *l;
	int mouse_objid;
	int mx_obj, my_obj, obj_x, obj_y;

	SDL_MouseButtonEvent *mbutton;

	// first, process the on_mouse_over and on_mouse_out events
	mouseObj = this->get_inputobj_bypos(mx, my);
	if (mouseObj) {
		mouseObj->get_pos( &obj_x, &obj_y );
		mx_obj = mx - obj_x;
		my_obj = my - obj_y;
		mouseObj->on_mouse_over(mx_obj, my_obj);

		mouse_objid = mouseObj->get_id();//printf("%i\n", mouse_objid);
//		printf("%i\n", l->->get_id());
		for (l=this->input.get_first(); l!=NULL; l=l->get_next()) {
			obj = l->get_data();//printf("%i\n", obj->get_id());
			if (obj->get_id() != mouse_objid)
				obj->on_mouse_out(mx, my);
		}
	}
	else {
		for (l=this->input.get_first(); l!=NULL; l=l->get_next()) {
			obj = l->get_data();
			obj->on_mouse_out(mx, my);
		}
	}

	// process other events
	
	switch (event->type) {
			
		// pressed a key
		case SDL_KEYDOWN:
			for (l=this->input.get_first(); l!=NULL; l=l->get_next()) {
				obj = l->get_data();
				obj->on_keyb_press(keyboard);
			}
			break;

		// mouse click
		case SDL_MOUSEBUTTONDOWN:
			mbutton = &event->button;
			lastFocused = this->get_focused();		
			this->set_focused(mouseObj);
			
			if (mouseObj) {
				if (lastFocused != mouseObj) {
					if (lastFocused)
						lastFocused->unset_focus();
					mouseObj->set_focus();
				}
//printf("%i\n", mouse_objid);
				for (l=this->input.get_first(); l!=NULL; l=l->get_next()) {
					obj = l->get_data();//printf("%i\n", obj->get_id());
					if (obj->get_id() != mouse_objid)
						obj->on_mouse_click_outDown( mbutton->button, mx, my );
					else {//printf("%i\n", mouse_objid);
						obj->get_pos( &obj_x, &obj_y );
						mx_obj = mx - obj_x;
						my_obj = my - obj_y;
						obj->on_mouse_click_inDown( mbutton->button, mx_obj, my_obj );
					}
				}
			}
			else {
				if (lastFocused)
					lastFocused->unset_focus();

				for (l=this->input.get_first(); l!=NULL; l=l->get_next()) {
					obj = l->get_data();
					obj->on_mouse_click_outDown( mbutton->button, mx, my );
				}
			}
			break;

		// mouse unclick
		case SDL_MOUSEBUTTONUP:
			mbutton = &event->button;
			
			if (mouseObj) {
				for (l=this->input.get_first(); l!=NULL; l=l->get_next()) {
					obj = l->get_data();
					if (obj->get_id() != mouse_objid)
						obj->on_mouse_click_outUp( mbutton->button, mx, my );
					else {
						obj->get_pos( &obj_x, &obj_y );
						mx_obj = mx - obj_x;
						my_obj = my - obj_y;
						obj->on_mouse_click_inUp( mbutton->button, mx_obj, my_obj );
					}
				}
			}
			else {
				for (l=this->input.get_first(); l!=NULL; l=l->get_next()) {
					obj = l->get_data();
					obj->on_mouse_click_outUp( mbutton->button, mx, my );
				}
			}
			break;
	}
}

