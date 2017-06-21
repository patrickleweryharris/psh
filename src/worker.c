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
  int fd[2], f, status, command_type;
  char *buf = malloc(sizeof(char) * MAXDATA);
  char *user_input = malloc(sizeof(char) * MAXLINE);
  FILE *in_file = malloc(sizeof(FILE));
  FILE *out_file = malloc(sizeof(FILE));
  char *cmd = malloc(sizeof(char) * MAXLINE);
  char *args = malloc(sizeof(char) * MAXDATA);
  if (buf == NULL || user_input == NULL || in_file == NULL || out_file == NULL || cmd == NULL || args == NULL){
    perror("malloc");
    exit(1);
  }

  while (1){
    scanf("%s", user_input);
    parser(user_input, in_file, out_file, cmd, args);

    if (pipe(fd) == -1){
      perror("pipe");
      exit(1);
    }

    if ((f = fork()) > 0){ // parent

      if (in_file != NULL){
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
            exit(1);
          }
        }
      }
    }

    else if (f == 0){ // child
      if (in_file != NULL){
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
      }

      if (out_file != NULL){
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
      if (execvp(cmd, &args) == -1){
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
