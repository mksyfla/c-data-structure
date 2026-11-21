#include "ds-vec.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define VEC_DEFAULT_CAP 8

struct ds_vec {
  void **data;
  size_t size;
  size_t capacity;
  free_fn free_fn;
};

// Lifecycle
vec_t *vec_create(size_t initial_cap, free_fn free_fn) {
  vec_t *vector = malloc(sizeof(vec_t));
  if (vector == NULL)
    return NULL;

  initial_cap = initial_cap < 1 ? VEC_DEFAULT_CAP : initial_cap;

  vector->data = calloc(initial_cap, sizeof(*vector->data));
  if (vector->data == NULL) {
    free(vector);
    return NULL;
  }

  vector->capacity = initial_cap;
  vector->size = 0;
  vector->free_fn = free_fn;
  return vector;
}

void vec_clear(vec_t *vec) {
  if (vec == NULL)
    return;

  if (vec->free_fn != NULL) {
    for (size_t i = 0; i < vec->size; i++) {
      if (vec->data[i] != NULL) {
        vec->free_fn(vec->data[i]);
      }
    }
  }

  vec->size = 0;
}

void vec_destroy(vec_t *vec) {
  if (vec == NULL)
    return;
  vec_clear(vec);
  free(vec->data);
  free(vec);
}

// Capacity & Info
size_t vec_size(const vec_t *vec) { return vec == NULL ? 0 : vec->size; }

size_t vec_capacity(const vec_t *vec) {
  return vec == NULL ? 0 : vec->capacity;
}

bool vec_is_empty(const vec_t *vec) { return vec == NULL || vec->size == 0; }

status_t vec_reserve(vec_t *vec, size_t new_cap) {
  if (vec == NULL)
    return DS_ERR_INVALID_ARG;

  if (new_cap <= vec->capacity)
    return DS_OK;

  void **temp = realloc(vec->data, new_cap * sizeof(*vec->data));
  if (temp == NULL)
    return DS_ERR_ALLOC;

  vec->data = temp;
  vec->capacity = new_cap;
  return DS_OK;
}

status_t vec_grow(vec_t *vec) {
  if (vec == NULL)
    return DS_ERR_INVALID_ARG;

  size_t new_cap = (vec->capacity == 0) ? VEC_DEFAULT_CAP : vec->capacity * 2;

  void **temp = realloc(vec->data, new_cap * sizeof(*vec->data));
  if (temp == NULL)
    return DS_ERR_ALLOC;

  vec->data = temp;
  vec->capacity = new_cap;
  return DS_OK;
}

status_t vec_shrink(vec_t *vec) {
  if (vec == NULL)
    return DS_ERR_INVALID_ARG;

  if (vec->capacity <= VEC_DEFAULT_CAP || vec->size > vec->capacity / 4)
    return DS_OK;

  size_t new_cap = vec->capacity / 2;
  if (new_cap < VEC_DEFAULT_CAP)
    new_cap = VEC_DEFAULT_CAP;

  void **temp = realloc(vec->data, new_cap * sizeof(*vec->data));
  if (temp == NULL)
    return DS_ERR_ALLOC;

  vec->data = temp;
  vec->capacity = new_cap;
  return DS_OK;
}

// Modifiers
status_t vec_push(vec_t *vec, void *item) {
  if (vec == NULL || item == NULL)
    return DS_ERR_INVALID_ARG;

  if (vec->size == vec->capacity) {
    status_t growing_alloc = vec_grow(vec);
    if (growing_alloc != DS_OK)
      return growing_alloc;
  }

  size_t index = vec->size;
  vec->size++;
  vec->data[index] = item;
  return DS_OK;
}

status_t vec_pop(vec_t *vec, void **out_item) {
  if (vec == NULL || out_item == NULL)
    return DS_ERR_INVALID_ARG;

  if (vec->size == 0)
    return DS_ERR_EMPTY;

  vec->size--;
  *out_item = vec->data[vec->size];

  if (vec->size == vec->capacity / 4)
    (void)vec_shrink(vec);

  return DS_OK;
}

status_t vec_insert(vec_t *vec, size_t index, void *item) {
  if (vec == NULL || item == NULL)
    return DS_ERR_INVALID_ARG;

  if (index > vec->size)
    return DS_ERR_OUT_OF_BOUNDS;

  if (vec->size == vec->capacity) {
    status_t growing_alloc = vec_grow(vec);
    if (growing_alloc != DS_OK)
      return growing_alloc;
  }

  for (size_t i = vec->size; i > index; i--)
    vec->data[i] = vec->data[i - 1];

  vec->size++;
  vec->data[index] = item;
  return DS_OK;
}

status_t vec_remove(vec_t *vec, size_t index, void **out_item) {
  if (vec == NULL || out_item == NULL)
    return DS_ERR_INVALID_ARG;

  if (vec->size == 0)
    return DS_ERR_EMPTY;

  if (index > vec->size)
    return DS_ERR_OUT_OF_BOUNDS;

  *out_item = vec->data[index];

  for (size_t i = index; i < vec->size - 1; i++)
    vec->data[i] = vec->data[i + 1];
  vec->size--;

  if (vec->size == vec->capacity / 4)
    (void)vec_shrink(vec);

  return DS_OK;
}

// Access
status_t vec_get(const vec_t *vec, size_t index, void **out_item) {
  if (vec == NULL || out_item == NULL)
    return DS_ERR_INVALID_ARG;

  if (index >= vec->size)
    return DS_ERR_OUT_OF_BOUNDS;

  *out_item = vec->data[index];
  return DS_OK;
}

status_t vec_set(vec_t *vec, size_t index, void *item) {
  if (vec == NULL || item == NULL)
    return DS_ERR_INVALID_ARG;

  if (index >= vec->size)
    return DS_ERR_OUT_OF_BOUNDS;

  if (vec->free_fn != NULL && vec->data[index] != NULL &&
      vec->data[index] != item)
    vec->free_fn(vec->data[index]);

  vec->data[index] = item;
  return DS_OK;
}
