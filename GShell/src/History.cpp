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
	if(first == 0 && last == size - 1)
		isFull=true;
	if (first == last && isFull) {
		first++;
		first %= size;
	}
}

template <class HistoryItem>
HistoryItem *History<HistoryItem>::up()
{
	if (isEmpty) return NULL;
	int tcurrent = current;
	if(current != first)
	{
		current--;
		current %= size;
		if(current<0) current = size - current;
	}
	else
		return items[first];
	return items[tcurrent];
}

template <class HistoryItem>
HistoryItem *History<HistoryItem>::down()
{
	if (isEmpty) return NULL;
	int tcurrent = current;//the next current
	tcurrent++;
	if(current != last)
	{
		current++;
		current %= size;
	}
	else
		return items[last];
	return items[tcurrent];
}




