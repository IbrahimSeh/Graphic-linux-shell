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
	HistoryItem *up();
	HistoryItem *down();
	virtual ~History();
private:
	HistoryItem **items;
	bool isEmpty;
	bool isFull;
	int size;
	int current;
	int first;
	int last;
};

#include "History.cpp"

#endif /* HISTORY_H_ */
