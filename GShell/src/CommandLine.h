/*
 * CommandLine.h
 *
 *  Created on: Mar 17, 2018
 *      Author: cmshalom
 */

#ifndef COMMANDLINE_H_
#define COMMANDLINE_H_


#include <ncurses.h>
#include <string>

#include "History.h"


class CommandLine {
public:
	CommandLine();
	CommandLine(std::string);
	CommandLine(char);
	CommandLine(CommandLine &);
	const CommandLine & operator= (const CommandLine &);
	virtual ~CommandLine();
	std::string getTheString();
	int edit();
	void send(int fd);
	void do_backspace(WINDOW* win);

	static void init();

private:
	std::string line;
	int currX=0, currY=0;
	int commandLength;

	static bool insertMode;
	static int eraseCharacter;
	static History<CommandLine> history;

	static void toggleInsertMode();
	static WINDOW *initWindow();
	void leftArrow(WINDOW *win);
	void rightArrow(WINDOW *win);
	void deleteCharacter(WINDOW *win);
	void updateLineFromWindow(WINDOW *win);
	void winAddChar(WINDOW *win,int c);
	void mouseClick(WINDOW *win);
	void homeKey(WINDOW *win);
	void endKey(WINDOW *win);
	void upArrow(WINDOW *win);
	void downArrow(WINDOW *win);
	void restartEdit(WINDOW *win);
};

#endif /* COMMANDLINE_H_ */
