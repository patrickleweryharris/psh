#ifndef _PSH_H_
#define _PSH_H_

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>
#include <sys/wait.h>
#include <stdio.h>

// Path for exec:
// FIXME not yet implemented. 
#define PATH "/Users/patrickharris/repos/current/copyscript:/Users/patrickharris/repos/current/code-snippets/scripts:/Users/patrickharris/:/Users/patrickharris/.local/bin::/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin"

// Constants that define the maximum length of a parseable string.
// Maximum length of a command is MAXLINE characters.
// Maximum length of arguments is MAXDATA characters
#define MAXLINE 128
#define MAXDATA 1024

// File redirection operators
#define F_OUT 0 // >
#define F_IN 1 // <
#define NO_REDIR 2 // No rediction occurs
#define OUT_FIRST 3 // Out redir first, in next
#define IN_FIRST 4 // In redir first, out next

// Prototypes:
void main_shell();
int which_first(char *input);
char **mkargs(char *args);
void strip_leading_space(char *args);
void special_functions(char *cmd);

#endif // _PSH_H_
