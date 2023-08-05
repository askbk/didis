#include "common.h"
#include "keyvaluestore.h"

#ifndef LISTS_H
#define LISTS_H

typedef enum { LEFT, RIGHT } list_end;

typedef struct ListNode {
  struct ListNode *left;
  struct ListNode *right;
  element value;
} ListNode;

typedef struct {
  ListNode *head;
  ListNode *tail;
  int length;
} List;

void lists_print(KeyValueStore *kvs, kv_key list_name);
ReturnValue lists_lpush(KeyValueStore *kvs, kv_key list_name, element v);
ReturnValue lists_lpop(KeyValueStore *kvs, kv_key list_name);
ReturnValue lists_rpush(KeyValueStore *kvs, kv_key list_name, element v);
ReturnValue lists_rpop(KeyValueStore *kvs, kv_key list_name);
ReturnValue lists_length(KeyValueStore *kvs, kv_key list_name);
ReturnValue lists_move(KeyValueStore *kvs, kv_key source, kv_key destination,
                       list_end wherefrom, list_end whereto);
ReturnValue lists_trim(KeyValueStore *kvs, kv_key list_name, int start,
                       int end);

#endif
