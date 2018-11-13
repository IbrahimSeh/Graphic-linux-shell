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
int fdToStdout(int fd) {
    char c;
    int dollars = 0;

    while(read(fd, &c, 1) == 1) {
    	putchar(c);
    	if (c == '\n') fflush(stdout);
        if (c == '$') dollars++;
        else dollars=0;
        if (dollars == 3) break;
    }
    fflush(stdout);
    getchar();
    return 1;
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
    CommandLine::init();
    int terminator=0;
    do {
    	CommandLine command = CommandLine();
        fdToStdout(mfd);
    	terminator = command.edit();
    	if (terminator != 4) {
            command.send(mfd);
    	}
    } while(terminator != 4);
    kill(childPid, SIGTERM);
    wait(NULL);
    close(mfd);
    return 0;
}
