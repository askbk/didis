#include "common.h"
#include "keyvaluestore.h"

#ifndef STRINGS_H
#define STRINGS_H

ReturnValue strings_set(KeyValueStore *kvs, kv_key key, kv_value value);
ReturnValue strings_get(KeyValueStore *kvs, kv_key key);
ReturnValue strings_delete(KeyValueStore *kvs, kv_key key);
ReturnValue strings_increment(KeyValueStore *kvs, kv_key key);

#endif
