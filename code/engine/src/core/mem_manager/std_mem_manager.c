#include "core/mem_manager/mem_manager.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "core/logger/logger.h"

#define KB                          1024
#define MB                          (1024 * KB)
#define GB                          (1024 * MB)

#define LOG_TOTAL_STRING_LENGTH     8000
#define LOG_UNIT_STRING_LENGTH      4


typedef struct std_mem_manager_data_t std_mem_manager_data_t;

struct std_mem_manager_data_t {
    uint64_t total_allocated;
    uint64_t tagged_allocations[MEMORY_TAG_COUNT];
    logger_t *logger;
};

static void* std_mem_manager_allocate(mem_manager_t *mem_manager, uint64_t  size, memory_tag_t tag);
static void std_mem_manager_release(mem_manager_t *mem_manager, void *block, uint64_t  size, memory_tag_t tag);
static void std_mem_manager_shutdown(mem_manager_t *mem_manager);
static void std_mem_manager_log(mem_manager_t *mem_manager);

static void* std_mem_manager_noop_allocate(mem_manager_t *mem_manager, uint64_t  size, memory_tag_t tag);
static void std_mem_manager_noop_release(mem_manager_t *mem_manager, void *block, uint64_t  size, memory_tag_t tag);
static void std_mem_manager_noop_shutdown(mem_manager_t *mem_manager);
static void std_mem_manager_noop_log(mem_manager_t *mem_manager);


static const mem_manager_fns_t std_mem_manager_fns = {
    .allocate = std_mem_manager_allocate,
    .release = std_mem_manager_release,
    .shutdown = std_mem_manager_shutdown,
    .log = std_mem_manager_log
};

static const mem_manager_fns_t std_mem_manager_noop_fns = {
    .allocate = std_mem_manager_noop_allocate,
    .release = std_mem_manager_noop_release,
    .shutdown = std_mem_manager_noop_shutdown,
    .log = std_mem_manager_noop_log
};

static const char* memory_tag_strings[MEMORY_TAG_COUNT] = {
    "UNKNOWN          ",
    "ENGINE           ",
    "MEM_MANAGER      "
};


bool8_t std_mem_manager_init(mem_manager_t* mem_manager, logger_t *logger)
{
    K_LOG_INFO(logger, "std_mem_manger: Initializing");

    std_mem_manager_data_t *data = calloc(1, sizeof(*data));
    if (data == NULL) {
        K_LOG_FATAL(logger, "std_mem_manager: Could not initialize");
        return FALSE;
    }
    
    data->total_allocated += sizeof(*data);
    data->tagged_allocations[MEMORY_TAG_MEM_MANAGER] += sizeof(*data);
    data->logger = logger;

    mem_manager->data = data;
    mem_manager->fns = &std_mem_manager_fns;

    K_LOG_INFO(logger, "std_mem_manger: Succesfully initialized");

    return TRUE;
}


static void* std_mem_manager_allocate(mem_manager_t *mem_manager, uint64_t  size, memory_tag_t tag)
{
    std_mem_manager_data_t *data = mem_manager->data;
    logger_t *logger = data->logger;

    if (tag == MEMORY_TAG_UNKNOWN) {
        K_LOG_WARN(logger, "std_mem_manager: Allocatition with MEMORY_TAG_UNKNOWN. Re-classify the allocation.");
    }

    void* block = calloc(1, size);
    if (block == NULL) {
        K_LOG_ERROR(logger, "std_mem_manger: Allocation failed (size: %d)", size);
        return NULL;
    }

    data->total_allocated += size;
    data->tagged_allocations[tag] += size;

    return block;
}

static void std_mem_manager_release(mem_manager_t *mem_manager, void *block, uint64_t  size, memory_tag_t tag)
{
    std_mem_manager_data_t *data = mem_manager->data;
    logger_t *logger = data->logger;

    if (tag == MEMORY_TAG_UNKNOWN) {
        K_LOG_WARN(logger, "std_mem_manager: Release with MEMORY_TAG_UNKNOWN. Re-classify the allocation.");
    }

    data->total_allocated -= size;
    data->tagged_allocations[tag] -= size;
    free(block);
}

static void std_mem_manager_shutdown(mem_manager_t *mem_manager)
{
    std_mem_manager_data_t *data = mem_manager->data;
    logger_t *logger = data->logger;

    K_LOG_INFO(logger, "std_mem_mangaer: Shutting down");

    data->tagged_allocations[MEMORY_TAG_MEM_MANAGER] -= sizeof(*data);
    data->total_allocated -= sizeof(*data);

    if (data->total_allocated != 0) {
        K_LOG_ERROR(logger, "std_mem_manger: Unreleased memory at shut down");
        mem_manager->fns->log(mem_manager);
    }

    free(mem_manager->data);
    mem_manager->data = NULL;
    mem_manager->fns = &std_mem_manager_noop_fns;
    
    K_LOG_INFO(logger, "std_mem_mangaer: Successfully shut down");
}

// Todo: Own string library
static void std_mem_manager_log(mem_manager_t *mem_manager)
{
    std_mem_manager_data_t *data = mem_manager->data;
    logger_t *logger = data->logger;

    char buffer[LOG_TOTAL_STRING_LENGTH] = "std_mem_manger: System memory use(tagged):\n";
    uint64_t offset = strlen(buffer);

    for (uint32_t i = 0; i < MEMORY_TAG_COUNT; ++i) {
        char unit[LOG_UNIT_STRING_LENGTH] = "";
        float32_t amount = 1.0f;

        if (data->tagged_allocations[i] >= GB) {
            amount = (float32_t)data->tagged_allocations[i] / (float32_t)GB;
            snprintf(unit, LOG_UNIT_STRING_LENGTH, "GB");
        } else if (data->tagged_allocations[i] >= MB) {
            amount = (float32_t)data->tagged_allocations[i] / (float32_t)MB;
            snprintf(unit, LOG_UNIT_STRING_LENGTH, "MB");
        } else if (data->tagged_allocations[i] >= KB) {
            amount = (float32_t)data->tagged_allocations[i] / (float32_t)KB;
            snprintf(unit, LOG_UNIT_STRING_LENGTH, "KB");
        } else {
            amount = (float32_t)data->tagged_allocations[i];
            snprintf(unit, LOG_UNIT_STRING_LENGTH, "B");
        }

        int32_t length = snprintf(buffer + offset, 8000,
                 "  %s : %.2f %s\n",
                 memory_tag_strings[i],
                 amount,
                 unit);

        offset += length;
    }

    K_LOG_DEBUG(logger, "%s", buffer);
}


static void* std_mem_manager_noop_allocate(mem_manager_t *mem_manager, uint64_t  size, memory_tag_t tag)
{
    return NULL;
}

static void std_mem_manager_noop_release(mem_manager_t *mem_manager, void *block, uint64_t  size, memory_tag_t tag)
{
    ;
}

static void std_mem_manager_noop_shutdown(mem_manager_t *mem_manager)
{
    ;
}

static void std_mem_manager_noop_log(mem_manager_t *mem_manager)
{
    ;
}