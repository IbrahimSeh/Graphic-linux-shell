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
	isFull = false;
	first = 0;
	//last = size - 1;
	last = 0;
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
	if(isFull){
		first++;
	}
	if(last == size){
		isFull = true;
		last = 0;
		first = 1;
	}

	current = last;
	items[last++] = item;
}

template <class HistoryItem>
HistoryItem *History<HistoryItem>::up()
{
	if(current == first) return items[current];
	else return items[current--];
}

template <class HistoryItem>
HistoryItem *History<HistoryItem>::down()
{
	if(current == last -1) return items[current];
	else return items[current++];
}
