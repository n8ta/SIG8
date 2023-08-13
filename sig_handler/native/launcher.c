#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "fsutils.h"
#include "metalogger.h"

// Launch user supplied crash app
int launch_app(char* app_path, struct MetaLogger ml) {

  ml_log(ml, "Launching user crash app, pre fork\n");
  pid_t pid = fork();
  if (pid == -1) {
    char* fork_err = strerror(errno);
    ml_err(ml, "Failed fork to launch crash app continue on to sample. Error: %s\n", fork_err);
    return 0;
  }

  if (pid == 0) {
    // Child execs open to launch crash app

    // argV is a null terminated array of null terminated char*s
    // -a is app path
    // -n means open even if an instance already exists
    // --env sets the environment variable SIG8_CRASH_DUMP_PATH pointing to the crash dump folder
    char env_var[MAX_PATH_LEN];
      snprintf(&env_var[0], MAX_PATH_LEN, "SIG8_CRASH_DUMP_PATH=%s", log_dir_path);
    ml_log(ml, "Child pre-execvp open crash app %s with env_var %s\n", app_path, env_var);

    char* open_argv[] = { "/usr/bin/open", "-a", app_path, "-n", "--env", env_var, 0 };
    execvp("/usr/bin/open", open_argv);

    // If we got here execvp failed. Child thread logs and exits.
    char* error = strerror(errno);
    ml_err(ml, "Unable to open crash app '%s' due to error '%s'\n", app_path, error);
    exit(-1);
  } else if (pid > 0) {
    // Parent after fork
    ml_log(ml, "Parent after fork. Child pid is %d\n", pid);
  }
  return 1;
}