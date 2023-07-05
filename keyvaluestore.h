#include "common.h"

#ifndef KEYVALUESTORE_H
#define KEYVALUESTORE_H

typedef char *kv_key;
typedef char *kv_value;

typedef struct KeyValueStore {
  kv_key *keys;
  kv_value *values;
  int size;
} KeyValueStore;

KeyValueStore *new_kv_store(void);
void delete_kv_store(KeyValueStore *kvs);
ReturnValue kv_store_key_exists(KeyValueStore *kvs, kv_key key);
int kv_store_find_key_index(KeyValueStore *kv, kv_key key);

#endif
