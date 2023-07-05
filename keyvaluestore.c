#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "keyvaluestore.h"

KeyValueStore *new_kv_store(void) {
  KeyValueStore *kv = malloc(sizeof(KeyValueStore));
  kv->keys = NULL;
  kv->values = NULL;
  kv->size = 0;
  return kv;
}

void delete_kv_store(KeyValueStore *kv) {
  for (int i = 0; i < kv->size; ++i) {
    free(kv->keys[i]);
    free(kv->values[i]);
  }

  free(kv->keys);
  free(kv->values);
  free(kv);
}

void print_kv_contents(KeyValueStore *kv) {
  for (int i = 0; i < kv->size; ++i) {
    printf("%s: %s\n", kv->keys[i], kv->values[i]);
  }
}

static int kv_key_compare(kv_key k1, kv_key k2) { return strcmp(k1, k2) == 0; }

int kv_store_find_key_index(KeyValueStore *kv, kv_key key) {
  for (int i = 0; i < kv->size; ++i) {
    if (kv_key_compare(key, kv->keys[i])) return i;
  }

  return -1;
}

ReturnValue kv_store_key_exists(KeyValueStore *kv, kv_key key) {
  return make_integer(kv_store_find_key_index(kv, key) >= 0);
}
