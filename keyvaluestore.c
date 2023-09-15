#include "keyvaluestore.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

struct KeyValueStore *new_kv_store(void) {
  struct KeyValueStore *kv = malloc(sizeof(struct KeyValueStore));
  kv->keys = NULL;
  kv->datastructures = NULL;
  kv->size = 0;
  return kv;
}

void delete_kv_store(struct KeyValueStore *kv) {
  for (int i = 0; i < kv->size; ++i) {
    free(kv->keys[i]);
    kv->datastructures[i]->free(kv->datastructures[i]);
  }

  free(kv->keys);
  free(kv->datastructures);
  free(kv);
}

void print_kv_entry(struct KeyValueStore *kvs, int key) {
  switch (kvs->datastructures[key]->type) {
  case STRING:
    printf("%s: %s\n", kvs->keys[key], (char *)kvs->datastructures[key]->data);
    break;
  default:
    printf("%s: <datastructure>", kvs->keys[key]);
    break;
  }
}

void print_kv_contents(struct KeyValueStore *kv) {
  for (int i = 0; i < kv->size; ++i) {
    print_kv_entry(kv, i);
  }
}

static int kv_store_find_key_index(struct KeyValueStore *kv, kv_key key) {
  for (int i = 0; i < kv->size; ++i) {
    if (values_are_equal(key, kv->keys[i]))
      return i;
  }

  return -1;
}

struct ReturnValue kv_store_key_exists(struct KeyValueStore *kv, kv_key key) {
  return make_integer(kv_store_find_key_index(kv, key) >= 0);
}

// Returns the kv store datastructure or NULL if key does not exist
struct Datastructure *kv_store_get_entry(struct KeyValueStore *kvs,
                                         kv_key key) {
  int index = kv_store_find_key_index(kvs, key);
  if (index == -1)
    return NULL;
  return kvs->datastructures[index];
}

void kv_store_set_entry(struct KeyValueStore *kv, kv_key key,
                        struct Datastructure *value) {
  int existing_key_index = kv_store_find_key_index(kv, key);

  if (existing_key_index >= 0) {
    kv->datastructures[existing_key_index]->free(
        kv->datastructures[existing_key_index]);
    kv->datastructures[existing_key_index] = value;
    return;
  }

  ++(kv->size);
  kv->keys = reallocarray(kv->keys, sizeof(kv_key), kv->size);
  kv->datastructures = reallocarray(kv->datastructures,
                                    sizeof(struct Datastructure *), kv->size);

  int index = kv->size - 1;

  kv->keys[index] = malloc(strlen(key) + 1);
  kv->datastructures[index] = malloc(sizeof(struct Datastructure *));

  strcpy(kv->keys[index], key);
  kv->datastructures[index] = value;
}

// Deletes an entry from the KeyValueStore. Returns 0 if the entry does not
// exist, otherwise returns 1.
struct ReturnValue kv_store_delete_entry(struct KeyValueStore *kv, kv_key key) {
  int key_index = kv_store_find_key_index(kv, key);
  if (key_index < 0)
    return make_integer(0);

  kv->datastructures[key_index]->free(kv->datastructures[key_index]);
  free(kv->keys[key_index]);

  if (key_index < kv->size - 1) {
    memmove(&kv->keys[key_index], &kv->keys[key_index + 1],
            sizeof(kv_key) * (kv->size - key_index - 1));
    memmove(&kv->datastructures[key_index], &kv->datastructures[key_index + 1],
            sizeof(char *) * (kv->size - key_index - 1));
  }

  --(kv->size);

  kv->keys = reallocarray(kv->keys, sizeof(kv_key), kv->size);
  kv->datastructures =
      reallocarray(kv->datastructures, sizeof(struct Datastructure), kv->size);

  return make_integer(1);
}
