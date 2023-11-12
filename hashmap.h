#ifndef HASHMAP_H__
#define HASHMAP_H__
#include <stdlib.h>
#include <string.h>

typedef struct pair_key_values {
  void *key;
  void *values;
} pair_key_value;

typedef struct hashmap {
  pair_key_value **key_values;
  size_t count;
  size_t capacity;
  int (*hash)(void *data);
  int (*keys_equals)(void *key1, void *key2);
} hashmap;

/* check if the hashmap is empty */
inline int hashmap_is_empty(hashmap *hm) { return hm->count == 0; }

/*
 * insert the pair (key value) into an hashmap
 * returns -1 if the hashmap has no room left
 * */
inline int hashmap_insert(hashmap *hm, void *key, void *value) {
  if (hm->count >= hm->capacity)
    return -1;

  hm->count += 1;
  int index = hm->hash(key) % hm->capacity;
  for (size_t i = 0; i < hm->capacity; ++i) {
    if (hm->key_values[index + i] == NULL) {
      pair_key_value *p = (pair_key_value *)malloc(sizeof(pair_key_value));
      p->key = key;
      p->values = value;
      hm->key_values[index + i] = p;
      break;
    }
  }
  return 0;
}

/*
 * get the value from a key
 * return NULL if not found
 * */
inline void *hashmap_search(hashmap *hm, void *key) {
  int index = hm->hash(key) % hm->capacity;

  for (size_t i = 0; i < hm->capacity; ++i) {
    if (hm->keys_equals(hm->key_values[index + i]->key, key)) {
      return hm->key_values[index + i]->values;
    }
  }
  return NULL;
}

/*
 * remove an element (found by the key) from the hashmap
 * returns < 0 if failed
 * */
inline int hashmap_remove(hashmap *hm, void *key) {
  int index = hm->hash(key) % hm->capacity;

  for (size_t i = 0; i < hm->capacity; ++i) {
    if (hm->keys_equals(hm->key_values[index + i]->key, key)) {
      hm->key_values[index + i] = 0;
      free(hm->key_values[index + i]);
      return 0;
    }
  }

  return -1;
}

/* initialize the hashmap with default values and allocate it */
inline hashmap *hashmap_init(size_t capacity, int (*hash)(void *key),
                             int (*keys_equals)(void *key1, void *key2)) {
  hashmap *res = (hashmap *)malloc(sizeof(hashmap));
  res->key_values = (pair_key_value **)calloc(capacity, sizeof(pair_key_value));
  res->count = 0;
  res->capacity = capacity;
  res->hash = hash;
  res->keys_equals = keys_equals;
  return res;
}

/* dealloc the hashmap and its elements */
inline void hashmap_delete(hashmap *hm) {
  for (size_t i = 0; i < hm->capacity; ++i) {
    if (hm->key_values[i] == 0)
      continue;
    free(hm->key_values[i]);
  }
  free(hm->key_values);
  free(hm);
}
#endif // HASHMAP_H__
