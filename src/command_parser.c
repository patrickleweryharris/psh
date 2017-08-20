#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>
#include <sys/wait.h>
#include <stdio.h>
#include <ctype.h>

#include "pshell.h"

int which_first(char *input);
char **mkargs(char *args);
void strip_leading_space(char *args);


/*
 * Return which redirection operator occurs first in string input
 * Order of redirection operators indicates the order in which the argument string
 * is to be parsed
 */
int which_first(char *input){
  // By default, assume that no redirection occurs.
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
  // Seperates args into an array of chars by each space.
  char **ret_args = NULL;
  char *buff = strtok(args, " ");
  int n_words = 0;
  while (buff){
    // Dynamic allocation of memory needed for ret_args.
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

/*
 * Strip leading spaces from string args, if there are any
 * Use this method after arg has been split into different parts for command,
 * arguments, and file redirection
 */
void strip_leading_space(char *args){

  const char *first_non_space = args;

  while (*first_non_space != '\0' && isspace(*first_non_space)){
    ++first_non_space;
  }

  size_t len = strlen(first_non_space) + 1;

  memmove(args, first_non_space, len);

}
