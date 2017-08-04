#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>
#include <sys/wait.h>
#include <stdio.h>

#include "pshell.h"

int which_first(char *input);
char **mkargs(char *args);


// Return which redirection operator occurs first in string input
int which_first(char *input){
  int status_code = NO_REDIR;
  int i;

  for(i = 0; i < strlen(input); i++){
    if (input[i] == '<'){
      if (status_code == NO_REDIR){
        status_code = F_IN;
      }
      else if (status_code == F_OUT){
        status_code = OUT_FIRST;
      }
      else {
        fprintf(stderr, "Multiple occurences of input file detected\n");
        exit(1);
      }
    }
    else if (input[i] == '>'){
      if (status_code == NO_REDIR){
        status_code = F_OUT;
      }
      else if (status_code == F_IN){
        status_code = IN_FIRST;
      }
      else {
        fprintf(stderr, "Multiple occurences of output file detected\n");
        exit(1);
      }
    }
  }

  return status_code;
}

/*
 * Returns an argument list for execvp from a string of arguments
 */
char **mkargs(char *args){

  int array_size = 1;
  for (int i = 0; i < strlen(args); i++){
    if (args[i] == ' '){
      array_size += 1;
    }
  }
  char **ret_args = malloc(sizeof(char *) * array_size);

  char *buff;
  buff = strtok(args, " ");
  int j = 0;
  while (buff != NULL){
    ret_args[j] = malloc(sizeof(char) * strlen(buff));
    ret_args[j] = buff;
    j++;
    buff = strtok(NULL, " ");
  }

  return ret_args;
}
