#ifndef HECTOR
#define HECTOR

#include <stddef.h>
#include <stdlib.h>

/**
 * Hector is a basic vector implementation
 */

struct Hector {
    void *mem;
    size_t capacity;
    size_t length;
    size_t elem_size;
};

struct Hector *hector_create(size_t elem_size, size_t init_size);

void hector_push(struct Hector *hec, void *item);
void *hector_pop(struct Hector *hec);
void *hector_get(struct Hector *hec, size_t n);
void hector_splice(struct Hector *hec, size_t n, size_t count);
size_t hector_size(struct Hector *hec);

#endif
