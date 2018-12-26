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
	bool isEmpty, isFull;
	int size;
	int current;
	int first;  // points to the first element of history
	            // if empty it is last + 1 (mod size)
	int last;   // points to the last element of history
                // if empty it is first - 1 (mod size)
	            // We have the same situation also when list is full
	            // we distinguish using the flag isEmpty
};

#include "History.cpp"

#endif /* HISTORY_H_ */
