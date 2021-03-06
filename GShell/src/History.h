/*
 * History.h
 *
 *  Created on: Dec 11, 2018
 *      Author: root
 */

#ifndef HISTORY_H_
#define HISTORY_H_
#define HISTORY_DEFAULT_SIZE 100

#include <menu.h>
#include "Menu.h"
template <class HistoryItem>
class History {
public:
	History(int size=HISTORY_DEFAULT_SIZE);
	void append(HistoryItem *);
	void goPastEnd();
	HistoryItem *up();
	HistoryItem *down();
	int getCount() {return count;}
	int getSize() {return size;}
	std::string showHistory();
	virtual ~History();
private:
	struct element {
		HistoryItem *item;
		struct element *next, *prev;
	};
	struct element *first, *last, *current;
	int size;
	int count;
	std::string * getItems(std::string *stringItems);

};

#include "History.cpp"

#endif /* HISTORY_H_ */
