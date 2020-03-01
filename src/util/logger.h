#ifndef LOGGER_H
#define LOGGER_H


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

#endif
