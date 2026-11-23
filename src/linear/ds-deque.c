#include "../../include/linear/ds-deque.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

struct deque_node {
  void *data;
  struct deque_node *next;
  struct deque_node *back;
};
struct ds_deque {
  deque_node_t *front;
  deque_node_t *rear;
  size_t size;
  free_fn free_fn;
};

// Lifecycle
deque_t *deque_create(free_fn free_fn) {
  deque_t *deque = malloc(sizeof(deque_t));
  if (deque == NULL)
    return NULL;

  deque->front = deque->rear = NULL;
  deque->size = 0;
  deque->free_fn = free_fn;
  return deque;
}

void deque_destroy(deque_t *deque) {
  if (deque == NULL)
    return;

  deque_clear(deque);
  free(deque);
}

void deque_clear(deque_t *deque) {
  if (deque == NULL)
    return;

  deque_node_t *curr = deque->front;
  while (curr != NULL) {
    deque_node_t *next = curr->next;
    if (deque->free_fn != NULL && curr->data != NULL)
      deque->free_fn(curr->data);

    free(curr);
    curr = next;
  }

  deque->front = deque->rear = NULL;
  deque->size = 0;
}

// Capacity & Info
size_t deque_size(const deque_t *deque) {
  return deque == NULL ? 0 : deque->size;
}

bool deque_is_empty(const deque_t *deque) {
  return deque == NULL || deque->size == 0;
}

// Modifiers
static deque_node_t *deque_node_create(void *item) {
  deque_node_t *node = malloc(sizeof(deque_node_t));
  if (node == NULL)
    return NULL;

  node->data = item;
  node->next = node->back = NULL;
  return node;
}

status_t deque_push_front(deque_t *deque, void *item) {
  if (deque == NULL || item == NULL)
    return DS_ERR_INVALID_ARG;

  deque_node_t *node = deque_node_create(item);
  if (node == NULL)
    return DS_ERR_ALLOC;

  if (deque->front == NULL || deque->rear == NULL)
    deque->front = deque->rear = node;
  else {
    node->next = deque->front;
    deque->front->back = node;
    deque->front = node;
  }

  deque->size++;
  return DS_OK;
}

status_t deque_pop_front(deque_t *deque, void **out_item) {
  if (deque == NULL || out_item == NULL)
    return DS_ERR_INVALID_ARG;

  if (deque->size == 0)
    return DS_ERR_EMPTY;

  deque_node_t *node = deque->front;
  *out_item = node->data;

  if (deque->front == deque->rear)
    deque->front = deque->rear = NULL;
  else {
    deque->front = node->next;
    deque->front->back = NULL;
  }

  deque->size--;
  free(node);
  return DS_OK;
}

status_t deque_push_rear(deque_t *deque, void *item) {
  if (deque == NULL || item == NULL)
    return DS_ERR_INVALID_ARG;

  deque_node_t *node = deque_node_create(item);
  if (node == NULL)
    return DS_ERR_ALLOC;

  if (deque->front == NULL || deque->rear == NULL)
    deque->front = deque->rear = node;
  else {
    node->back = deque->rear;
    deque->rear->next = node;
    deque->rear = deque->rear->next;
  }

  deque->size++;
  return DS_OK;
}

status_t deque_pop_rear(deque_t *deque, void **out_item) {
  if (deque == NULL || out_item == NULL)
    return DS_ERR_INVALID_ARG;

  if (deque->size == 0)
    return DS_ERR_EMPTY;

  deque_node_t *node = deque->rear;
  *out_item = node->data;

  if (deque->front == deque->rear)
    deque->front = deque->rear = NULL;
  else {
    deque->rear = node->back;
    deque->rear->next = NULL;
  }

  deque->size--;
  free(node);
  return DS_OK;
}

// Access
status_t deque_peek_front(const deque_t *deque, void **out_item) {
  if (deque == NULL || out_item == NULL)
    return DS_ERR_INVALID_ARG;

  if (deque->size == 0)
    return DS_ERR_EMPTY;

  *out_item = deque->front->data;
  return DS_OK;
}

status_t deque_peek_rear(const deque_t *deque, void **out_item) {
  if (deque == NULL || out_item == NULL)
    return DS_ERR_INVALID_ARG;

  if (deque->size == 0)
    return DS_ERR_EMPTY;

  *out_item = deque->rear->data;
  return DS_OK;
}

bool deque_contains(const deque_t *deque, const void *item, cmp_fn cmp) {
  if (deque == NULL || item == NULL)
    return false;

  deque_node_t *curr = deque->front;
  while (curr != NULL) {
    if (cmp(curr->data, item) == 0)
      return true;

    curr = curr->next;
  }

  return false;
}
