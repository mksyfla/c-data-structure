#include "../../include/linear/ds-vector.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

struct ds_vector {
  void **data;
  size_t size;
  size_t capacity;
  free_fn free_fn;
};

// Lifecycle
vector_t *vector_create(size_t initial_cap, free_fn free_fn) {
  if (free_fn == NULL)
    return NULL;

  vector_t *vector = malloc(sizeof(vector_t));
  if (vector == NULL)
    return NULL;

  initial_cap = initial_cap < 1 ? vector_DEFAULT_CAP : initial_cap;

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

void vector_clear(vector_t *vector) {
  if (vector == NULL)
    return;

  if (vector->free_fn != NULL) {
    for (size_t i = 0; i < vector->size; i++) {
      if (vector->data[i] != NULL) {
        vector->free_fn(vector->data[i]);
      }
    }
  }

  vector->size = 0;
}

void vector_destroy(vector_t *vector) {
  if (vector == NULL)
    return;
  vector_clear(vector);
  free(vector->data);
  free(vector);
}

// Capacity & Info
size_t vector_size(const vector_t *vector) {
  return vector == NULL ? 0 : vector->size;
}

size_t vector_capacity(const vector_t *vector) {
  return vector == NULL ? 0 : vector->capacity;
}

bool vector_is_empty(const vector_t *vector) {
  return vector == NULL || vector->size == 0;
}

static status_t vector_grow(vector_t *vector) {
  if (vector == NULL)
    return DS_ERR_INVALID_ARG;

  size_t new_cap =
      (vector->capacity == 0) ? vector_DEFAULT_CAP : vector->capacity * 2;

  void **temp = realloc(vector->data, new_cap * sizeof(*vector->data));
  if (temp == NULL)
    return DS_ERR_ALLOC;

  vector->data = temp;
  vector->capacity = new_cap;
  return DS_OK;
}

static status_t vector_shrink(vector_t *vector) {
  if (vector == NULL)
    return DS_ERR_INVALID_ARG;

  size_t target_cap = (vector->size < vector_DEFAULT_CAP)
                          ? vector_DEFAULT_CAP
                          : vector->capacity / 2;
  if (target_cap == vector->capacity)
    return DS_OK;

  void **temp = realloc(vector->data, target_cap * sizeof(*vector->data));
  if (temp == NULL)
    return DS_ERR_ALLOC;

  vector->data = temp;
  vector->capacity = target_cap;
  return DS_OK;
}

// Modifiers
status_t vector_push(vector_t *vector, void *item) {
  if (vector == NULL || item == NULL)
    return DS_ERR_INVALID_ARG;

  if (vector->size == vector->capacity) {
    status_t growing_alloc = vector_grow(vector);
    if (growing_alloc != DS_OK)
      return growing_alloc;
  }

  size_t index = vector->size;
  vector->size++;
  vector->data[index] = item;
  return DS_OK;
}

status_t vector_pop(vector_t *vector, void **out_item) {
  if (vector == NULL || out_item == NULL)
    return DS_ERR_INVALID_ARG;

  if (vector->size == 0)
    return DS_ERR_EMPTY;

  vector->size--;
  *out_item = vector->data[vector->size];

  if (vector->size == vector->capacity / 4)
    (void)vector_shrink(vector);

  return DS_OK;
}

status_t vector_insert(vector_t *vector, size_t index, void *item) {
  if (vector == NULL || item == NULL)
    return DS_ERR_INVALID_ARG;

  if (index > vector->size)
    return DS_ERR_OUT_OF_BOUNDS;

  if (vector->size == vector->capacity) {
    status_t growing_alloc = vector_grow(vector);
    if (growing_alloc != DS_OK)
      return growing_alloc;
  }

  for (size_t i = vector->size; i > index; i--)
    vector->data[i] = vector->data[i - 1];

  vector->size++;
  vector->data[index] = item;
  return DS_OK;
}

status_t vector_remove(vector_t *vector, size_t index, void **out_item) {
  if (vector == NULL || out_item == NULL)
    return DS_ERR_INVALID_ARG;

  if (vector->size == 0)
    return DS_ERR_EMPTY;

  if (index >= vector->size)
    return DS_ERR_OUT_OF_BOUNDS;

  *out_item = vector->data[index];

  for (size_t i = index; i < vector->size - 1; i++)
    vector->data[i] = vector->data[i + 1];
  vector->size--;

  if (vector->size == vector->capacity / 4)
    (void)vector_shrink(vector);

  return DS_OK;
}

status_t vector_set(vector_t *vector, size_t index, void *item) {
  if (vector == NULL || item == NULL)
    return DS_ERR_INVALID_ARG;

  if (index >= vector->size)
    return DS_ERR_OUT_OF_BOUNDS;

  if (vector->free_fn != NULL && vector->data[index] != NULL &&
      vector->data[index] != item)
    vector->free_fn(vector->data[index]);

  vector->data[index] = item;
  return DS_OK;
}

// Access
status_t vector_get(const vector_t *vector, size_t index, void **out_item) {
  if (vector == NULL || out_item == NULL)
    return DS_ERR_INVALID_ARG;

  if (index >= vector->size)
    return DS_ERR_OUT_OF_BOUNDS;

  *out_item = vector->data[index];
  return DS_OK;
}

bool vector_contains(const vector_t *vector, const void *item, cmp_fn cmp) {
  if (vector == NULL || item == NULL || cmp == NULL)
    return false;

  for (size_t i = 0; i < vector->size; i++)
    if (cmp(vector->data[i], item) == 0)
      return true;

  return false;
}

size_t vector_index_of(const vector_t *vector, const void *item, cmp_fn cmp) {
  if (vector == NULL || item == NULL || cmp == NULL)
    return -1;

  for (size_t i = 0; i < vector->size; i++)
    if (cmp(vector->data[i], item) == 0)
      return i;

  return -1;
}
