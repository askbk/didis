#include "strings.h"
#include "common.h"
#include "keyvaluestore.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void string_free(Datastructure *string_datastructure) {
  free(string_datastructure->data);
}

Datastructure *make_string_datastructure(char *string) {
  Datastructure *v = malloc(sizeof(*v));
  v->data = malloc(strlen(string) + 1);
  strcpy(v->data, string);
  v->type = STRING;
  v->free = string_free;
  return v;
}

ReturnValue strings_set(KeyValueStore *kv, kv_key key, char *value) {
  kv_store_set_entry(kv, key, make_string_datastructure(value));
  return make_ok();
  /*int existing_key_index = kv_store_find_key_index(kv, key);

  if (existing_key_index >= 0) {
    kv->datastructures[existing_key_index].data =
        realloc(kv->datastructures[existing_key_index].data, strlen(value) + 1);
    strcpy(kv->datastructures[existing_key_index].data, value);
    return make_ok();
  }

  ++(kv->size);
  kv->keys = realloc(kv->keys, sizeof(char *) * kv->size);
  kv->values = realloc(kv->datastructure, sizeof(char *) * kv->size);

  int index = kv->size - 1;

  kv->keys[index] = malloc(strlen(key) + 1);
  kv->values[index] = malloc(strlen(value) + 1);

  strcpy(kv->keys[index], key);
  strcpy(kv->values[index], value);

  return make_ok();
  */
}

ReturnValue strings_get(KeyValueStore *kv, kv_key key) {
  int key_index = kv_store_find_key_index(kv, key);
  if (key_index == -1)
    return make_nil();
  return make_string(kv->datastructures[key_index]->data);
}

ReturnValue strings_delete(KeyValueStore *kv, kv_key key) {
  return make_integer(kv_store_delete_entry(kv, key));
}

ReturnValue strings_increment(KeyValueStore *kv, kv_key key) {
  int index = kv_store_find_key_index(kv, key);

  if (index < 0) {
    strings_set(kv, key, "1");
    return make_integer(1);
  }

  char *badchar;
  long incremented = strtol(kv->datastructures[index]->data, &badchar, 10) + 1;
  if (*badchar != '\0')
    return make_error("Cannot increment non-integer");
  sprintf(kv->datastructures[index]->data, "%ld", incremented);
  return make_integer(incremented);
}
