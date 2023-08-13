#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include "metalogger.h"

int sample(char *output_file, struct MetaLogger ml) {
  ml_log(ml, "Begin sample into: %s.\n", output_file);

  pid_t pid = getpid();

  char pid_str[1024];
  if (sprintf(&pid_str[0], "%d", pid) > 1024) {
    ml_err(ml, "Unable to convert pid to string. Longer than 1024\n");
    return 0;
  };
  char *sample_argv[] = {"/usr/bin/sample", pid_str, "1", "-mayDie", "-f", output_file, NULL};

  pid_t fork_pid = fork();
  if (fork_pid == -1) {
    ml_err(ml, "Failed to fork. Returning without sampling\n");
    return 0;
  }
  if (fork_pid == 0)
  {
    ml_log(ml, "Child of sample fork, pre-execvp\n");
    // Child process run sample
    execvp("/usr/bin/sample", &sample_argv[0]);

    // execvp failed;
    char *error = strerror(errno);
    ml_err(ml,"Child: Unable to open sample due to error '%s'\n", error);

    // Exit child process
    exit(-1);
  }

  ml_log(ml, "Sample launched. Blocking SIGCHLD and sleeping\n");
  
  // In order to call sleep we need to block the SIGCHLD signal
  // https://stackoverflow.com/questions/60209210/sleep-instruction-doesnt-work-with-signal-handler
  sigset_t mask;
  if ((sigemptyset(&mask) == -1) || (sigaddset(&mask, SIGCHLD) == -1)) {
    ml_err(ml, "Failed to set mask or mark flag. Unable to sleep.\n");
    return 0;
  }
  
  if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) {
    ml_err(ml, "Unable to block signal will not be able to sleep.\n");
    return 0;
  }

  // Sleep
  sleep(5);
  
  // Put sigchld back
  if (sigprocmask(SIG_UNBLOCK, &mask, NULL) == -1) {
    ml_err(ml, "Unable to unblock signal. Probably not an issue.\n");
    return 0;
  }

  ml_log(ml, "Done sleeping returning.\n");
  return 1;
}