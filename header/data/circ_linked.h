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

#ifndef _LIB_GAME_CIRCSIMPLELINKEDLIST
#define _LIB_GAME_CIRCSIMPLELINKEDLIST

#include <stdlib.h>

// Circular simple linked list

template <class T>
class lg_circ_linked_el {
	protected:
		T *data;
		lg_circ_linked_el<T> *next;

	public:
		inline T* get_data() {
			return this->data;
		}

		inline lg_circ_linked_el<T>* get_next() {
			return this->next;
		}

		inline void set_data(T *data) {
			this->data = data;
		}

		inline void set_next(lg_circ_linked_el<T> *next) {
			this->next = next;
		}
};

template <class T>
class lg_circ_linked_list
{
	protected:
		lg_circ_linked_el<T> *list;

	public:
		lg_circ_linked_list();
		~lg_circ_linked_list();
		void add_end(T *el);
		void add_beginning(T *el);
		void destroy();
		
		inline lg_circ_linked_el<T>* get_list() {
			return this->list;
		}
};

/********************************************************************/

template <class T>
lg_circ_linked_list<T>::lg_circ_linked_list ()
{
	this->list = NULL;
}

template <class T>
lg_circ_linked_list<T>::~lg_circ_linked_list ()
{
	this->destroy();
}

template <class T>
void lg_circ_linked_list<T>::destroy ()
{
	lg_circ_linked_el<T> *next, *first;
	
	if (this->list) {
		first = this->list;
		do {
			next = this->list->get_next();
			delete this->list;
			this->list = next;
		} while(this->list != first);
		this->list = NULL;
	}
}

template <class T>
void lg_circ_linked_list<T>::add_end (T *el)
{
	lg_circ_linked_el<T> *tmp;
	
	tmp = new lg_circ_linked_el<T>;
	
	if (!this->list) {
		tmp->set_data(el);		
		this->list = tmp;
		this->list->set_next(this->list);
	}
	else if (this->list == this->list->get_next()) {
		tmp->set_data(el);			
		this->list->set_next(tmp);
		tmp->set_next(this->list);
	}
	else {
		tmp->set_next( this->list->get_next() );
		tmp->set_data( this->list->get_data() );
		this->list->set_next(tmp);
		this->list->set_data(el);
		this->list = this->list->get_next();
	}
}

template <class T>
void lg_circ_linked_list<T>::add_beginning (T *el)
{
	lg_circ_linked_el<T> *tmp;
	
	tmp = new lg_circ_linked_el<T>;
	tmp->set_data(el);
	tmp->set_next(this->list);
	this->list = tmp;
}

#endif
