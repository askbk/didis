#ifndef SETS_H
#define SETS_H
#include "common.h"
#include "keyvaluestore.h"

struct Set {
  int count;
  char **elements;
};

struct ReturnValue sets_add(struct KeyValueStore *kv, kv_key key, char *value);
struct ReturnValue sets_cardinality(struct KeyValueStore *kv, kv_key key);
struct ReturnValue sets_remove(struct KeyValueStore *kv, kv_key key,
                               char *value);
struct ReturnValue sets_ismember(struct KeyValueStore *kv, kv_key key,
                                 char *value);
struct ReturnValue sets_intersection(struct KeyValueStore *kv, kv_key key1,
                                     kv_key key2);
struct ReturnValue sets_difference(struct KeyValueStore *kv, kv_key key1,
                                   kv_key key2);
struct ReturnValue sets_union(struct KeyValueStore *kv, kv_key key1,
                              kv_key key2);

#endif
