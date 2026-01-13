#include "engine/engine.h"

#include <Windows.h>

static bool8_t init_engine(engine_t *engine);

int main()
{
    
    engine_t engine = { 0 };
    bool8_t res = TRUE;
    
    res = init_engine(&engine);

    logger_t *logger = &engine.logger;
    mem_manager_t *mem_manager = &engine.mem_manager;

    engine.fns->shutdown(&engine);
    mem_manager->fns->shutdown(mem_manager);
    logger->fns->shutdown(logger);

    return 0;
}


static bool8_t init_engine(engine_t *engine)
{
    bool8_t res = TRUE;
    
    logger_t *logger = &engine->logger;
    mem_manager_t *mem_manager = &engine->mem_manager;
    
    if (res) {
        res = bootstrap_logger_init(logger);
    }
    
    if (res) {
        res = std_mem_manager_init(mem_manager, logger);
    }
    
    if (res) {
        res = std_engine_init(engine);
    }
    
    return res;
}