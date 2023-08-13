extern void sig_handler_trigger_segfault();

#include <stdio.h>
#include <time.h>
#include "logger.h"

char* TEST_APP_NAME = "TEST_APP_NAME";

int main() {
  printf("Main running. Triggering segfault\n");
//  srand((int) time(NULL));
//  int crash_id = rand();
//  log_crash(TEST_APP_NAME, 0, 11, time(NULL), crash_id);
  sleep(1000);
  printf("woke up");
  sleep(1000);
}
