#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strings.h"
#include "keyvaluestore.h"

ReturnValue strings_set(KeyValueStore *kv, kv_key key, kv_value value) {
  int existing_key_index = kv_store_find_key_index(kv, key);

  if (existing_key_index >= 0) {
    kv->values[existing_key_index] =
        realloc(kv->values[existing_key_index], strlen(value) + 1);
    strcpy(kv->values[existing_key_index], value);
    return make_ok();
  }

  ++(kv->size);
  kv->keys = realloc(kv->keys, sizeof(char *) * kv->size);
  kv->values = realloc(kv->values, sizeof(char *) * kv->size);

  int index = kv->size - 1;

  kv->keys[index] = malloc(strlen(key) + 1);
  kv->values[index] = malloc(strlen(value) + 1);

  strcpy(kv->keys[index], key);
  strcpy(kv->values[index], value);

  return make_ok();
}

ReturnValue strings_get(KeyValueStore *kv, kv_key key) {
  int key_index = kv_store_find_key_index(kv, key);
  if (key_index == -1) return make_nil();
  return make_string(kv->values[key_index]);
}

ReturnValue strings_delete(KeyValueStore *kv, kv_key key) {
  int key_index = kv_store_find_key_index(kv, key);
  if (key_index < 0) return make_integer(0);

  if (key_index < kv->size - 1) {
    memmove(&kv->keys[key_index], &kv->keys[key_index + 1],
            sizeof(kv_key) * (kv->size - key_index - 1));
    memmove(&kv->values[key_index], &kv->values[key_index + 1],
            sizeof(kv_value) * (kv->size - key_index - 1));
  }

  --(kv->size);

  kv->keys = realloc(kv->keys, kv->size * sizeof(kv_key));
  kv->values = realloc(kv->values, kv->size * sizeof(kv_value));

  return make_integer(1);
}

ReturnValue strings_increment(KeyValueStore *kv, kv_key key) {
  int index = kv_store_find_key_index(kv, key);

  if (index < 0) {
    strings_set(kv, key, "1");
    return make_integer(1);
  }

  char *badchar;
  long incremented = strtol(kv->values[index], &badchar, 10) + 1;
  if (*badchar != '\0') return make_error("Cannot increment non-integer");
  sprintf(kv->values[index], "%ld", incremented);
  return make_integer(incremented);
}
