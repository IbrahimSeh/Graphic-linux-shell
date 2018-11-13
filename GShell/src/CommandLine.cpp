/*
 * CommandLine.cpp
 *
 *  Created on: Mar 17, 2018
 *      Author: cmshalom
 */

#include "CommandLine.h"
using namespace std;
#include<stdio.h>
#include <unistd.h>
#include <string>

bool CommandLine::insertMode;

CommandLine::CommandLine() {
	this->line = "";

}

CommandLine::CommandLine(string line) {
	this->line = line;
}

CommandLine::~CommandLine() {
}

int CommandLine::edit(WINDOW *win) {
	int c;
	getmaxyx(win,maxY,maxX);
	minX = 3;
	minY = 1;
	eraseCharacter = erasechar();
	commandLength = line.length();
	wmove(win, 1, 3);
	waddstr(win, line.c_str());
	wclrtoeol(win);
	keypad(stdscr, TRUE);
	mousemask(ALL_MOUSE_EVENTS,NULL);
	//	keypad(win,true);
	while (true) {
		getyx(win,currY,currX);
		c=wgetch(win);
		if(c < KEY_MIN && isprint(c))
		{
			winAddChar(win,c);
		}
		else if (c == eraseCharacter) {
			leftArrow(win);
			DeleteCharacter(win);
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
				DeleteCharacter(win);
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
	mousemask(BUTTON1_CLICKED,NULL);
	insertMode= false;
	toggleInsertMode();
}

void CommandLine::toggleInsertMode(){
	insertMode = ! insertMode;
	curs_set(insertMode ? 1 : 2);
}

void CommandLine::leftArrow(WINDOW *win){
	if(currX == 3 && currY == 1) return;
	if(currX == 3 && currY > 1)
		wmove(win,currY-1,maxX -2);
	else wmove(win,currY,currX-1);
}

void CommandLine::rightArrow(WINDOW *win){
	if((currY-1)*(maxX) + currX == commandLength+5*(currY-1) + 3)   //end of command
		return;
	else if((currY-1)*(maxX) + currX < commandLength+5*(currY-1) + 3 && currX == maxX-2) //next line of command
		wmove(win,currY+1,minX);
	else
		wmove(win,currY,currX+1);

}

void CommandLine::updateLineFromWindow(WINDOW *win){
	int i=0;
	char c;
	wmove(win,1,3);
	while(i<commandLength){
		c = mvwinch(win,1,3+i);
		line += c;
		i++;
	}
}

void CommandLine::DeleteCharacter(WINDOW *win)
{
	wdelch(win);
	commandLength--;
}

void CommandLine::winAddChar(WINDOW *win,int c)
{
	if(currX<getmaxx(win)-2)
	{
		if(commandLength < currX-2)
		{
			waddch(win, c);
			commandLength++;
			//printf("currX = %d ,commandLength = %d\n",currX,commandLength);
		}
		else if(commandLength+3 < maxX-2 || insertMode)
		{
			if(insertMode==true){
				waddch(win,c);
				commandLength++;
			}
			else{
				winsch(win,c);
				if(currX == commandLength+3) commandLength++;
			}

			wmove(win,currY,currX+1);
			//commandLength++;
		}
	}
	else
		flash();
}

void CommandLine::mouseClick(WINDOW *win){
	MEVENT mouseEvent;
	int begX,begY;
	getbegyx(win,begY,begX);
	getmouse(&mouseEvent);
	//printf("mouseX = %d  mouseY = %d  parentX = %d  parentY = %d ",mouseEvent.x,mouseEvent.y,begX,begY);
	int rangeOfCommand = (mouseEvent.y - begY -1) * (maxX-2) + (mouseEvent.x -minX);
	if(mouseEvent.x >= minX && mouseEvent.x <= maxX-2 && rangeOfCommand < commandLength+1){
		if(mouseEvent.y >= begY + minY && mouseEvent.y <= begY+ maxY)
			wmove(win,mouseEvent.y-begY,mouseEvent.x-begX);
	}

}

void CommandLine::homeClick(WINDOW *win){
	wmove(win,minY,minX);
}

void CommandLine::endClick(WINDOW *win){
	wmove(win,minY,commandLength+3);
}

/*  Remnants of a trial program written at the beginning of the semester
    wattron(inputWindow, A_STANDOUT);
    mvwprintw(inputWindow, 5, 15, "Hello %s", "World");
    mvwaddch(inputWindow, 6, 15, 'X');
    wattron(inputWindow, COLOR_PAIR(RED_BLUE));
    mvwprintw(inputWindow, 7, 15, "Red / Blue");
    wattroff(inputWindow, COLOR_PAIR(RED_BLUE));
    wattroff(inputWindow, A_STANDOUT);
    wrefresh(inputWindow);

    MEVENT mouseEvent;
        if (c == KEY_MOUSE) {
            if (getmouse(&mouseEvent) == OK) {
                if(mouseEvent.bstate & BUTTON1_CLICKED)
                    mvwprintw(inputWindow, 9, 60, "CLICKED");
                if(mouseEvent.bstate & BUTTON1_DOUBLE_CLICKED)
                    mvwprintw(inputWindow, 9, 60, "DBL CLICKED");
                wprintw(inputWindow, "AT %d, %d, %d",
                       mouseEvent.x, mouseEvent.y, mouseEvent.z);
            } else {
                mvwprintw(inputWindow, 9, 70, "MOUSE_ERR", c);
            }
 */
