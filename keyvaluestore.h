#include "common.h"

#ifndef KEYVALUESTORE_H
#define KEYVALUESTORE_H

typedef char *kv_key;

typedef struct Datastructure {
  enum datastructure type;
  void *data;
  void (*free)(struct Datastructure *);
} Datastructure;

typedef struct KeyValueStore {
  kv_key *keys;
  Datastructure **datastructures;
  int size;
} KeyValueStore;

KeyValueStore *new_kv_store(void);
void delete_kv_store(KeyValueStore *kvs);
ReturnValue kv_store_key_exists(KeyValueStore *kvs, kv_key key);
int kv_store_find_key_index(KeyValueStore *kv, kv_key key);
Datastructure *kv_store_get_entry(KeyValueStore *kvs, kv_key key);
void kv_store_set_entry(KeyValueStore *kvs, kv_key key, Datastructure *value);
int kv_store_delete_entry(KeyValueStore *kvs, kv_key key);

#endif
