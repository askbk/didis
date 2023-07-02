#ifndef DIDIS_H
#define DIDIS_H

typedef char *kv_key;
typedef char *kv_value;

typedef struct {
  kv_key *keys;
  kv_value *values;
  int size;
} KeyValueStore;

KeyValueStore *new_kv_store(void);
void delete_kv_store(KeyValueStore *kvs);
int kv_store_add(KeyValueStore *kvs, kv_key key, kv_value value);
char *kv_store_get(KeyValueStore *kvs, kv_key key);
int kv_store_key_exists(KeyValueStore *kvs, kv_key key);
int kv_store_delete(KeyValueStore *kvs, kv_key key);
int kv_store_increment(KeyValueStore *kvs, kv_key key);

#endif