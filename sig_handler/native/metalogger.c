#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


#define MAX_LOG_LEN 4096

struct MetaLogger {
  FILE *file;
};

int ml_create(char *path, struct MetaLogger *ml) {
  FILE *meta_log;
  if ((meta_log = fopen(path, "w+")) == NULL) {
    return 0;
  }

  struct MetaLogger new_ml;
  new_ml.file = meta_log;
  *ml = new_ml;

  return 1;
}

void ml_destroy(struct MetaLogger ml) {
  fclose(ml.file);
}

// Forgive the code duplication here. Not sure how to make va_list va_start work two layers deep...

void ml_log(struct MetaLogger ml, const char *format, ...) {
  va_list args;
  va_start(args, format);
  pid_t pid = getpid();

  char log_line[MAX_LOG_LEN];
  int bytes_needed = snprintf(&log_line[0], MAX_LOG_LEN, "%ld\t%d\tLOG\t", time(NULL), pid);
  vsnprintf(&log_line[bytes_needed], MAX_LOG_LEN - bytes_needed, format, args);
  fprintf(ml.file, log_line, args);
  fflush(ml.file);
  va_end(args);}

void ml_err(struct MetaLogger ml, const char *format, ...) {
  va_list args;
  va_start(args, format);
  pid_t pid = getpid();

  char log_line[MAX_LOG_LEN];
  int bytes_needed = snprintf(&log_line[0], MAX_LOG_LEN, "%ld\t%d\tERR\t", time(NULL), pid);
  vsnprintf(&log_line[bytes_needed], MAX_LOG_LEN - bytes_needed, format, args);
  fprintf(ml.file, log_line, args);
  fflush(ml.file);
  va_end(args);
}
