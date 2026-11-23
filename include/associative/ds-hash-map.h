#ifndef DS_HASH_MAP_H
#define DS_HASH_MAP_H

#include "../core/ds-types.h"
#include <stddef.h>
#include <stdbool.h>

typedef struct ds_hash_map hash_map_t;

// Lifecycle
hash_map_t *map_create(size_t initial_cap, free_fn free_fn, cmp_fn cmp);
void hash_map_destroy(hash_map_t *map);
void hash_map_clear(hash_map_t *map);

// Capacity & Info
size_t hash_map_size(const hash_map_t *map);
size_t hash_map_capacity(const hash_map_t *map);
bool hash_map_is_empty(const hash_map_t *map);

// Modifiers
status_t hash_map_put(hash_map_t *map, void *key, void *value);
status_t hash_map_remove(hash_map_t *map, const void *key, void **out_item);

// Access
status_t hash_map_get(const hash_map_t *map, const void *key, void **out_item);
bool hash_map_contain_key(const hash_map_t *map, const void *key);

#endif
