#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>
#include <sys/wait.h>
#include <stdio.h>

#include "pshell.h"

int which_first(char *input);

/* Parse the command into three seperate parts:
 * 1. The command
 * 2. The output file for stdout to go to (optional) -> Set to NULL if not applicable
 * 3. The input file for stdin to come from (optional) -> Set to NULL if not applicable
 */
void parser(char *user_input, FILE *in_file, FILE *out_file, char *base_cmd, char *args){

  /* This method assumes a structure for a command
   * $ cmd args {file redirection}
   * Everything before the first white space is interpreted to be the command
   * From there, everything until the file redirection operator is the arguments
   * The word after a file redirection operator is interpreted to be the filename.
   * The method als assumes that all parameters are properly allocated in memory
   */
   char *buff = malloc(sizeof(char) * MAXDATA);
   buff = strtok(user_input, " ");
   strncpy(base_cmd, buff, sizeof(char) * MAXLINE);

   int command_type = which_first(user_input);
   switch (command_type) {
    case NO_REDIR:
      strncpy(args, buff, sizeof(char) * MAXDATA);
      in_file = NULL; // Not sure this will set out_file to something out of scope
      out_file = NULL;

    case F_IN:
      buff = strtok(NULL, "<");
      strncpy(args, buff, sizeof(char) * MAXDATA);
      out_file = NULL;
      in_file = fopen(user_input, "r");

    case F_OUT:
      buff = strtok(NULL, ">");
      strncpy(args, buff, sizeof(char) * MAXDATA);
      in_file = NULL;
      out_file = fopen(user_input, "w");

    case OUT_FIRST:
      buff = strtok(NULL, ">");
      strncpy(args, buff, sizeof(char) * MAXDATA);
      buff = strtok(NULL, "<");
      out_file = fopen(buff, "w");
      in_file = fopen(user_input, "r");

    case IN_FIRST:
      buff = strtok(NULL, "<");
      strncpy(args, buff, sizeof(char) * MAXDATA);
      buff = strtok(NULL, ">");
      in_file = fopen(buff, "r");
      out_file = fopen(user_input, "w");
   }

}

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
