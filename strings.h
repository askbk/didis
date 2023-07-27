#include "common.h"
#include "keyvaluestore.h"

#ifndef STRINGS_H
#define STRINGS_H

ReturnValue strings_set(KeyValueStore *kvs, kv_key key, char *value);
ReturnValue strings_get(KeyValueStore *kvs, kv_key key);
ReturnValue strings_increment(KeyValueStore *kvs, kv_key key);

#endif
