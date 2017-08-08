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
  char **ret_args = NULL;
  char *buff = strtok(args, " ");
  int n_words = 0;
  while (buff){
    ret_args = realloc(ret_args, sizeof(char*) * ++n_words);
    if (ret_args == NULL){
      perror("realloc");
      exit(1);
    }

    ret_args[n_words - 1] = buff;

    buff = strtok(NULL, " ");
  }

  ret_args = realloc(ret_args, sizeof(char*) * (n_words + 1));
  ret_args[n_words] = 0;

  // Utility to print args for testing purposes: 
  // for(int i=0; i < (n_words + 1); i++){
  //   printf ("args[%d] = %s\n", i, ret_args[i]);
  // }


  return ret_args;
}
