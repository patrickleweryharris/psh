#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "pshell.h"

void print_license();

void special_functions(char *cmd){

  char *info = "psh - Patrick's Shell. Type 'license' for copyright information";
  char *version = "0.0.1";

  if (strncmp(cmd, "about", 5) == 0){
    printf("%s\n", info);
    exit(0);
  }

  else if (strncmp(cmd, "license", 7) == 0){
    print_license();
    exit(0);
  }

  else if (strncmp(cmd, "version", 7) == 0){
    printf("%s\n", version);
    exit(0);
  }

}

void print_license(){
  FILE *license_file;
  char c;

  license_file = fopen("LICENSE", "r");
  if (license_file == NULL){
    perror("fopen");
    exit(1);
  }

  c = fgetc(license_file);

  while (c != EOF){
    printf("%c", c);
    c = fgetc(license_file);
  }

  fclose(license_file);
}
