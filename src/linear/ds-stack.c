#include "../../include/linear/ds-stack.h"
#include <stdlib.h>

struct stack_node {
  void *data;
  struct stack_node *next;
};

struct ds_stack {
  stack_node_t *top;
  size_t size;
  free_fn free_fn;
  cmp_fn cmp_fn;
};

// Lifecycle
stack_t *stack_create(free_fn free_fn, cmp_fn cmp) {
  if (free_fn == NULL || cmp == NULL)
    return NULL;

  stack_t *stack = malloc(sizeof(stack_t));
  if (stack == NULL)
    return NULL;

  stack->top = NULL;
  stack->size = 0;
  stack->free_fn = free_fn;
  stack->cmp_fn = cmp;
  return stack;
}

void stack_destroy(stack_t *stack) {
  if (stack == NULL)
    return;

  stack_clear(stack);
  free(stack);
}

void stack_clear(stack_t *stack) {
  if (stack == NULL)
    return;

  stack_node_t *curr = stack->top;
  while (curr != NULL) {
    stack_node_t *next = curr->next;
    if (stack->free_fn != NULL && curr->data != NULL)
      stack->free_fn(curr->data);

    free(curr);
    curr = next;
  }

  stack->top = NULL;
  stack->size = 0;
}

// Capacity & Info
size_t stack_size(const stack_t *stack) {
  return stack == NULL ? 0 : stack->size;
}

bool stack_is_empty(const stack_t *stack) {
  return stack == NULL || stack->size == 0;
}

// Modifiers
stack_node_t *stack_node_create(void *item) {
  stack_node_t *node = malloc(sizeof(stack_node_t));
  if (node == NULL)
    return NULL;

  node->data = item;
  node->next = NULL;
  return node;
}

status_t stack_push(stack_t *stack, void *item) {
  if (stack == NULL || item == NULL)
    return DS_ERR_INVALID_ARG;

  stack_node_t *node = stack_node_create(item);
  if (node == NULL)
    return DS_ERR_ALLOC;

  node->next = stack->top;
  stack->top = node;
  stack->size++;
  return DS_OK;
}

status_t stack_pop(stack_t *stack, void **out_item) {
  if (stack == NULL || out_item == NULL)
    return DS_ERR_INVALID_ARG;

  if (stack->size == 0)
    return DS_ERR_EMPTY;

  stack_node_t *node = stack->top;
  *out_item = node->data;

  stack->top = node->next;
  stack->size--;
  free(node);
  return DS_OK;
}

// Access
status_t stack_peek(const stack_t *stack, void **out_item) {
  if (stack == NULL || out_item == NULL)
    return DS_ERR_INVALID_ARG;

  if (stack->size == 0)
    return DS_ERR_EMPTY;

  *out_item = stack->top->data;
  return DS_OK;
}

status_t stack_peek_n(const stack_t *stack, size_t offset, void **out_item) {
  if (stack == NULL || out_item == NULL)
    return DS_ERR_INVALID_ARG;

  if (stack->size == 0)
    return DS_ERR_EMPTY;

  if (offset >= stack->size)
    return DS_ERR_OUT_OF_BOUNDS;

  stack_node_t *curr = stack->top;
  size_t i = 0;

  while (curr != NULL && i < offset) {
    if (i == offset)
      *out_item = curr->data;

    curr = curr->next;
  }

  return DS_OK;
}

bool stack_contains(const stack_t *stack, const void *item) {
  if (stack == NULL || item == NULL)
    return false;

  stack_node_t *curr = stack->top;
  while (curr != NULL) {
    if (stack->cmp_fn(curr->data, item) == 0)
      return true;

    curr = curr->next;
  }

  return false;
}
