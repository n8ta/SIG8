#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#include "sampler.h"
#include "metalogger.h"
#include "launcher.h"
#include "fsutils.h"

// Log a file to disk with this information:
// 0. app name
// 1. process id
// 2. thread id receiving signal
// 3. signal
// 4. time
// Then launch crash app (if supplied) and begin sampling
int log_crash(char *app_name,
              char *crash_app_path,
              pid_t pid,
              int signo,
              time_t time,
              int crash_id) {

  if (!create_logdir(app_name, crash_id)) {
    return 0;
  }

  struct MetaLogger ml;
  int err = ml_create(handlerlog_path, &ml);
  if (err != 1) {
    // Unable to init logger return.
    return 0;
  }
  ml_log(ml, "Log dirs created and logger successfully initialized.\n");

  uint64_t thread_id;
  if (pthread_threadid_np(NULL, &thread_id) != 0) {
    // Unable to get thread id. Set to 0
    ml_err(ml, "Failed to get thread id");
    thread_id = 0;
  }

  FILE *crash_file;
  if ((crash_file = fopen(crash_path, "w+")) == NULL) {
    ml_err(ml, "Failed to open crash file %s\n", crash_path);
    ml_destroy(ml);
    return 0;
  }

  ml_log(ml, "Crash file created.\n");

  char crash_data[MAX_CRASH_LEN];
  int bytes_needed = snprintf(&crash_data[0], MAX_CRASH_LEN,
                              "SIG8 Crash Report\napp_name\t%s\npid\t%d\ntid\t%lld\nsigno\t%d\ntime\t%ld",
                              app_name, pid, thread_id, signo, time);

  if (bytes_needed >= MAX_CRASH_LEN) {
    ml_err(ml, "Insufficient space in MAX_CRASH_LEN %d to fit crash report. App name: %s\n", MAX_CRASH_LEN, app_name);
    fclose(crash_file);
    ml_destroy(ml);
    return 0;
  }

  ml_log(ml, "Crash data created.\n");

  int write_result = fputs(crash_data, crash_file);
  if (write_result == EOF) {
    char *err = strerror(errno);
    ml_err(ml, "Failed to write crash data to file with io error: %s\n", err);
    fclose(crash_file);
    ml_destroy(ml);
    return 0;
  }

  ml_log(ml, "Crash data written, closing file.\n");

  // Done with crash file.
  fclose(crash_file);

  // Fork user supplied crash app if any
  if (crash_app_path != NULL) {
    if (!launch_app(crash_app_path, ml)) {
      ml_err(ml, "Failed to launch crash app. Continuing on to sample.\n");
    }
  } else {
    ml_log(ml, "User does not have a crash app path. Skipping.\n");
  }

  if (!sample(sample_path, ml)) {
    ml_err(ml, "Failed to sample.");
  }

  ml_log(ml, "Tearing down logger and returning.");
  ml_destroy(ml);

  return 1;
}