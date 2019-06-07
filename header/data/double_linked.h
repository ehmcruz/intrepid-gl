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

#ifndef _LIB_GAME_DOUBLELINKEDLIST
#define _LIB_GAME_DOUBLELINKEDLIST

#include <stdlib.h>

// Double linked list

template <class T>
class lg_dblinked_el {
	protected:
		T *data;
		lg_dblinked_el<T> *prev, *next;

	public:
		inline T* get_data() {
			return this->data;
		}

		inline lg_dblinked_el<T>* get_next() {
			return this->next;
		}

		inline lg_dblinked_el<T>* get_prev() {
			return this->prev;
		}

		inline void set_data(T* data) {
			this->data = data;
		}

		inline void set_next (lg_dblinked_el<T> *next) {
			this->next = next;
		}

		inline void set_prev (lg_dblinked_el<T> *prev) {
			this->prev = prev;
		}
};

template <class T>
class lg_dblinked_list
{
	protected:
		lg_dblinked_el<T> *list, *end;

	public:
		lg_dblinked_list();
		~lg_dblinked_list();
		void add_end(T *el);
		void add_beginning(T *el);
		void destroy();

		inline lg_dblinked_el<T>* get_first() {
			return this->list;
		}
		
		inline lg_dblinked_el<T>* get_last() {
			return this->end;
		}
};

/********************************************************************/

template <class T>
lg_dblinked_list<T>::lg_dblinked_list ()
{
	this->list = NULL;
	this->end = NULL;
}

template <class T>
lg_dblinked_list<T>::~lg_dblinked_list ()
{
	this->destroy();
}

template <class T>
void lg_dblinked_list<T>::destroy ()
{
	lg_dblinked_el<T> *next;
	
	while (this->list) {
		next = this->list->get_next();
		delete this->list;
		this->list = next;
	}
}

template <class T>
void lg_dblinked_list<T>::add_end (T *el)
{
	lg_dblinked_el<T> *tmp;
	
	tmp = new lg_dblinked_el<T>;
	tmp->set_next(NULL);
	tmp->set_prev(this->end);
	tmp->set_data(el);
	
	if (!this->list) {
		this->list = tmp;
		this->end = tmp;
	}
	else {
		this->end->set_next(tmp);
		this->end = tmp;
	}
}

template <class T>
void lg_dblinked_list<T>::add_beginning (T *el)
{
	lg_dblinked_el<T> *tmp;
	
	tmp = new lg_dblinked_el<T>;
	tmp->set_next(this->list);
	tmp->set_prev(NULL);
	tmp->set_data(el);
	
	if (!this->end) // first element
		this->end = tmp;
	else
		this->list->set_prev(tmp);

	this->list = tmp;	
}

#endif
