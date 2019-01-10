/*
 * History.cpp
 *
 *  Created on: Dec 11, 2018
 *      Author: root
 */

#include "History.h"
#include <menu.h>

using namespace std;
template <class HistoryItem>
History<HistoryItem>::History(int size) {
	this->size = size;
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
	if(count == size) return ;
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
	return current ? current->item : NULL;
}

template <class HistoryItem>
HistoryItem *History<HistoryItem>::down()
{
	if (!first) return NULL;
	if (current != NULL && current != last) current = current->next;
	return current ? current->item : NULL;
}

template <class HistoryItem>
string  History<HistoryItem>::showHistory()
{
string *stringItems  = new string[count];
stringItems = getItems(stringItems);
Menu *menu = new Menu(count, stringItems);
string chosen = menu->showMenu();
delete []stringItems;
return chosen;
}

template <class HistoryItem>
string * History<HistoryItem>::getItems(string *stringItems)
{
	struct element *p=first;
		int i=0;
		while(p){
			stringItems[i++] =p->item->getTheString().c_str();
			p = p->next;
		}
		return stringItems;
}
