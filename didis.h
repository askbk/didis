#ifndef DIDIS_H
#define DIDIS_H

typedef struct {
  char **keys;
  char **values;
  int size;
} KeyValueStore;

KeyValueStore *new_kv_store(void);
void delete_kv_store(KeyValueStore *kvs);
int kv_store_add(KeyValueStore *kvs, char *key, char *value);
char *kv_store_get(KeyValueStore *kvs, char *key);
int kv_store_key_exists(KeyValueStore *kvs, char *key);
int kv_store_delete(KeyValueStore *kvs, char *key);
int kv_store_increment(KeyValueStore *kvs, char *key);

#endif