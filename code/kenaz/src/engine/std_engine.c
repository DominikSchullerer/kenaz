#include "engine.h"

#include "core/logger/logger.h"
#include "core/mem_manager/mem_manager.h"

typedef struct engine_data_t engine_data_t;

struct engine_data_t {
    bool8_t is_initialized;
};


static void std_engine_shutdown(engine_t *engine);

static void std_engine_noop_shutdown(engine_t *engine);


static const engine_fns_t std_engine_fns = {
    .shutdown = std_engine_shutdown,
};


static const engine_fns_t std_engine_noop_fns = {
    .shutdown = std_engine_noop_shutdown,
};


bool8_t std_engine_init(engine_t *engine)
{
    logger_t *logger = &engine->logger;
    mem_manager_t *mem_manager = &engine->mem_manager;

    K_LOG_INFO(logger, "engine: Initializing");

    engine_data_t *data = mem_manager->fns->allocate(mem_manager, sizeof(*data), MEMORY_TAG_ENGINE);
    if (data == NULL) {
        K_LOG_FATAL(logger, "engine: Could not initialize");
        return FALSE;
    }

    data->is_initialized = TRUE;

    engine->data = data;
    engine->fns = &std_engine_fns;

    K_LOG_INFO(logger, "engine: Successfully initialized");
    return TRUE;
}


static void std_engine_shutdown(engine_t *engine)
{
    engine_data_t *data = engine->data;
    logger_t *logger = &engine->logger;
    mem_manager_t *mem_manager = &engine->mem_manager;

    K_LOG_INFO(logger, "engine: Shutting down");

    mem_manager->fns->release(mem_manager, data, sizeof(*data), MEMORY_TAG_ENGINE);
    engine->data = NULL;
    engine->fns = &std_engine_noop_fns;

    K_LOG_INFO(logger, "engine: Successfully shut down");
}


static void std_engine_noop_shutdown(engine_t *engine)
{
    ;
}
