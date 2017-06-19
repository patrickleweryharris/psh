#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>
#include <sys/wait.h>
#include <stdio.h>

#include "pshell.h"

/* Parse the command into three seperate parts:
 * 1. The command
 * 2. The output file for stdout to go to (optional) -> Set to NULL if not applicable
 * 3. The input file for stdin to come from (optional) -> Set to NULL if not applicable
 */
int parser(char *user_input, FILE *in_file, FILE *out_file, char *base_cmd, char *args){

  /* This method assumes a structure for a command
   * $ cmd args {file redirection}
   * Everything before the first white space is interpreted to be the command
   * From there, everything until the file redirection operator is the arguments
   * The word after a file redirection operator is interpreted to be the filename
   */
  // TODO

  return NORM;
}
