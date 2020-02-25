#ifndef LOGGER
#define LOGGER


typedef enum LogLevel {

    INFO, WARNING, ERROR

} LogLevel;

void set_log_level(LogLevel new_level);

//everything
void log_info(char* msg);

//only warnings and errors
void log_warn(char* msg);

//only errors
void log_err(char* msg);

void log_err_detail(char* msg, char* detail, int pos);

#endif
