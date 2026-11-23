#ifndef DS_STACK_H
#define DS_STACK_H

#include "../core/ds-types.h"
#include <stddef.h>
#include <stdbool.h>

typedef struct stack_node stack_node_t;
typedef struct ds_stack stack_t;

// Lifecycle
stack_t *stack_create(free_fn free_fn, cmp_fn cmp);
void stack_destroy(stack_t *stack);
void stack_clear(stack_t *stack);

// Capacity & Info
size_t stack_size(const stack_t *stack);
bool stack_is_empty(const stack_t *stack);

// Modifiers
status_t stack_push(stack_t *stack, void *item);
status_t stack_pop(stack_t *stack, void **out_item);

// Access
status_t stack_peek(const stack_t *stack, void **out_item);
status_t stack_peek_n(const stack_t *stack, size_t offset, void **out_item);
bool stack_contains(const stack_t *stack, const void *item);

#endif
