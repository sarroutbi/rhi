#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

#define USOCKET "/dev/log"
#define MAX_LEN 1024
#define LOG_ARRAY_SIZE 65535

uint8_t g_running = 1;
uint32_t g_socket = 0;

typedef struct {
  char* log_entry;
  uint64_t occurrences;
} log_entry_t;

static int rhlogger_create_usocket() {
  int diag;
  struct sockaddr_un uaddr;
  g_socket = socket(PF_UNIX, SOCK_DGRAM, 0);
  if (diag == -1) {
    return -1;
  }
  memset(&uaddr, 0, sizeof(uaddr));
  uaddr.sun_family = AF_UNIX;
  strncpy(uaddr.sun_path, USOCKET, strlen(USOCKET));
  unlink(USOCKET);
  if (bind(g_socket, (struct sockaddr *)&uaddr, sizeof(uaddr)) < 0) {
    return -1;
  }
  return 0;
}

static void clean_log_array(log_entry_t** log_array, int log_array_size) {
  for(int i = 0; i < log_array_size; i++) {
    free(log_array[i]);
  }
  free(log_array);
}

static void add_entry(log_entry_t** log_array, uint64_t pos, char* log) {
  log_array[pos] = (log_entry_t*)malloc(sizeof(log_entry_t));
  log_array[pos]->log_entry = (char*)malloc(strlen(log)*sizeof(char));
  strncpy(log_array[pos]->log_entry, log, strlen(log)-1);
  log_array[pos]->occurrences++;
}

static uint8_t check_or_add_entry(log_entry_t** log_array, uint64_t total_log_counter, char* log) {
  // TODO: this could be ordered after insertion for performance, but nothing specified in exercise
  uint8_t added = 0;
  if(!total_log_counter) {
    add_entry(log_array, total_log_counter, log);
    return 1;
  }
  for(uint64_t entry = 0; entry < total_log_counter; entry++) {
    if(strncmp(log_array[entry]->log_entry, log, strlen(log)-1) == 0) {
      log_array[entry]->occurrences++;
      return 0;
    }
  }
  add_entry(log_array, total_log_counter, log);
  return 1;
}

static void dump_most_frequent(log_entry_t** log_array, int log_array_size) {
  uint64_t max_occurrences = 0;
  uint64_t most_frequent_log_idx = 0;
  for(uint64_t entry = 0; entry < log_array_size; entry++) {
    if(log_array[entry]->occurrences > max_occurrences) {
      max_occurrences = log_array[entry]->occurrences;
      most_frequent_log_idx = entry;
    }
  }
  printf("\n\nMost common log:[%s](%lu times)\n\n", log_array[most_frequent_log_idx]->log_entry,
         log_array[most_frequent_log_idx]->occurrences );
  fflush(stdout);
}

static void rhlogger_loop(char** farray) {
  uint32_t length = 0;
  uint32_t overrides = 0;
  uint64_t log_counter = 0;
  uint64_t total_log_counter = 0;
  int flags = 0;
  char buff[MAX_LEN];
  log_entry_t** log_array;
  uint64_t log_array_size = LOG_ARRAY_SIZE;
  log_array = (log_entry_t**)malloc(sizeof(log_entry_t*)*log_array_size);
  flags |= MSG_DONTWAIT;
  while(g_running) {
    memset(buff, 0, MAX_LEN);
    if((length = recv(g_socket, buff, MAX_LEN, flags)) < 0) {
      fprintf(stderr, "Error reading from socket: [%d(%s)]\n", errno, strerror(errno));
    }
    if(strlen(buff) > 0 && length > 0) {
      if(log_counter >= LOG_ARRAY_SIZE) {
        overrides += 1;
        log_array_size += (overrides + 1) * log_array_size;
        log_array = realloc(log_array, log_array_size);
        log_counter = 0;
      }
      if(check_or_add_entry(log_array, total_log_counter, buff)) {
        printf("[%lu]:%s\n", total_log_counter, log_array[total_log_counter]->log_entry);
        total_log_counter+=1;
      }
    }
    usleep(1);
  }
  if(total_log_counter) {
    dump_most_frequent(log_array, total_log_counter);
  }
  clean_log_array(log_array, total_log_counter);
}

int rhlogger_run(char** farray) {
  // Create Unix Socket
  if(rhlogger_create_usocket()) {
    fprintf(stderr, "Error on Unix Socket creation: [%d(%s)]\n", errno, strerror(errno));
  }
  // Run loop
  rhlogger_loop(farray);
}

