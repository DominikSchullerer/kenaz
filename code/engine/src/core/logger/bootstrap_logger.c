#include "core/logger/logger.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>


#define MSG_LENGTH          32000
#define MSG_PREFIX_LENGTH   32
#define MSG_FINAL_LENGTH    (MSG_LENGTH + MSG_PREFIX_LENGTH)


static void bootstrap_logger_log(logger_t *logger, log_level_e log_level, const char *message, ...);
static void bootstrap_logger_shutdown(logger_t *logger);

static void bootstrap_logger_noop_log(logger_t *logger, log_level_e log_level, const char *message, ...);
static void bootstrap_logger_noop_shutdown(logger_t *logger);


static const logger_fns_t bootstrap_logger_fns = {
    .log = bootstrap_logger_log,
    .shutdown = bootstrap_logger_shutdown
};

static const logger_fns_t bootstrap_logger_noop_fns = {
    .log = bootstrap_logger_noop_log,
    .shutdown = bootstrap_logger_noop_shutdown
};


const char level_strings[LOG_LEVEL_COUNT][MSG_PREFIX_LENGTH] = {
    "[FATAL]: ",
    "[ERROR]: ",
    "[WARN]:  ",
    "[INFO]:  ",
    "[DEBUG]: ",
    "[TRACE]: "
};


bool8_t bootstrap_logger_init(logger_t *logger)
{
    logger->data = NULL;
    logger->fns = &bootstrap_logger_fns;
    return TRUE;
}


static void bootstrap_logger_log(logger_t *logger, log_level_e log_level, const char *message, ...)
{
    bool8_t is_error = log_level <= LOG_LEVEL_ERROR;
    char out_message[MSG_LENGTH];

    __builtin_va_list arg_ptr;
    va_start(arg_ptr, message);
    vsnprintf(out_message, MSG_LENGTH - 1, message, arg_ptr);
    va_end(arg_ptr);

    char final_message[MSG_FINAL_LENGTH];
    snprintf(final_message, MSG_FINAL_LENGTH, "%s%s\n", level_strings[log_level], out_message);
    
    // platform-specific output handling
    if (is_error) {
        fprintf(stderr, "%s", final_message);
    } 
    else {
        fprintf(stdout, "%s", final_message);
    }
}


static void bootstrap_logger_shutdown(logger_t *logger)
{
    logger->fns = &bootstrap_logger_noop_fns;
}


static void bootstrap_logger_noop_log(logger_t *logger, log_level_e log_level, const char *message, ...)
{
    (void)logger;
    (void)log_level;
    (void)message;
}


static void bootstrap_logger_noop_shutdown(logger_t *logger)
{
    (void)logger;
}