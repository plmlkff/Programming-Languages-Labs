//
// Created by Павел Мальков on 28.12.2023.
//

#ifndef MEMORY_ALLOCATOR_TESTS_H
#define MEMORY_ALLOCATOR_TESTS_H

#include <assert.h>
#include <stdio.h>
#include <sys/mman.h>

#include "../mem.h"
#include "../mem_internals.h"

#include "../util.h"

#define TEST_HEAP_SIZE 2048
#define BASE_ALLOC_SIZE 200

void run_tests();

#endif //MEMORY_ALLOCATOR_TESTS_H
