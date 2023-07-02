#ifndef DIDIS_H
#define DIDIS_H

typedef char *kv_key;
typedef char *kv_value;

enum return_type { ERR_RETURN, STR_RETURN, INT_RETURN, OK_RETURN, NIL_RETURN };

typedef struct {
  enum return_type type;

  char *string;
  long integer;
  char *error_message;
} ReturnValue;

typedef struct {
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