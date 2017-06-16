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

#define NORM 0
#define OUT 1
#define IN 2
#define INOUT 3
#define PIPE 4
#define INOUTPIPE 5

void main_shell();

int parser(char *user_input, FILE *in_file, FILE *out_file, char *base_cmd, char *args);

#endif // _PSH_H_
