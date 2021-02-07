#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <signal.h>

#include "rhlogger_loop.h"

#define MAX_FILES 1000

extern uint8_t g_running;

void handle_sigint(int sig)
{
  g_running = 0;
} 

void parse_args(char** farray, int argc, char** argv) {
  for(int i = 1; i < argc; i++) {
    farray[i-1] = (char*)malloc(sizeof(char)*PATH_MAX);
    strncpy(farray[i-1], argv[i], strlen(argv[i]));
  }
}

void dump_args(char** farray, int size) {
  for(int i = 0; i < size; i++) {
    printf("File[%d/%d]:%s\n", i, size, farray[i]);
  }
}

void clean_args(char** farray, int size) {
  for(int i = 0; i < size; i++) {
   free (farray[i]);
  }
  free(farray);
}

int main(int argc, char* argv[]) {
  int farray_size;
  char** farray;
  umask(0); // Set write/read permissions for everybody
  signal(SIGINT, handle_sigint);
  farray_size = argc-1;
  farray = (char**)malloc(sizeof(char*)*farray_size);
  parse_args(farray, argc, argv);
  rhlogger_run(farray, farray_size);
  clean_args(farray, farray_size);
  return 0;
}
