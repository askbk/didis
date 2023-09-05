#include "sets.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void sets_free(Datastructure *set_datastructure) {
  struct Set *s = set_datastructure->data;
  free(s->elements);
  free(s);
}

static Datastructure *make_set_datastructure() {
  Datastructure *d = malloc(sizeof(*d));
  struct Set *s = malloc(sizeof(*s));
  s->count = 0;
  s->elements = NULL;
  d->type = SET;
  d->data = s;
  d->free = sets_free;
  return d;
}

ReturnValue sets_add(KeyValueStore *kv, kv_key key, char *value) {
  Datastructure *d = kv_store_get_entry(kv, key);
  if (d == NULL) {
    d = make_set_datastructure();
    kv_store_set_entry(kv, key, d);
  }

  struct Set *s = d->data;

  for (int i = 0; i < s->count; ++i) {
    if (strcmp(s->elements[i], value) == 0) {
      return make_integer(0);
    }
  }

  char **new_elements = malloc(sizeof(char *) * s->count + 1);
  memcpy(new_elements, s->elements, sizeof(char *) * s->count);
  new_elements[s->count] = malloc(strlen(value) + 1);
  strcpy(new_elements[s->count], value);

  free(s->elements);
  s->elements = new_elements;
  ++s->count;
  return make_integer(1);
}

ReturnValue sets_cardinality(KeyValueStore *kv, kv_key key) {
  Datastructure *d = kv_store_get_entry(kv, key);
  if (d == NULL)
    return make_integer(0);
  struct Set *s = d->data;
  return make_integer(s->count);
}
