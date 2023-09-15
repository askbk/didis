#include "common.h"
#include "keyvaluestore.h"

#ifndef STRINGS_H
#define STRINGS_H

struct ReturnValue strings_set(struct KeyValueStore *kvs, kv_key key,
                               char *value);
struct ReturnValue strings_get(struct KeyValueStore *kvs, kv_key key);
struct ReturnValue strings_increment(struct KeyValueStore *kvs, kv_key key);

#endif
