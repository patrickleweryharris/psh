#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>
#include <sys/wait.h>
#include <stdio.h>

#include "pshell.h"

/* Run the psh shell */
void main_shell(){
  // Integers denote pipes, command types and statuses.
  int fd[2], f, status, command_type;

  // Buffer for pipe
  char *buf = malloc(sizeof(char) * MAXDATA);

  // Raw input
  char *user_input = malloc(sizeof(char) * MAXLINE);

  // Copy of input that is modified during parsing
  char *input_to_modify = malloc(sizeof(char) * MAXLINE);

  // Files for redirection
  FILE *in_file = malloc(sizeof(FILE));
  FILE *out_file = malloc(sizeof(FILE));

  // The command and arguments
  char *cmd = malloc(sizeof(char) * MAXLINE);
  char *args = malloc(sizeof(char) * MAXDATA);

  // Error checking
  if (buf == NULL || user_input == NULL || in_file == NULL || out_file == NULL || cmd == NULL || args == NULL){
    perror("malloc");
    exit(1);
  }

  while (1){
    // Shell prompt
    printf("> ");
    fgets(user_input, MAXLINE, stdin);

    // Input sanitization. Any errors input by the user will result in command fail later, not dealt with here.
    user_input[strcspn(user_input, "\n")] = 0;
    char *buff = malloc(sizeof(char) * MAXDATA);
    strncpy(input_to_modify, user_input, sizeof(char) * MAXLINE);
    buff = strtok(input_to_modify, " ");
    strncpy(cmd, buff, sizeof(char) * MAXLINE);


    // Exit function. Needs to be before fork to allow entire program to exit
    if (strncmp(cmd, "exit", 4) == 0){
      exit(0);
    }

    // Switch controls how the command is parsed based upon present file redirection operators
    command_type = which_first(user_input);
    //printf("Command type: %i\n", command_type);
    switch (command_type) {
     case NO_REDIR:
       strncpy(args, buff, sizeof(char) * MAXDATA);
       in_file = NULL;
       out_file = NULL;
       break;

     case F_IN:
       buff = strtok(NULL, "<");
       strncpy(args, buff, sizeof(char) * MAXDATA);
       out_file = NULL;
       strip_leading_space(buff);
       in_file = fopen(buff, "r");
       break;

     case F_OUT:
       buff = strtok(NULL, ">");
       //printf("%s\n", buff);
       //printf("%s\n", input_to_modify);
       strncpy(args, buff, sizeof(char) * MAXDATA);
       in_file = NULL;
       strip_leading_space(buff);
       out_file = fopen(buff, "w");
       break;

     case OUT_FIRST:
       buff = strtok(NULL, ">");
       strip_leading_space(buff);
       out_file = fopen(buff, "w");
       strncpy(args, buff, sizeof(char) * MAXDATA);
       buff = strtok(NULL, "<");
       strip_leading_space(buff);
       in_file = fopen(buff, "r");
       break;

     case IN_FIRST:
       buff = strtok(NULL, "<");
       strip_leading_space(buff);
       in_file = fopen(buff, "r");
       strncpy(args, buff, sizeof(char) * MAXDATA);
       buff = strtok(NULL, ">");
       strip_leading_space(buff);
       out_file = fopen(buff, "w");
       break;
    }


    if (pipe(fd) == -1){
      perror("pipe");
      exit(1);
    }

    if ((f = fork()) > 0){ // parent

      if (command_type == F_IN || command_type == OUT_FIRST || command_type == IN_FIRST){
        // Write contents of in_file to buffer
        int read_b = fread(&buf, sizeof(char) * MAXDATA, 1, in_file);
        if (read_b == 0){
          perror("fread");
          exit(1);
        }

        // Write buf to pipe
        close(fd[0]);
        write(fd[1], buf, sizeof(buf));
        close(fd[1]);
      }

      if(wait(&status) == -1) {
          perror("wait");
          exit(1);
      }

      else {
        if(WIFEXITED(status)) {
          if (WEXITSTATUS(status) != 0){ // The child ran into an error somewhere and hasn't been reported already
            fprintf(stderr, "Error encountered during command execution\n");
          }
        }
      }
    }

    else if (f == 0){ // child
      // Check for special commands
      special_functions(cmd);

      if (command_type == F_IN || command_type == OUT_FIRST || command_type == IN_FIRST){
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
      }

      if (command_type == F_OUT || command_type == OUT_FIRST || command_type == IN_FIRST){
        if (dup2(fileno(out_file), STDOUT_FILENO) == -1){
          perror("dup2");
          exit(1);
        }
        if (dup2(fileno(out_file), fileno(stderr)) == -1){
          perror("dup2");
          exit(1);
        }
      }

      // Command execution
      char **split_args = mkargs(input_to_modify);
      if (command_type == NO_REDIR){ // FIXME temp fix to make parsing work! Shouldn't be in final product
        split_args = mkargs(user_input);
      }
      if (execvp(cmd, split_args) == -1){
        perror("exec");
        exit(1);
      }

      exit(0);

    }

    else { // fork failed
      perror("fork");
      exit(1);
    }
  }
}
