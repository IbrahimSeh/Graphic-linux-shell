/*
 * History.cpp
 *
 *  Created on: Dec 11, 2018
 *      Author: root
 */

#include "History.h"

template <class HistoryItem>
History<HistoryItem>::History(int size) {
	first = last = current = NULL;
	count = 0;
}

template <class HistoryItem>
History<HistoryItem>::~History() {
	struct element *p, *q;
	p = first;
	while (p) {
		q=p;
		p=p->next;
	    delete q;
	}

}

template <class HistoryItem>
void History<HistoryItem>::append(HistoryItem *item){
	if(count == HISTORY_DEFAULT_SIZE) return ;
	struct element *p = new struct element;
	p->item = item;
	p->next = NULL;
	p->prev = last;
	if (last) {
		last->next = p;
	} else {
		first=p;
	}
	last = p;
	current = p;
	count++;
}

template <class HistoryItem>
void History<HistoryItem>::goPastEnd()
{
	current=NULL;
}


template <class HistoryItem>
HistoryItem *History<HistoryItem>::up()
{
	if (!first) return NULL;
	if (current == NULL) {
		current = last;
	} else if (current != first) current = current->prev;
	return current->item;
}

template <class HistoryItem>
HistoryItem *History<HistoryItem>::down()
{
	if (!first) return NULL;
	if (current != NULL && current != last) current = current->next;
	return current->item;
}

