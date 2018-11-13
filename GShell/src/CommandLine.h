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
	virtual ~CommandLine();
	int edit(WINDOW *win);
	void send(int fd);
	void do_backspace(WINDOW* win);

private:
	std::string line;
	static bool insertMode;
	int minX, minY, currX, currY, maxX, maxY;
	int eraseCharacter;
	int commandLength;

	static void init();
	static void toggleInsertMode();
	void leftArrow(WINDOW *win);
	void rightArrow(WINDOW *win);
	void DeleteCharacter(WINDOW *win);
	void updateLineFromWindow(WINDOW *win);
	void winAddChar(WINDOW *win,int c);
	void mouseClick(WINDOW *win);
	void homeClick(WINDOW *win);
	void endClick(WINDOW *win);

};

#endif /* COMMANDLINE_H_ */
