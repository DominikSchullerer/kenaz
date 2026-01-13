#ifndef _MEM_MANAGER_H_
    #define _MEM_MANAGER_H_

    #include "defines.h"

    typedef struct mem_manager_t mem_manager_t;
    typedef struct mem_manager_fns_t mem_manager_fns_t;
    typedef struct logger_t logger_t;

    typedef enum memory_tag_t {
        // For temporary use. Should be assigned one of the below or have a new tag created.
        MEMORY_TAG_UNKNOWN = 0,
        MEMORY_TAG_ENGINE,
        MEMORY_TAG_MEM_MANAGER,
        
        MEMORY_TAG_COUNT
    } memory_tag_t;

    typedef void* (*mem_manager_allocate_fn)(mem_manager_t *mem_manager, uint64_t  size, memory_tag_t tag);
    typedef void (*mem_manager_release_fn)(mem_manager_t *mem_manager, void *block, uint64_t  size, memory_tag_t tag);
    typedef void (*mem_manager_shutdown_fn)(mem_manager_t *mem_manager);
    typedef void (*mem_manager_log_fn)(mem_manager_t *mem_manager);

    struct mem_manager_fns_t {
        mem_manager_allocate_fn allocate;
        mem_manager_release_fn release;
        mem_manager_shutdown_fn shutdown;
        mem_manager_log_fn log;
    };

    struct mem_manager_t {
        void *data;
        const mem_manager_fns_t *fns;
    };

    bool8_t std_mem_manager_init(mem_manager_t* mem_manager, logger_t *logger);

#endif // _MEM_MANAGER_H_