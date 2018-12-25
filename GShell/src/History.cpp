/*
 * History.cpp
 *
 *  Created on: Dec 11, 2018
 *      Author: root
 */

#include "History.h"

template <class HistoryItem>
History<HistoryItem>::History(int size) {
	this->size = size;
	items = new HistoryItem*[size];
	isEmpty = true;
	first = 0;
	last = size - 1;
	current = last;
}

template <class HistoryItem>
History<HistoryItem>::~History() {
	if (!isEmpty) {
		for (int i=first; i != last; i++, i %= size) {
			delete (items[i]);
		}
	}
	delete items;
}

template <class HistoryItem>
void History<HistoryItem>::append(HistoryItem *item){
	last++;
	last %= size;
	items[last] = item;
	current = last;
	isEmpty=false;
	if (first == last && !isEmpty) {
		first++;
		first %= size;
	}
}

template <class HistoryItem>
HistoryItem *History<HistoryItem>::up()
{
	if (isEmpty) return NULL;
	if (current != first) {
	    if (current == 0) {
	    	current = size -1;
	    } else {
	    	current--;
	    }
	}
    return items[current];
}

template <class HistoryItem>
HistoryItem *History<HistoryItem>::down()
{
	if (isEmpty) return NULL;
	if (current != last) {
	    if (current == size - 1) {
	    	current = 0;
	    } else {
	    	current++;
	    }
	}
    return items[current];
}
