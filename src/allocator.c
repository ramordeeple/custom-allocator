/**
 * Кастомный аллокатор памяти для фиксированных блоков.
 * Поддерживает выделение памяти двух размеров:
 * - Малые блоки: 15 байт
 * - Большие блоки: 180 байт
 *
 */

#include "allocator.h"
#include <stdint.h>
#include <stddef.h>

/**
 * Внутренние структуры
 */
typedef struct Block {
    struct Block* next;
} Block;

/**
 * Пулы блоков
 */
static uint8_t small_pool[SMALL_BLOCK_SIZE * SMALL_POOL_SIZE];
static uint8_t large_pool[LARGE_BLOCK_SIZE * LARGE_POOL_SIZE];

/**
 * Свободные блоки
 */
static Block* free_small = NULL;
static Block* free_large = NULL;

/**
 * init_pools — создаёт связные списки свободных блоков для каждого пула.
 * Вызывается при первом malloc, чтобы не тратить память заранее.
 */
static void init_pools(void) {
    if (!free_small) {
        for (int i = 0; i < SMALL_POOL_SIZE; i++) {
            Block* blk = (Block*)(small_pool + i * SMALL_BLOCK_SIZE);
            blk->next = free_small;
            free_small = blk;
        }
    }

    if (!free_large) {
        for (int i = 0; i < LARGE_POOL_SIZE; i++) {
            Block* blk = (Block*)(large_pool + i * LARGE_BLOCK_SIZE);
            blk->next = free_large;
            free_large = blk;
        }
    }
}

void* malloc(size_t size) {
    init_pools();

    Block* blk = NULL;

    if (size <= SMALL_BLOCK_SIZE) {
        if (!free_small) return NULL;  // пул исчерпан
        blk = free_small;
        free_small = free_small->next;
    }

    else if (size <= LARGE_BLOCK_SIZE) {
        if (!free_large) return NULL;
        blk = free_large;
        free_large = free_large->next;
    }

    /**
     * блоки больших размеров не поддерживаются
     */
    else return NULL;

    return (void*)blk;
}

void free(void* ptr) {
    if (!ptr) return;

    uintptr_t addr = (uintptr_t)ptr;

    // проверка, к какому пулу относится блок
    if (addr >= (uintptr_t)small_pool &&
        addr < (uintptr_t)(small_pool + sizeof(small_pool))) {
        Block* blk = (Block*)ptr;
        blk->next = free_small;
        free_small = blk;
    }

    else if (addr >= (uintptr_t)large_pool &&
               addr < (uintptr_t)(large_pool + sizeof(large_pool))) {
        Block* blk = (Block*)ptr;
        blk->next = free_large;
        free_large = blk;
    }
}
