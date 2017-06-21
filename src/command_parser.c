#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>
#include <sys/wait.h>
#include <stdio.h>

#include "pshell.h"


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
