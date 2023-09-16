#include "common.h"
#include "keyvaluestore.h"

#ifndef LISTS_H
#define LISTS_H

typedef enum { LEFT, RIGHT } list_end;

struct ListNode {
  struct ListNode *left;
  struct ListNode *right;
  element value;
};

struct List {
  struct ListNode *head;
  struct ListNode *tail;
  int length;
};

void lists_print(struct KeyValueStore *kvs, kv_key list_name);
struct ReturnValue *lists_lpush(struct KeyValueStore *kvs, kv_key list_name,
                                element v);
struct ReturnValue *lists_lpop(struct KeyValueStore *kvs, kv_key list_name);
struct ReturnValue *lists_rpush(struct KeyValueStore *kvs, kv_key list_name,
                                element v);
struct ReturnValue *lists_rpop(struct KeyValueStore *kvs, kv_key list_name);
struct ReturnValue *lists_length(struct KeyValueStore *kvs, kv_key list_name);
struct ReturnValue *lists_move(struct KeyValueStore *kvs, kv_key source,
                               kv_key destination, list_end wherefrom,
                               list_end whereto);
struct ReturnValue *lists_trim(struct KeyValueStore *kvs, kv_key list_name,
                               int start, int end);
struct ReturnValue *lists_range(struct KeyValueStore *kvs, kv_key list_name,
                                int start, int end);

#endif
