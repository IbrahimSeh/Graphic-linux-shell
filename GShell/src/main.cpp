#define _XOPEN_SOURCE
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <ncurses.h>

#include "CommandLine.h"

#define MASTER_TTY_NAME "/dev/ptmx"

// #define RED_BLUE 1
WINDOW *inputWindow;
WINDOW *outputWindow;

void openSlave(int masterFd) {
    struct termios stermios;
    char *slaveDeviceName = ptsname(masterFd);
    int fd = open (slaveDeviceName, O_RDWR);
    if (fd == -1) {
        perror (slaveDeviceName);
        exit (20);
    }
    if (tcgetattr(fd, &stermios) < 0) {
        perror("tcgetattr");
        exit (21);
    }
    stermios.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);
    stermios.c_oflag &= ~ONLCR;
    if (tcsetattr(fd, TCSANOW, &stermios) < 0) {
        perror("tcsetattr");
        exit (22);
    }

    if(dup2(fd, STDIN_FILENO) != STDIN_FILENO) {
        perror (slaveDeviceName);
        exit (23);
    }
    if(dup2(fd, STDOUT_FILENO) != STDOUT_FILENO) {
        perror (slaveDeviceName);
        exit (24);
    }
    if(dup2(fd, STDERR_FILENO) != STDERR_FILENO) {
        perror (slaveDeviceName);
        exit (25);
    }
    close(fd);
}

// Returns non-zero if prompt ($$$) detected
int lineFdToWin(int fd, WINDOW *win) {
    char c;
    int x, y, maxx, maxy;
    int dollars;

    getyx(win, y, x);
    getmaxyx(win, maxy, maxx);
    y++;
    if (y==maxy-1) {
       wclear(win);
       y = 1;
    }
    wmove(win, y, 1);
    dollars=0;

    while(read(fd, &c, 1) == 1) {
    	//putchar(c);
        if (c == '\n') {
            wrefresh(win);
            return 0;
        }
        waddch(win,c);
        if (c == '$') dollars++;
        else dollars=0;
        if (dollars == 3) return 1;
    }
    return 1;
}

void initWindows() {
	//CommandLine::insert_mode = true;
    initscr();
    raw();
//    cbreak();
    noecho();
    start_color();
   // init_pair(RED_BLUE, COLOR_RED, COLOR_BLUE);
    inputWindow = newwin(3,80,21,0);
    outputWindow = newwin(21,80,0,0);
    refresh();
    box(inputWindow, 0, 0);
    mvwaddstr(inputWindow, 2, 60, "CTRL-D TO TERMINATE");
    scrollok(outputWindow, true);
    keypad(inputWindow, true);
    mousemask(BUTTON1_CLICKED|BUTTON1_DOUBLE_CLICKED, NULL);
}

int main () {

    int mfd = open (MASTER_TTY_NAME, O_RDWR);
    if (mfd == -1) {
        perror (MASTER_TTY_NAME);
        exit (1);
    }
    // This is the place to do any initializations we want
    // before any process can open the slave device
    if (unlockpt(mfd) != 0) {
        perror (MASTER_TTY_NAME);
        exit (2);
    }
    printf ("Pts name = %s\n", ptsname(mfd));
    int childPid = fork();
    if (childPid==0) {
        openSlave (mfd);
        close(mfd);
        execlp("bash", "bash", "-init-file", ".init", NULL);
        perror("exec");
        exit(11);
    }
    initWindows();
    CommandLine command;
    //CommandLine::init_insertMode();
    int terminator=0;
    do {
    	command = CommandLine();
        while(!lineFdToWin(mfd, outputWindow));
        //wrefresh(outputWindow);

    	terminator = command.edit(inputWindow);
    	if (terminator != 4) {
            command.send(mfd);
    	}
    } while(terminator != 4);
    endwin();
    kill(childPid, SIGTERM);
    wait(NULL);
    close(mfd);
    return 0;
}
