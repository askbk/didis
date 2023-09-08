#include "keyvaluestore.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

KeyValueStore *new_kv_store(void) {
  KeyValueStore *kv = malloc(sizeof(KeyValueStore));
  kv->keys = NULL;
  kv->datastructures = NULL;
  kv->size = 0;
  return kv;
}

void delete_kv_store(KeyValueStore *kv) {
  for (int i = 0; i < kv->size; ++i) {
    free(kv->keys[i]);
    kv->datastructures[i]->free(kv->datastructures[i]);
  }

  free(kv->keys);
  free(kv->datastructures);
  free(kv);
}

void print_kv_entry(KeyValueStore *kvs, int key) {
  switch (kvs->datastructures[key]->type) {
  case STRING:
    printf("%s: %s\n", kvs->keys[key], (char *)kvs->datastructures[key]->data);
    break;
  default:
    printf("%s: <datastructure>", kvs->keys[key]);
    break;
  }
}

void print_kv_contents(KeyValueStore *kv) {
  for (int i = 0; i < kv->size; ++i) {
    print_kv_entry(kv, i);
  }
}

static int kv_store_find_key_index(KeyValueStore *kv, kv_key key) {
  for (int i = 0; i < kv->size; ++i) {
    if (values_are_equal(key, kv->keys[i]))
      return i;
  }

  return -1;
}

ReturnValue kv_store_key_exists(KeyValueStore *kv, kv_key key) {
  return make_integer(kv_store_find_key_index(kv, key) >= 0);
}

// Returns the kv store datastructure or NULL if key does not exist
Datastructure *kv_store_get_entry(KeyValueStore *kvs, kv_key key) {
  int index = kv_store_find_key_index(kvs, key);
  if (index == -1)
    return NULL;
  return kvs->datastructures[index];
}

void kv_store_set_entry(KeyValueStore *kv, kv_key key, Datastructure *value) {
  int existing_key_index = kv_store_find_key_index(kv, key);

  if (existing_key_index >= 0) {
    kv->datastructures[existing_key_index]->free(
        kv->datastructures[existing_key_index]);
    kv->datastructures[existing_key_index] = value;
    return;
  }

  ++(kv->size);
  kv->keys = realloc(kv->keys, sizeof(kv_key) * kv->size);
  kv->datastructures =
      realloc(kv->datastructures, sizeof(Datastructure *) * kv->size);

  int index = kv->size - 1;

  kv->keys[index] = malloc(strlen(key) + 1);
  kv->datastructures[index] = malloc(sizeof(Datastructure *));

  strcpy(kv->keys[index], key);
  kv->datastructures[index] = value;
}

// Deletes an entry from the KeyValueStore. Returns 0 if the entry does not
// exist, otherwise returns 1.
ReturnValue kv_store_delete_entry(KeyValueStore *kv, kv_key key) {
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

  kv->keys = realloc(kv->keys, kv->size * sizeof(kv_key));
  kv->datastructures =
      realloc(kv->datastructures, kv->size * sizeof(Datastructure));

  return make_integer(1);
}
