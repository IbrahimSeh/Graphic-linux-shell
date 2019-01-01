/*
 * History.h
 *
 *  Created on: Dec 11, 2018
 *      Author: root
 */

#ifndef HISTORY_H_
#define HISTORY_H_
#define HISTORY_DEFAULT_SIZE 100

template <class HistoryItem>
class History {
public:
	History(int size=HISTORY_DEFAULT_SIZE);
	void append(HistoryItem *);
	void goPastEnd();
	HistoryItem *up();
	HistoryItem *down();
	virtual ~History();
private:
	struct element {
		HistoryItem *item;
		struct element *next, *prev;
	};
	struct element *first, *last, *current;
	int count;
};

#include "History.cpp"

#endif /* HISTORY_H_ */
