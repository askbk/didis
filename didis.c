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

static int kv_store_find_key_index(KeyValueStore *kv, kv_key key) {
  for (int i = 0; i < kv->size; ++i) {
    if (kv_key_compare(key, kv->keys[i])) return i;
  }

  return -1;
}

ReturnValue kv_store_add(KeyValueStore *kv, kv_key key, kv_value value) {
  int existing_key_index = kv_store_find_key_index(kv, key);

  if (existing_key_index >= 0) {
    kv->values[existing_key_index] =
        realloc(kv->values[existing_key_index], strlen(value) + 1);
    strcpy(kv->values[existing_key_index], value);
    ReturnValue ok = {.type = OK_RETURN};
    return ok;
  }

  ++(kv->size);
  kv->keys = realloc(kv->keys, sizeof(char *) * kv->size);
  kv->values = realloc(kv->values, sizeof(char *) * kv->size);

  int index = kv->size - 1;

  kv->keys[index] = malloc(strlen(key) + 1);
  kv->values[index] = malloc(strlen(value) + 1);

  strcpy(kv->keys[index], key);
  strcpy(kv->values[index], value);

  ReturnValue ok = {.type = OK_RETURN};
  return ok;
}

ReturnValue kv_store_get(KeyValueStore *kv, kv_key key) {
  int key_index = kv_store_find_key_index(kv, key);
  ReturnValue r;
  if (key_index == -1) {
    r.type = NIL_RETURN;
    return r;
  };
  r.type = STR_RETURN;
  r.string = kv->values[key_index];
  return r;
}

ReturnValue kv_store_key_exists(KeyValueStore *kv, kv_key key) {
  ReturnValue r = {.type = INT_RETURN,
                   .integer = kv_store_find_key_index(kv, key) >= 0};
  return r;
}

ReturnValue kv_store_delete(KeyValueStore *kv, kv_key key) {
  int key_index = kv_store_find_key_index(kv, key);
  ReturnValue r = {.type = INT_RETURN};
  if (key_index < 0) {
    r.integer = 0;
    return r;
  };

  if (key_index < kv->size - 1) {
    memmove(&kv->keys[key_index], &kv->keys[key_index + 1],
            sizeof(kv_key) * (kv->size - key_index - 1));
    memmove(&kv->values[key_index], &kv->values[key_index + 1],
            sizeof(kv_value) * (kv->size - key_index - 1));
  }

  --(kv->size);

  kv->keys = realloc(kv->keys, kv->size * sizeof(kv_key));
  kv->values = realloc(kv->values, kv->size * sizeof(kv_value));

  r.integer = 1;
  return r;
}

ReturnValue kv_store_increment(KeyValueStore *kv, kv_key key) {
  ReturnValue r;
  int index = kv_store_find_key_index(kv, key);

  if (index < 0) {
    kv_store_add(kv, key, "1");
    r.type = INT_RETURN;
    r.integer = 1;
    return r;
  }

  char *badchar;
  long incremented = strtol(kv->values[index], &badchar, 10) + 1;
  if (*badchar != '\0') {
    r.type = ERR_RETURN;
    r.error_message = "Cannot increment non-integer";
    return r;
  }
  sprintf(kv->values[index], "%ld", incremented);
  r.type = INT_RETURN;
  r.integer = incremented;
  return r;
}