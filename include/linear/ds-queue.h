#ifndef DS_QUEUE_H
#define DS_QUEUE_H

#include "../core/ds-types.h"
#include <stddef.h>
#include <stdbool.h>

typedef struct queue_node queue_node_t;
typedef struct ds_queue queue_t;

// Lifecycle
queue_t *queue_create(free_fn free_fn, cmp_fn cmp);
void queue_destroy(queue_t *queue);
void queue_clear(queue_t *queue);

// Capacity & Info
size_t queue_size(const queue_t *queue);
bool queue_is_empty(const queue_t *queue);

// Modifiers
status_t queue_enqueue(queue_t *queue, void *item);
status_t queue_dequeue(queue_t *queue, void **out_item);

// Access
status_t queue_peek_front(const queue_t *queue, void **out_item);
status_t queue_peek_rear(const queue_t *queue, void **out_item);
bool queue_contains(const queue_t *queue, const void *item);

#endif
