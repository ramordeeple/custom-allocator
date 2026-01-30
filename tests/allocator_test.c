/**
 * test_allocator.c
 *
 * Тесты для кастомного аллокатора памяти с фиксированными блоками.
 * Проверяет:
 * 1. Выделение и освобождение малых и больших блоков
 * 2. Корректность записи и чтения данных
 * 3. Повторное выделение после free
 * 4. Работа при исчерпании памяти
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "../include/allocator.h"

/**
 * Глобальные массивы для тестов
 */
void* small_blocks[SMALL_POOL_SIZE];
void* large_blocks[LARGE_POOL_SIZE];

bool test_small_blocks(void);
bool test_large_blocks(void);
bool test_data_integrity(void);
bool test_free_and_realloc(void);
bool test_exhaustion(void);

int main(void) {
    printf("=== Запуск тестов кастомного аллокатора ===\n\n");

    if (!test_small_blocks()) return EXIT_FAILURE;
    if (!test_large_blocks()) return EXIT_FAILURE;
    if (!test_data_integrity()) return EXIT_FAILURE;
    if (!test_free_and_realloc()) return EXIT_FAILURE;
    if (!test_exhaustion()) return EXIT_FAILURE;

    printf("\n=== Все тесты завершены успешно ===");

    return EXIT_SUCCESS;
}

/**
 * Тест выделения малых блоков
 */
bool test_small_blocks(void) {
    printf("Тест: выделение малых блоков (%d байт)...\n", SMALL_BLOCK_SIZE);
    for (int i = 0; i < SMALL_POOL_SIZE; i++) {
        small_blocks[i] = malloc(SMALL_BLOCK_SIZE);

        if (!small_blocks[i]) {
            printf("Ошибка: не удалось выделить малый блок %d\n\n", i);
            return false;
        }

        memset(small_blocks[i], i, SMALL_BLOCK_SIZE); // записываем тестовые данные
    }

    printf("Все малые блоки успешно выделены.\n\n");

    return true;
}

/**
 * Тест выделения больших блоков
 */
bool test_large_blocks(void) {
    printf("Тест: выделение больших блоков (%d байт)...\n", LARGE_BLOCK_SIZE);
    for (int i = 0; i < LARGE_POOL_SIZE; i++) {
        large_blocks[i] = malloc(LARGE_BLOCK_SIZE);

        if (!large_blocks[i]) {
            printf("Ошибка: не удалось выделить большой блок %d\n\n", i);
            return false;
        }

        memset(large_blocks[i], i + 100, LARGE_BLOCK_SIZE); // тестовые данные
    }

    printf("Все большие блоки успешно выделены.\n\n");

    return true;
}

/**
 * Проверка целостности данных в блоках
 */
bool test_data_integrity(void) {
    for (int i = 0; i < SMALL_POOL_SIZE; i++) {
        uint8_t* p = (uint8_t*)small_blocks[i];

        for (int j = 0; j < SMALL_BLOCK_SIZE; j++) {
            if (p[j] != i) {
                printf("Ошибка: данные в малом блоке %d повреждены\n\n", i);
                return false;
            }
        }
    }

    for (int i = 0; i < LARGE_POOL_SIZE; i++) {
        uint8_t* p = (uint8_t*)large_blocks[i];

        for (int j = 0; j < LARGE_BLOCK_SIZE; j++) {
            if (p[j] != i + 100) {
                printf("Ошибка: данные в большом блоке %d повреждены\n\n", i);
                return false;
            }
        }
    }

    printf("Данные в блоках корректны.\n\n");

    return true;
}

/**
 * Тест free и повторного выделения
 */
bool test_free_and_realloc(void) {
    printf("Тест: освобождение всех блоков и повторное выделение...\n");

    for (int i = 0; i < SMALL_POOL_SIZE; i++) free(small_blocks[i]);
    for (int i = 0; i < LARGE_POOL_SIZE; i++) free(large_blocks[i]);

    void* test_small = malloc(SMALL_BLOCK_SIZE);
    void* test_large = malloc(LARGE_BLOCK_SIZE);

    if (test_small && test_large) {
        printf("Повторное выделение прошло успешно.\n\n");
        free(test_small);
        free(test_large);
        return true;

    } else {
        printf("Ошибка: повторное выделение не удалось.\n\n");
        return false;
    }
}

/**
 * Тест на исчерпание памяти для малых блоков
 */
bool test_exhaustion(void) {
    printf("Тест: исчерпание памяти малых блоков...\n");
    void* temp_blocks[SMALL_POOL_SIZE];
    int allocated = 0;

    for (int i = 0; i < SMALL_POOL_SIZE; i++) {
        temp_blocks[i] = malloc(SMALL_BLOCK_SIZE);
        if (!temp_blocks[i]) break;
        allocated++;
    }

    if (allocated == SMALL_POOL_SIZE) {
        printf("Все малые блоки успешно выделены.\n");
    }

    else
        printf("Ошибка: не удалось выделить все малые блоки, выделено: %d\n\n", allocated);

    for (int i = 0; i < allocated; i++) free(temp_blocks[i]);

    return allocated == SMALL_POOL_SIZE;
}

