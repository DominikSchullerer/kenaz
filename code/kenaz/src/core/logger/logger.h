#ifndef _LOGGER_H_
    #define _LOGGER_H_

    #include "defines.h"


    #define LOG_WARN_ENBALED 1
    #define LOG_INFO_ENABLED 1
    #if VRELEASE == 1
        #define LOG_DEBUG_ENABLED 0
        #define LOG_TRACE_ENABLED 0
    #else
        #define LOG_DEBUG_ENABLED 1
        #define LOG_TRACE_ENABLED 1
    #endif


    typedef struct logger_t logger_t;
    typedef struct logger_fns_t logger_fns_t;

    typedef enum log_level_e {
        LOG_LEVEL_FATAL,
        LOG_LEVEL_ERROR,
        LOG_LEVEL_WARN,
        LOG_LEVEL_INFO,
        LOG_LEVEL_DEBUG,
        LOG_LEVEL_TRACE,
        
        LOG_LEVEL_COUNT
    } log_level_e;

    typedef void (*logger_log_fn)(logger_t *logger, log_level_e log_level, const char *message, ...);
    typedef void (*logger_shutdown_fn)(logger_t *logger);

    struct logger_fns_t {
        logger_log_fn log;
        logger_shutdown_fn shutdown;
    };

    struct logger_t {
        void *data;
        const logger_fns_t *fns;
    };

    bool8_t bootstrap_logger_init(logger_t *logger);


    #define K_LOG_FATAL(logger, msg, ...)                                       \
        do {                                                                    \
            logger->fns->log(logger, LOG_LEVEL_FATAL, msg, ##__VA_ARGS__);      \
        } while(0)


    #define K_LOG_ERROR(logger, msg, ...)                                       \
        do {                                                                    \
            logger->fns->log(logger, LOG_LEVEL_ERROR, msg, ##__VA_ARGS__);      \
        } while(0)   


    #if LOG_WARN_ENBALED == 1
        #define K_LOG_WARN(logger, msg, ...)                                    \
        do {                                                                    \
            logger->fns->log(logger, LOG_LEVEL_WARN, msg, ##__VA_ARGS__);       \
        } while(0)   
    #else
        #define K_LOG_WARN(logger, msg, ...)
    #endif


    #if LOG_INFO_ENABLED == 1 
        #define K_LOG_INFO(logger, msg, ...)                                    \
        do {                                                                    \
            logger->fns->log(logger, LOG_LEVEL_INFO, msg, ##__VA_ARGS__);       \
        } while(0)   
    #else
        #define K_LOG_INFO(logger, msg, ...)
    #endif


    #if LOG_DEBUG_ENABLED == 1
        #define K_LOG_DEBUG(logger, msg, ...)                                   \
        do {                                                                    \
            logger->fns->log(logger, LOG_LEVEL_DEBUG, msg, ##__VA_ARGS__);      \
        } while(0)   
    #else
        #define K_LOG_DEBUG(logger, msg, ...)
    #endif


    #if LOG_TRACE_ENABLED == 1
        #define K_LOG_TRACE(logger, msg, ...)                                   \
        do {                                                                    \
            logger->fns->log(logger, LOG_LEVEL_TRACE, msg, ##__VA_ARGS__);      \
        } while(0)   
    #else
        #define K_LOG_TRACE(logger, msg, ...)
    #endif


#endif // _LOGGER_H_