#include "sets.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void sets_free(struct Datastructure *set_datastructure) {
  struct Set *s = set_datastructure->data;
  free(s->elements);
  free(s);
}

static struct Datastructure *make_set_datastructure() {
  struct Datastructure *d = malloc(sizeof(*d));
  struct Set *s = malloc(sizeof(*s));
  s->count = 0;
  s->elements = NULL;
  d->type = SET;
  d->data = s;
  d->free = sets_free;
  return d;
}

struct ReturnValue *sets_add(struct KeyValueStore *kv, kv_key key,
                             char *value) {
  struct Datastructure *d = kv_store_get_entry(kv, key);
  if (d == NULL) {
    d = make_set_datastructure();
    kv_store_set_entry(kv, key, d);
  }

  TYPECHECK_DATASTRUCTURE(d, SET);
  struct Set *s = d->data;

  for (int i = 0; i < s->count; ++i) {
    if (values_are_equal(s->elements[i], value)) {
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

struct ReturnValue *sets_cardinality(struct KeyValueStore *kv, kv_key key) {
  struct Datastructure *d = kv_store_get_entry(kv, key);
  TYPECHECK_DATASTRUCTURE_RETURN_ZERO_IF_NULL(d, SET);
  struct Set *s = d->data;
  return make_integer(s->count);
}

struct ReturnValue *sets_remove(struct KeyValueStore *kv, kv_key key,
                                char *value) {
  struct Datastructure *d = kv_store_get_entry(kv, key);
  TYPECHECK_DATASTRUCTURE_RETURN_ZERO_IF_NULL(d, SET);
  struct Set *s = d->data;

  for (int i = 0; i < s->count; ++i) {
    if (values_are_equal(s->elements[i], value)) {
      free(s->elements[i]);
      if (i < s->count - 1) {
        memmove(&s->elements[i], &s->elements[i + 1],
                sizeof(char *) * (s->count - i - 1));
      }
      s->count--;
      s->elements = reallocarray(s->elements, sizeof(char *), s->count);
      if (s->count == 0) {
        kv_store_delete_entry(kv, key);
      }
      return make_integer(1);
    }
  }

  return make_integer(0);
}

static int ismember(struct Set *set, char *value) {
  for (int i = 0; i < set->count; ++i) {
    if (values_are_equal(set->elements[i], value)) {
      return 1;
    }
  }
  return 0;
}

struct ReturnValue *sets_ismember(struct KeyValueStore *kv, kv_key key,
                                  char *value) {
  struct Datastructure *d = kv_store_get_entry(kv, key);
  TYPECHECK_DATASTRUCTURE_RETURN_ZERO_IF_NULL(d, SET);
  struct Set *s = d->data;
  return make_integer(ismember(s, value));
}

struct ReturnValue *sets_intersection(struct KeyValueStore *kv, kv_key key1,
                                      kv_key key2) {
  struct Datastructure *d1 = kv_store_get_entry(kv, key1);
  TYPECHECK_DATASTRUCTURE_RETURN_EMPTY_ARRAY_IF_NULL(d1, SET);
  struct Datastructure *d2 = kv_store_get_entry(kv, key2);
  TYPECHECK_DATASTRUCTURE_RETURN_EMPTY_ARRAY_IF_NULL(d2, SET);
  struct Set *s1 = d1->data;
  struct Set *s2 = d2->data;

  struct Set *smallest = s1->count < s2->count ? s1 : s2;
  struct Set *biggest = s1->count >= s2->count ? s1 : s2;

  char **result = malloc(sizeof(*result) * smallest->count);
  int result_size = 0;
  for (int i = 0; i < smallest->count; ++i) {
    if (ismember(biggest, smallest->elements[i])) {
      result[result_size] =
          malloc(sizeof(result[result_size]) * strlen(smallest->elements[i]));
      strcpy(result[result_size], smallest->elements[i]);
      ++result_size;
    }
  }

  result = reallocarray(result, sizeof(result[0]), result_size);

  return make_array(result, result_size);
}

struct ReturnValue *sets_difference(struct KeyValueStore *kv, kv_key key1,
                                    kv_key key2) {
  struct Datastructure *d1 = kv_store_get_entry(kv, key1);
  TYPECHECK_DATASTRUCTURE_RETURN_EMPTY_ARRAY_IF_NULL(d1, SET);
  struct Datastructure *d2 = kv_store_get_entry(kv, key2);
  TYPECHECK_DATASTRUCTURE(d2, SET);

  struct Set *s1 = d1->data;
  if (d2 == NULL) {
    d2 = make_set_datastructure();
  }
  struct Set *s2 = d2->data;

  char **result = malloc(sizeof(*result) * s1->count);
  int result_size = 0;
  for (int i = 0; i < s1->count; ++i) {
    if (!ismember(s2, s1->elements[i])) {
      result[result_size] =
          malloc(sizeof(result[result_size]) * strlen(s1->elements[i]));
      strcpy(result[result_size], s1->elements[i]);
      ++result_size;
    }
  }

  result = reallocarray(result, sizeof(result[0]), result_size);
  return make_array(result, result_size);
}

struct ReturnValue *sets_union(struct KeyValueStore *kv, kv_key key1,
                               kv_key key2) {
  struct Datastructure *d1 = kv_store_get_entry(kv, key1);
  TYPECHECK_DATASTRUCTURE(d1, SET);
  struct Datastructure *d2 = kv_store_get_entry(kv, key2);
  TYPECHECK_DATASTRUCTURE(d2, SET);
  if (d1 == NULL && d2 == NULL)
    return make_array(NULL, 0);
  if (d1 == NULL)
    d1 = make_set_datastructure();
  if (d2 == NULL)
    d2 = make_set_datastructure();

  struct Set *s1 = d1->data;
  struct Set *s2 = d2->data;

  char **result = malloc(sizeof(*result) * (s1->count + s2->count));
  int result_size = s1->count;

  for (int i = 0; i < s1->count; ++i) {
    result[i] = malloc(sizeof(*result) * strlen(s1->elements[i]));
    strcpy(result[0], s1->elements[i]);
  }

  for (int i = 0; i < s2->count; ++i) {
    if (!ismember(s1, s2->elements[i])) {
      result[result_size] = malloc(sizeof(*result) * strlen(s2->elements[i]));
      strcpy(result[result_size], s2->elements[i]);
      result_size++;
    }
  }

  result = reallocarray(result, sizeof(*result), result_size);

  return make_array(result, result_size);
}
