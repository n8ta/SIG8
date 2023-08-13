#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

#include "logger.h"
#include "fsutils.h"

static int initialized = 0;

// Name of the application using the signal handler
// Null terminated string or NULL
static char* app_name = NULL;
static int app_name_len = 0;

// Path the application wants the signal handler to launch on crash
// Null terminated string or NULL
static char* crash_app_path = NULL;
static int crash_app_path_len = 0;

void sig_handler(int signo)
{
  time_t current_time = time(NULL);
  pid_t crashing_pid = (pid_t) 1;
  srand(current_time);
  int crash_id = rand();

  if (app_name_len != 0) {
    printf("App %s received signal %d\n", app_name, signo);
    log_crash(app_name, crash_app_path,crashing_pid, signo, current_time, crash_id);
  } else  {
    printf("Received signal '%d' but no app registered, exiting...\n", signo);
  }
  exit(-1);
}


// Called from c# to set the application name
void __attribute__((cdecl)) sig_handler_set_app_name(char* name, int length) {
  printf("Setting app name to %.*s\n", length, name);
  copy_string(&app_name, &app_name_len, name, length);
}

// Called from c# to set the crash app path
void __attribute__((cdecl)) sig_handler_set_app_callback_app(char* path, int length) {
  printf("Setting crash app to %.*s\n", length, path);
  copy_string(&crash_app_path, &crash_app_path_len, path, length);
}

void register_signal(int sig) {
  if (signal(sig, sig_handler) == SIG_ERR) {
    printf("Signal %d failed to register.\n", sig);
  }
}

// Called by the linker when the library is loaded
void __attribute((constructor))init()
{
  if (initialized != 0) {
    return;
  }
  initialized = 1;

  // TODO: What other signals?
  register_signal(SIGILL);
  register_signal(SIGSEGV);
}