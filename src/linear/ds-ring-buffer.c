#include "../../include/linear/ds-ring-buffer.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

struct ds_ring_buffer {
  void **data;
  size_t size;
  size_t capacity;
  size_t front;
  size_t rear;
  free_fn free_fn;
};

// Lifecycle
ring_buffer_t *ring_buffer_create(size_t initial_cap, free_fn free_fn) {
  if (free_fn == NULL)
    return NULL;

  ring_buffer_t *buffer = malloc(sizeof(ring_buffer_t));
  if (buffer == NULL)
    return NULL;

  initial_cap = initial_cap < 1 ? vector_DEFAULT_CAP : initial_cap;

  buffer->data = calloc(initial_cap, sizeof(*buffer->data));
  if (buffer->data == NULL) {
    free(buffer);
    return NULL;
  }

  buffer->capacity = initial_cap;
  buffer->free_fn = free_fn;
  buffer->size = buffer->front = buffer->rear = 0;
  return buffer;
}

void ring_buffer_destroy(ring_buffer_t *buffer) {
  if (buffer == NULL)
    return;

  ring_buffer_clear(buffer);
  free(buffer->data);
  free(buffer);
}

void ring_buffer_clear(ring_buffer_t *buffer) {
  if (buffer == NULL)
    return;

  if (buffer->free_fn != NULL) {
    for (size_t i = 0; i < buffer->size; i++) {
      size_t idx = (buffer->front + i) % buffer->capacity;
      if (buffer->data[idx] != NULL)
        buffer->free_fn(buffer->data[idx]);
    }
  }

  buffer->size = buffer->front = buffer->rear = 0;
}

// Capacity & Info
size_t ring_buffer_size(const ring_buffer_t *buffer) {
  return buffer == NULL ? 0 : buffer->size;
}

size_t ring_buffer_capacity(const ring_buffer_t *buffer) {
  return buffer == NULL ? 0 : buffer->capacity;
}

bool ring_buffer_is_empty(const ring_buffer_t *buffer) {
  return buffer == NULL || buffer->size == 0;
}

bool ring_buffer_is_full(const ring_buffer_t *buffer) {
  return buffer != NULL && buffer->size == buffer->capacity;
}

// Modifiers
status_t ring_buffer_enqueue(ring_buffer_t *buffer, void *item) {
  if (buffer == NULL || item == NULL)
    return DS_ERR_INVALID_ARG;

  if (buffer->size == buffer->capacity)
    return DS_ERR_FULL;

  buffer->data[buffer->rear] = item;
  buffer->rear = (buffer->rear + 1) % buffer->capacity;
  buffer->size++;
  return DS_OK;
}

status_t ring_buffer_dequeue(ring_buffer_t *buffer, void **out_item) {
  if (buffer == NULL || out_item == NULL)
    return DS_ERR_INVALID_ARG;

  if (buffer->size == 0)
    return DS_ERR_EMPTY;

  *out_item = buffer->data[buffer->front];
  buffer->data[buffer->front] = NULL;
  buffer->front = (buffer->front + 1) % buffer->capacity;
  buffer->size--;
  return DS_OK;
}

// Access
status_t ring_buffer_peek_front(const ring_buffer_t *buffer, void **out_item) {
  if (buffer == NULL || out_item == NULL)
    return DS_ERR_INVALID_ARG;

  if (buffer->size == 0)
    return DS_ERR_EMPTY;

  *out_item = buffer->data[buffer->front];
  return DS_OK;
}

status_t ring_buffer_peek_rear(const ring_buffer_t *buffer, void **out_item) {
  if (buffer == NULL || out_item == NULL)
    return DS_ERR_INVALID_ARG;

  if (buffer->size == 0)
    return DS_ERR_EMPTY;

  size_t last_idx = (buffer->rear + buffer->capacity - 1) % buffer->capacity;
  *out_item = buffer->data[last_idx];
  return DS_OK;
}

bool ring_buffer_contains(const ring_buffer_t *buffer, const void *item,
                          cmp_fn cmp) {
  if (buffer == NULL || item == NULL || cmp == NULL)
    return false;

  for (size_t i = 0; i < buffer->size; i++) {
    size_t idx = (buffer->front + i) % buffer->capacity;
    if (cmp(buffer->data[idx], item) == 0)
      return true;
  }

  return false;
}
