#ifndef RING_H
#define RING_H

#include <stdint.h>
#include <stdio.h>

typedef struct{
    size_t s_elem;
    uint16_t n_elem;
    void *buf;
    volatile uint16_t head;
    volatile uint16_t tail;
} ring_t;

int ring_buffer_init(ring_t *ring, void *buf, size_t buf_elements_size, size_t arrayByteSize);

int ring_buffer_full(ring_t *ring);
int ring_buffer_empty(ring_t *ring);

void memcopy(void *destref, const void *srcref, size_t count);

int ring_buffer_put(ring_t *ring, const void *data);
int ring_buffer_get(ring_t *ring, void *data);

#endif
