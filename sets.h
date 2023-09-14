#ifndef SETS_H
#define SETS_H
#include "common.h"
#include "keyvaluestore.h"

struct Set {
  int count;
  char **elements;
};

ReturnValue sets_add(KeyValueStore *kv, kv_key key, char *value);
ReturnValue sets_cardinality(KeyValueStore *kv, kv_key key);
ReturnValue sets_remove(KeyValueStore *kv, kv_key key, char *value);
ReturnValue sets_ismember(KeyValueStore *kv, kv_key key, char *value);
ReturnValue sets_intersection(KeyValueStore *kv, kv_key key1, kv_key key2);
ReturnValue sets_difference(KeyValueStore *kv, kv_key key1, kv_key key2);
ReturnValue sets_union(KeyValueStore *kv, kv_key key1, kv_key key2);

#endif
