#include "common.h"

#ifndef KEYVALUESTORE_H
#define KEYVALUESTORE_H

typedef char *kv_key;
typedef char *element;

struct Datastructure {
  enum datastructure type;
  void *data;
  void (*free)(struct Datastructure *);
};

struct KeyValueStore {
  kv_key *keys;
  struct Datastructure **datastructures;
  int size;
};

struct KeyValueStore *new_kv_store(void);
void delete_kv_store(struct KeyValueStore *kvs);
struct ReturnValue kv_store_key_exists(struct KeyValueStore *kvs, kv_key key);
struct Datastructure *kv_store_get_entry(struct KeyValueStore *kvs, kv_key key);
void kv_store_set_entry(struct KeyValueStore *kvs, kv_key key,
                        struct Datastructure *value);
struct ReturnValue kv_store_delete_entry(struct KeyValueStore *kvs, kv_key key);

#endif
