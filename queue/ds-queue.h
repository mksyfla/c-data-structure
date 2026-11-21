#ifndef DS_QUEUE_H
#define DS_QUEUE_H

#include "../errors/ds-error.h"
#include <stddef.h>
#include <stdbool.h>

typedef struct queue_node queue_node_t;
typedef struct ds_queue queue_t;
typedef void (*free_fn)(void *ptr);

// Lifecycle
queue_t *queue_create(free_fn free_fn);
void queue_destroy(queue_t *queue);
void queue_clear(queue_t *queue);

// Capacity & Info
size_t queue_size(const queue_t *queue);
bool queue_is_empty(const queue_t *queue);

// Modifiers
status_t queue_enqueue(queue_t *queue, void *item);
status_t queue_dequeue(queue_t *queue, void **out_item);

// Access
status_t queue_peek(const queue_t *queue, void **out_item);

#endif
