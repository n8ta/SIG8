#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <string.h>

#include "consts.h"
#include "fsutils.h"

// Recursive dir maker taken from: https://stackoverflow.com/questions/2336242/recursive-mkdir-system-call-on-unix
static int mkpath(const char *dir, mode_t mode) {
  char tmp[MAX_PATH_LEN];
  char *p = NULL;
  size_t len;

  snprintf(tmp, sizeof(tmp), "%s", dir);
  len = strlen(tmp);
  if (tmp[len - 1] == '/')
    tmp[len - 1] = 0;
  for (p = tmp + 1; *p; p++)
    if (*p == '/') {
      *p = 0;
      if (mkdir(tmp, mode) != 0 && errno != EEXIST) {
        return -1;
      }
      *p = '/';
    }
  if (mkdir(tmp, mode) != 0 && errno != EEXIST) {
    return -1;
  }
  return 0;
}

int create_file_path(char *dest_path, char *app_name, int crash_id, const char *file_name) {
  int bytes_needed = snprintf(dest_path, MAX_PATH_LEN, "%s%s%s/%d/%s", getenv("HOME"), SIG8_PATH, app_name, crash_id,
                              file_name);
  if (bytes_needed >= MAX_PATH_LEN) {
    return 0;
  }
  return 1;
}

int create_dir_path(char *dest_path, char *app_name, int crash_id) {
  int bytes_needed = snprintf(dest_path, MAX_PATH_LEN, "%s%s%s/%d", getenv("HOME"), SIG8_PATH, app_name, crash_id);
  if (bytes_needed >= MAX_PATH_LEN) {
    return 0;
  }
  return 1;
}

int create_logdir(char *app_name, int crash_id) {
  int err = create_dir_path(log_dir_path, app_name, crash_id);
  err += create_file_path(handlerlog_path, app_name, crash_id, HANDLER_LOG);
  err += create_file_path(crash_path, app_name, crash_id, CRASH_LOG);
  err += create_file_path(sample_path, app_name, crash_id, SAMPLE_LOG);
  if (err != 4) {
    // Failed to generate some log file paths. Return.
    return 0;
  }

  // Create all intermediate directories with 744 permissions
  mode_t mode = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;
  if (mkpath(log_dir_path, mode) != 0) {
    // Unable to make directories. Return.
    return 0;
  }

  return 1;
}

// Copy a c# string into a null terminated char buffer (more of a util than an fsutil).
int copy_string(char** dest, int* destLen, char* src, int srcLen) {
  // Make a copy to avoid the string getting GC'ed. Null terminate for ease of use.
  *dest = (char*) malloc(srcLen + 1);
  if (*dest == 0) {
    return 0;
  }
  memcpy(*dest, src, srcLen);
  (*dest)[srcLen] = '\0';
  *destLen = srcLen;
  return 1;
}
