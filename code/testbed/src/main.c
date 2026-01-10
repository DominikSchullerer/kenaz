#include <core/logger/logger.h>


static void test_logger(logger_t *logger);


int main()
{
    bool8_t res = TRUE;

    logger_t bootstrap_logger = { 0 };
    res = bootstrap_logger_init(&bootstrap_logger);
    if (!res) {
        return -1;
    }

    test_logger(&bootstrap_logger);

    
    bootstrap_logger.fns->shutdown(&bootstrap_logger);

    return 0;
}


static void test_logger(logger_t *logger)
{
    K_LOG_FATAL(logger, "This is a fatal message. %f", 3.14);
    K_LOG_ERROR(logger, "This is an error message. %d", 2);
    K_LOG_WARN(logger, "This is a warning message. %s", "This is a test string");
    K_LOG_INFO(logger, "This is an info message. %.2f", 0.577);
    K_LOG_DEBUG(logger, "This is a debug message. %f", 1.414);
    K_LOG_TRACE(logger, "This is a trace message. %f", 2.236);
}