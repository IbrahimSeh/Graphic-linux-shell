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

class CommandLine {
public:
	CommandLine();
	CommandLine(std::string);
	CommandLine(char);
	virtual ~CommandLine();
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

	static void toggleInsertMode();
	static WINDOW *initWindow();
	void leftArrow(WINDOW *win);
	void rightArrow(WINDOW *win);
	void deleteCharacter(WINDOW *win);
	void updateLineFromWindow(WINDOW *win);
	void winAddChar(WINDOW *win,int c);
	void mouseClick(WINDOW *win);
	void homeClick(WINDOW *win);
	void endClick(WINDOW *win);

};

#endif /* COMMANDLINE_H_ */
