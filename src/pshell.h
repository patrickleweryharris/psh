#ifndef _PSH_H_
#define _PSH_H_

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>
#include <sys/wait.h>
#include <stdio.h>

#define MAXLINE 128
#define MAXDATA 1024

// File redirection operators
#define F_OUT 0 // >
#define F_IN 1 // <
#define NO_REDIR 2 // No rediction occurs
#define OUT_FIRST 3 // Out redir first, in next
#define IN_FIRST 4 // In redir first, out next

void main_shell();

int which_first(char *input);

#endif // _PSH_H_
