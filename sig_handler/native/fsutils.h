#ifndef FSUTILS_H
#define FSUTILS_H

static int mkpath(const char *dir, mode_t mode);

int create_file_path(char *dest_path, char *app_name, int crash_id, const char *file_name);

int create_dir_path(char *dest_path, char *app_name, int crash_id);

int create_logdir(char *app_name, int crash_id);

int copy_string(char** dest, int* destLen, char* src, int srcLen);

#define MAX_PATH_LEN 1024
#define MAX_CRASH_LEN 4096

char log_dir_path[MAX_PATH_LEN];
char handlerlog_path[MAX_PATH_LEN];
char sample_path[MAX_PATH_LEN];
char crash_path[MAX_PATH_LEN];

/*
   Dir structure
  ~/Library/Application Support/SIG8
  ~/Library/Application Support/SIG8/<app_name_1>
  ~/Library/Application Support/SIG8/<app_name_1>/<crash_id_1>                       <-- log_dir_path
  ~/Library/Application Support/SIG8/<app_name_1>/<crash_id_1>/sig_handler.txt       <-- handlerlog_path
  ~/Library/Application Support/SIG8/<app_name_1>/<crash_id_1>/crash.txt             <-- crash_path
  ~/Library/Application Support/SIG8/<app_name_1>/<crash_id_1>/sample.txt            <-- sample_path
  ~/Library/Application Support/SIG8/<app_name_1>/<crash_id_2>
  ~/Library/Application Support/SIG8/<app_name_1>/<crash_id_2>/sig_handler.txt
  ~/Library/Application Support/SIG8/<app_name_1>/<crash_id_2>/crash.txt
  ~/Library/Application Support/SIG8/<app_name_1>/<crash_id_2>/sample.txt
  ~/Library/Application Support/SIG8/<app_name_1>/...
  ~/Library/Application Support/SIG8/<app_name_2>/...
*/




#endif //FSUTILS_H
