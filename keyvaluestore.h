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
ReturnValue kv_store_add(KeyValueStore *kvs, kv_key key, kv_value value);
ReturnValue kv_store_get(KeyValueStore *kvs, kv_key key);
ReturnValue kv_store_key_exists(KeyValueStore *kvs, kv_key key);
ReturnValue kv_store_delete(KeyValueStore *kvs, kv_key key);
ReturnValue kv_store_increment(KeyValueStore *kvs, kv_key key);

#endif