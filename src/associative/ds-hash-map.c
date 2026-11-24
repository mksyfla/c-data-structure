#include "../../include/associative/ds-hash-map.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static size_t hash_function(const char *key, size_t capacity) {
  size_t hash = 0;
  while (*key != '\0')
    hash = hash * 31 + (size_t)(*key++);

  return hash % capacity;
}

typedef struct ds_map_entry {
  void *key;
  void *value;
  struct ds_map_entry *next;
} map_entry;

struct ds_hash_map {
  map_entry **buckets;
  size_t size;
  size_t capacity;
  free_fn free_fn;
  cmp_fn cmp_fn;
};

// Lifecycle
hash_map_t *map_create(size_t initial_cap, free_fn free_fn, cmp_fn cmp) {
  if (free_fn == NULL || cmp == NULL)
    return NULL;

  hash_map_t *hash_map = malloc(sizeof(hash_map_t));
  if (hash_map == NULL)
    return NULL;

  initial_cap = initial_cap < 1 ? vector_DEFAULT_CAP : initial_cap;

  hash_map->buckets = calloc(initial_cap, sizeof(map_entry *));
  if (hash_map->buckets == NULL) {
    free(hash_map);
    return NULL;
  }

  hash_map->size = 0;
  hash_map->capacity = initial_cap;
  hash_map->free_fn = free_fn;
  hash_map->cmp_fn = cmp;
  return hash_map;
}

void hash_map_destroy(hash_map_t *map) {
  if (map == NULL)
    return;

  hash_map_clear(map);
  free(map->buckets);
  free(map);
}

void hash_map_clear(hash_map_t *map) {
  if (map == NULL)
    return;

  for (size_t i = 0; i < map->capacity; i++) {
    map_entry *curr = map->buckets[i];
    while (curr != NULL) {
      map_entry *next = curr->next;
      if (map->free_fn != NULL && curr->value != NULL)
        map->free_fn(curr->value);

      free(curr);
      curr = next;
    }

    map->buckets[i] = NULL;
  }

  map->size = 0;
}

// Capacity & Info
size_t hash_map_size(const hash_map_t *map) {
  return map == NULL ? 0 : map->size;
}

size_t hash_map_capacity(const hash_map_t *map) {
  return map == NULL ? 0 : map->capacity;
}

bool hash_map_is_empty(const hash_map_t *map) {
  return map == NULL || map->size == 0;
}

// Modifiers
map_entry *map_entry_create(void *key, void *value) {
  if (key == NULL || value == NULL)
    return NULL;

  map_entry *entry = malloc(sizeof(map_entry));
  if (entry == NULL)
    return NULL;

  entry->key = key;
  entry->value = value;
  entry->next = NULL;
  return entry;
}

status_t hash_map_put(hash_map_t *map, void *key, void *value) {
  if (map == NULL || key == NULL || value == NULL)
    return DS_ERR_INVALID_ARG;

  size_t hash = hash_function(key, map->capacity);
  map_entry *curr = map->buckets[hash];

  while (curr != NULL) {
    if (map->cmp_fn(curr->key, key) == 0) {
      if (map->free_fn != NULL && curr->value != NULL && curr->value != value)
        map->free_fn(curr->value);

      curr->value = value;
      return DS_OK;
    }

    curr = curr->next;
  }

  map_entry *entry = map_entry_create(key, value);
  if (entry == NULL)
    return DS_ERR_ALLOC;

  entry->next = map->buckets[hash];
  map->buckets[hash] = entry;
  map->size++;
  return DS_OK;
}

status_t hash_map_remove(hash_map_t *map, const void *key, void **out_item) {
  if (map == NULL || key == NULL || out_item == NULL)
    return DS_ERR_INVALID_ARG;

  size_t hash = hash_function(key, map->capacity);
  map_entry *curr = map->buckets[hash];
  map_entry *prev = NULL;

  while (curr != NULL) {
    if (map->cmp_fn(curr->key, key) == 0) {
      *out_item = curr->value;
      if (prev == NULL)
        map->buckets[hash] = curr->next;
      else
        prev->next = curr->next;

      free(curr);
      map->size--;
      return DS_OK;
    }

    prev = curr;
    curr = curr->next;
  }

  return DS_ERR_NOT_FOUND;
}

// Access
status_t hash_map_get(const hash_map_t *map, const void *key, void **out_item) {
  if (map == NULL || key == NULL || out_item == NULL)
    return DS_ERR_INVALID_ARG;

  size_t hash = hash_function(key, map->capacity);
  map_entry *curr = map->buckets[hash];
  while (curr != NULL) {
    if (map->cmp_fn(curr->key, key) == 0) {
      *out_item = curr->value;
      return DS_OK;
    }

    curr = curr->next;
  }

  return DS_ERR_NOT_FOUND;
}

bool hash_map_contain_key(const hash_map_t *map, const void *key) {
  if (map == NULL || key == NULL)
    return false;

  size_t hash = hash_function(key, map->capacity);
  map_entry *curr = map->buckets[hash];
  while (curr != NULL) {
    if (map->cmp_fn(curr->key, key) == 0)
      return true;

    curr = curr->next;
  }

  return false;
}
