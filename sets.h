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

#endif
