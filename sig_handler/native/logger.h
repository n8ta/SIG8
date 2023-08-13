#ifndef NATIVE_LOGGER_H
#define NATIVE_LOGGER_H

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int log_crash(char* app_name, char* crash_app_path, pid_t pid, int signo, time_t time, int crash_id);

#endif //NATIVE_LOGGER_H
