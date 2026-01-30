#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

#define SMALL_BLOCK_SIZE   15   // размер маленького блока
#define LARGE_BLOCK_SIZE  180   // размер большого блока
#define SMALL_POOL_SIZE   128   // кол-во малых блоков в пуле
#define LARGE_POOL_SIZE    32   // кол-во больших блоков в пуле

/**
* malloc - выделяет блок памяти заданного размера
* @size: размер блока в байтах
* Возвращает указатель на выделенный блок или NULL, если память закончилась
*/
void* malloc(size_t size);

/**
* free - освобождает ранее выделенный блок памяти
* @ptr: указатель на блок, ранее выделенный malloc
*/
void free(void* ptr);

#endif // ALLOCATOR_H
