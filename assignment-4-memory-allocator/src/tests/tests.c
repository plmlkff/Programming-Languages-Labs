#include "tests.h"


extern void debug(const char *fmt, ...);

static struct block_header *get_header(void *ptr) {
    return (struct block_header *) ((uint8_t *) ptr - offsetof(struct block_header, contents));
}

void *init_heap_test() {
    void *heap = heap_init(TEST_HEAP_SIZE);
    assert(heap != NULL);
    debug("Heap create, size %d\n", TEST_HEAP_SIZE);
    return heap;
}

void basic_memory_allocation_test(void *heap) {
    void *allocated_block = _malloc(BASE_ALLOC_SIZE);
    assert(allocated_block != NULL);
    debug_heap(stdout, heap);

    _free(allocated_block);
    heap_term();
    debug("Basic memory allocation: [passed]\n\n");
}

void release_one_of_allocated_blocks_test(void *heap) {
    void *b1 = _malloc(BASE_ALLOC_SIZE);
    void *b2 = _malloc(BASE_ALLOC_SIZE * 2);

    _free(b2);
    debug("Heap: ");
    debug_heap(stdout, heap);

    struct block_header *header1 = get_header(b1);
    struct block_header *header2 = get_header(b2);

    assert(header1->next == header2);
    assert(header2->is_free);

    _free(b1);
    heap_term();
    debug("Release of one block out of several: [passed]\n\n");
}

void release_two_of_allocated_blocks_test(void *heap) {
    void *b1 = _malloc(BASE_ALLOC_SIZE);
    void *b2 = _malloc(BASE_ALLOC_SIZE*2);
    void *b3 = _malloc(BASE_ALLOC_SIZE*3);
    assert(b1 != NULL);
    assert(b2 != NULL);
    assert(b3 != NULL);

    debug("Deleting two blocks");
    _free(b1);
    _free(b2);

    assert(b2 != NULL);
    debug("Heap: \n");
    debug_heap(stdout, heap);

    _free(b2);
    heap_term();
    debug("Release of two blocks out of several: [passed]\n\n");
}

void grow_memory_test(void *heap) {
    size_t initial_region_size = ((struct region *) heap)->size;

    void *block = _malloc(5 * TEST_HEAP_SIZE);
    struct block_header *new_block_header = get_header(block);
    size_t expanded_region_size = ((struct region *) heap)->size;

    assert(initial_region_size < expanded_region_size);
    assert(new_block_header);
    assert(new_block_header->capacity.bytes >= 5 * TEST_HEAP_SIZE);
    debug_heap(stdout, heap);

    _free(block);
    heap_term();
    debug("Expansion of existing memory region: [passed]\n\n");
}

void allocate_new_region_test(void *heap) {
    void *hole = mmap(HEAP_START + REGION_MIN_SIZE, BASE_ALLOC_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE, -1, 0);
    assert(hole == HEAP_START + REGION_MIN_SIZE);
    debug_heap(stdout, heap);

    void *new_block = _malloc(REGION_MIN_SIZE - offsetof(struct block_header, contents));
    struct block_header *new_block_header = get_header(new_block);

    assert(new_block != NULL);
    assert(new_block != hole);
    assert(new_block_header->capacity.bytes >= (REGION_MIN_SIZE - offsetof(struct block_header, contents)));

    munmap(hole, BASE_ALLOC_SIZE);
    _free(new_block);
    heap_term();
    debug("Allocation in a new region: [passed]\n\n");
}

void run_tests() {
    void *heap = init_heap_test();
    debug("[Basic memory allocation test]");
    basic_memory_allocation_test(heap);
    heap = init_heap_test();
    debug("[Release one of allocated blocks]");
    release_one_of_allocated_blocks_test(heap);
    heap = init_heap_test();
    debug("[Release two block from many allocated]");
    release_two_of_allocated_blocks_test(heap);
    heap = init_heap_test();
    debug("[Memory expansion on region]");
    grow_memory_test(heap);
    heap = init_heap_test();
    debug("[Allocation in a new region]");
    allocate_new_region_test(heap);
}
