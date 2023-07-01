#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "didis.h"

KeyValueStore *new_kv_store(void) {
  KeyValueStore *kv = malloc(sizeof(KeyValueStore));
  kv->keys = NULL;
  kv->values = NULL;
  kv->size = 0;
  return kv;
}

int kv_store_add(KeyValueStore *kv, char *key, char *value) {
  for (int i = 0; i < kv->size; ++i) {
    if (strcmp(kv->keys[i], key) == 0) {
      kv->values[i] = realloc(kv->values[i], strlen(value) + 1);
      strcpy(kv->values[i], value);
      return 0;
    }
  }

  ++(kv->size);
  kv->keys = realloc(kv->keys, sizeof(char *) * kv->size);
  kv->values = realloc(kv->values, sizeof(char *) * kv->size);

  int index = kv->size - 1;

  kv->keys[index] = malloc(strlen(key) + 1);
  kv->values[index] = malloc(strlen(value) + 1);

  strcpy(kv->keys[index], key);
  strcpy(kv->values[index], value);

  return 0;
}

char *kv_store_get(KeyValueStore *kv, char *key) {
  for (int i = 0; i < kv->size; ++i) {
    if (strcmp(kv->keys[i], key) == 0) {
      return kv->values[i];
    }
  }

  return NULL;
}
