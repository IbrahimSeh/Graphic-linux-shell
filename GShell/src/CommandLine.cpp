/*
 * CommandLine.cpp
 *
 *  Created on: Mar 17, 2018
 *      Author: cmshalom
 */
#include <stdio.h>
#include <unistd.h>
#include <string>
#include "CommandLine.h"

#define WIN_LEFT 0
#define WIN_TOP 21
#define WIN_HEIGHT 3
#define WIN_WIDTH 80
#define EDIT_START_POSITION 1
#define EDIT_END_POSITION WIN_WIDTH - 2
#define MAX_COMMAND_LENGTH EDIT_END_POSITION - EDIT_START_POSITION + 1

using namespace std;

bool CommandLine::insertMode;
int CommandLine::eraseCharacter;


CommandLine::CommandLine() {
	line = "";

}

CommandLine::CommandLine(string line) {
	this->line = line;
}

CommandLine::~CommandLine() {
}

int CommandLine::edit() {
	int c;

	WINDOW *win = initWindow();
	eraseCharacter = erasechar();
	commandLength = line.length();
	wmove(win, 1, EDIT_START_POSITION);
	waddstr(win, line.c_str());
	while (true) {
		getyx(win,currY, currX);
		c=wgetch(win);
		if (c == eraseCharacter) {
			leftArrow(win);
			deleteCharacter(win);
		} else if (c < KEY_MIN && isprint(c)) {
				winAddChar(win,c);
		} else {
			switch(c){
			case KEY_IC:
				toggleInsertMode();
				break;
			case KEY_LEFT:
				leftArrow(win);
				wrefresh(win);
				break;
			case KEY_RIGHT:
				rightArrow(win);
				wrefresh(win);
				break;
			case KEY_DC:
				deleteCharacter(win);
				refresh();
				break;
			case KEY_MOUSE:
				mouseClick(win);
				break;
			case KEY_HOME:
				homeClick(win);
				break;
			case KEY_END:
				endClick(win);
				break;
			default:
				updateLineFromWindow(win);
				endwin();
				return c;
			}
		}
	}
}


void CommandLine::send(int fd) {
	const char LF = '\n';
	write(fd, line.c_str() , line.length());
	write(fd, &LF, 1);
}

void CommandLine::init(){
	insertMode= false;
	toggleInsertMode();
}

void CommandLine::toggleInsertMode(){
	insertMode = ! insertMode;
	curs_set(insertMode ? 1 : 2);
}

void CommandLine::leftArrow(WINDOW *win){
	if(currX == EDIT_START_POSITION) return;
	wmove(win,currY,currX-1);
}

void CommandLine::rightArrow(WINDOW *win){
	if(currX == commandLength+EDIT_START_POSITION) return;
	wmove(win,currY,currX+1);
}

void CommandLine::updateLineFromWindow(WINDOW *win){
	int i;
	char c;

	for(i=0; i < commandLength; i++){
		c = mvwinch(win,1,EDIT_START_POSITION+i);
		line += c;
	}
}

void CommandLine::deleteCharacter(WINDOW *win)
{
	wdelch(win);
	commandLength--;
}

void CommandLine::winAddChar(WINDOW *win, int c)
{
	if (insertMode) {
		if (commandLength == MAX_COMMAND_LENGTH) {
			flash();
			return;
		}
		winsch(win,c);
		wmove(win, currY, currX+1);
        commandLength++;
	} else {
		if (currX == EDIT_END_POSITION) {
			flash();
			return;
		}
		waddch(win, c);
        if (currX == commandLength + EDIT_START_POSITION) commandLength++;
	}
}

void CommandLine::mouseClick(WINDOW *win){
	MEVENT mouseEvent;
	getmouse(&mouseEvent);

    if (mouseEvent.y != WIN_TOP + 1) return;
	if (mouseEvent.x >= EDIT_START_POSITION &&
		mouseEvent.x <= commandLength + EDIT_START_POSITION) {
			wmove(win,mouseEvent.y - WIN_TOP, mouseEvent.x-WIN_LEFT);
	}
}

void CommandLine::homeClick(WINDOW *win){
	wmove(win,1,EDIT_START_POSITION);
}

void CommandLine::endClick(WINDOW *win){
	wmove(win,1,commandLength+EDIT_START_POSITION);
}


WINDOW *CommandLine::initWindow() {
	WINDOW *ret;

    initscr();
    raw();
//  cbreak();
    noecho();
    start_color();
//  init_pair(RED_BLUE, COLOR_RED, COLOR_BLUE);
    ret = newwin(WIN_HEIGHT,WIN_WIDTH,WIN_TOP,0);
    box(ret, 0, 0);
    mvwaddstr(ret, WIN_HEIGHT-1, WIN_WIDTH - 20, "CTRL-D TO TERMINATE");
    keypad(ret, true);
    mousemask(BUTTON1_CLICKED | BUTTON1_DOUBLE_CLICKED, NULL);
    refresh();
    return ret;
}

// #define RED_BLUE 1

/*  Remnants of a trial program written at the beginning of the semester
    wattron(inputWindow, A_STANDOUT);
    mvwprintw(inputWindow, 5, 15, "Hello %s", "World");
    mvwaddch(inputWindow, 6, 15, 'X');
    wattron(inputWindow, COLOR_PAIR(RED_BLUE));
    mvwprintw(inputWindow, 7, 15, "Red / Blue");
    wattroff(inputWindow, COLOR_PAIR(RED_BLUE));
    wattroff(inputWindow, A_STANDOUT);
    wrefresh(inputWindow);
 */
