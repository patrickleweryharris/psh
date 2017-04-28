#include <stdio.h>
#include "psh.h"

int main(int argc, char **argv) {
  if (argc != 1){
    printf("Usage: ./psh \n");
    return 0;
  }
  main_shell();
  return 0;
}
