#include "strings.h"
#include "common.h"
#include "keyvaluestore.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void strings_free(struct Datastructure *string_datastructure) {
  free(string_datastructure->data);
}

static struct Datastructure *make_string_datastructure(element string) {
  struct Datastructure *v = malloc(sizeof(*v));
  v->data = malloc(strlen(string) + 1);
  strcpy(v->data, string);
  v->type = STRING;
  v->free = strings_free;
  return v;
}

struct ReturnValue *strings_set(struct KeyValueStore *kv, kv_key key,
                                element value) {
  kv_store_set_entry(kv, key, make_string_datastructure(value));
  return make_ok();
}

struct ReturnValue *strings_get(struct KeyValueStore *kv, kv_key key) {
  struct Datastructure *d = kv_store_get_entry(kv, key);
  TYPECHECK_DATASTRUCTURE_RETURN_NIL_IF_NULL(d, STRING);
  return make_string(d->data);
}

struct ReturnValue *strings_increment(struct KeyValueStore *kv, kv_key key) {
  struct Datastructure *d = kv_store_get_entry(kv, key);
  if (d == NULL) {
    strings_set(kv, key, "1");
    return make_integer(1);
  }
  if (d->type != STRING)
    return make_error(TYPE_ERROR_MSG);
  char *badchar;
  long incremented = strtol(d->data, &badchar, 10) + 1;
  if (*badchar != '\0')
    return make_error("Cannot increment non-integer");
  sprintf(d->data, "%ld", incremented);
  return make_integer(incremented);
}
