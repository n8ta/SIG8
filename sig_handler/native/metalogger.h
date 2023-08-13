#ifndef NATIVE_METALOGGER_H
#define NATIVE_METALOGGER_H

// The crash handler's own logs
struct MetaLogger {
  FILE* file;
};

int ml_create(char* path, struct MetaLogger * ml);

void ml_destroy(struct MetaLogger ml);

// Informational logs
void ml_log(struct MetaLogger ml, char* format, ...);

// Exceptions
void ml_err(struct MetaLogger ml, char* format, ...);

#endif //NATIVE_METALOGGER_H
