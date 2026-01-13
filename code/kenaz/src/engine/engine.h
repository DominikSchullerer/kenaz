#ifndef _ENGINE_H_
    #define _ENGINE_H_

    #include "defines.h"

    #include "core/logger/logger.h"
    #include "core/mem_manager/mem_manager.h"

    typedef struct engine_t engine_t;
    typedef struct engine_fns_t engine_fns_t;

    typedef struct logger_t logger_t;
    typedef struct mem_manager_t mem_manager_t;


    typedef void (*engine_shutdown_fn)(engine_t *engine);
    
    struct engine_fns_t {
        engine_shutdown_fn shutdown;
    };

    struct engine_t {
        void *data;
        const engine_fns_t *fns;
        mem_manager_t mem_manager;
        logger_t logger;
    };
    
    bool8_t std_engine_init(engine_t *engine);

#endif // _ENGINE_H_