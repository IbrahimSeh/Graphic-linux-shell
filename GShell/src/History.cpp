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
	current = 0;
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
