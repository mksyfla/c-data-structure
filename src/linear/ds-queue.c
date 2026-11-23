#include "../../include/linear/ds-queue.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

struct queue_node {
  void *data;
  struct queue_node *next;
};

struct ds_queue {
  queue_node_t *front;
  queue_node_t *rear;
  size_t size;
  free_fn free_fn;
};

// Lifecycle
queue_t *queue_create(free_fn free_fn) {
  queue_t *queue = malloc(sizeof(queue_t));
  if (queue == NULL)
    return NULL;

  queue->front = queue->rear = NULL;
  queue->size = 0;
  queue->free_fn = free_fn;
  return queue;
}

void queue_destroy(queue_t *queue) {
  if (queue == NULL)
    return;

  queue_clear(queue);
  free(queue);
}

void queue_clear(queue_t *queue) {
  if (queue == NULL)
    return;

  queue_node_t *curr = queue->front;
  while (curr != NULL) {
    queue_node_t *next = curr->next;
    if (queue->free_fn != NULL && curr->data != NULL)
      queue->free_fn(curr->data);

    free(curr);
    curr = next;
  }

  queue->front = queue->rear = NULL;
  queue->size = 0;
}

// Capacity & Info
size_t queue_size(const queue_t *queue) {
  return queue == NULL ? 0 : queue->size;
}

bool queue_is_empty(const queue_t *queue) {
  return queue == NULL || queue->size == 0;
}

// Modifiers
static queue_node_t *queue_node_create(void *item) {
  queue_node_t *node = malloc(sizeof(queue_node_t));
  if (node == NULL)
    return NULL;

  node->data = item;
  node->next = NULL;
  return node;
}

status_t queue_enqueue(queue_t *queue, void *item) {
  if (queue == NULL || item == NULL)
    return DS_ERR_INVALID_ARG;

  queue_node_t *node = queue_node_create(item);
  if (node == NULL)
    return DS_ERR_ALLOC;

  if (queue->front == NULL || queue->rear == NULL)
    queue->front = queue->rear = node;
  else {
    queue->rear->next = node;
    queue->rear = queue->rear->next;
  }

  queue->size++;
  return DS_OK;
}

status_t queue_dequeue(queue_t *queue, void **out_item) {
  if (queue == NULL || out_item == NULL)
    return DS_ERR_INVALID_ARG;

  if (queue->size == 0)
    return DS_ERR_EMPTY;

  queue_node_t *node = queue->front;
  *out_item = node->data;

  if (queue->front == queue->rear)
    queue->front = queue->rear = NULL;
  else
    queue->front = node->next;

  queue->size--;
  free(node);
  return DS_OK;
}

// Access
status_t queue_peek_front(const queue_t *queue, void **out_item) {
  if (queue == NULL || out_item == NULL)
    return DS_ERR_INVALID_ARG;

  if (queue->size == 0)
    return DS_ERR_EMPTY;

  *out_item = queue->front->data;
  return DS_OK;
}

status_t queue_peek_rear(const queue_t *queue, void **out_item) {
  if (queue == NULL || out_item == NULL)
    return DS_ERR_INVALID_ARG;

  if (queue->size == 0)
    return DS_ERR_EMPTY;

  *out_item = queue->rear->data;
  return DS_OK;
}

bool queue_contains(const queue_t *queue, const void *item, cmp_fn cmp) {
  if (queue == NULL || item == NULL || cmp == NULL)
    return false;

  queue_node_t *curr = queue->front;
  while (queue != NULL) {
    if (cmp(curr->data, item) == 0)
      return true;

    curr = curr->next;
  }

  return false;
}
