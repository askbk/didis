#include "common.h"
#include "keyvaluestore.h"

#ifndef LISTS_H
#define LISTS_H

typedef struct ListNode {
  struct ListNode *left;
  struct ListNode *right;
  char *value;
} ListNode;

typedef struct {
  ListNode *head;
  ListNode *tail;
  int length;
} List;

ReturnValue lists_lpush(KeyValueStore *kvs, kv_key list_name, char *element);
ReturnValue lists_lpop(KeyValueStore *kvs, char *list_name);
ReturnValue lists_rpush(KeyValueStore *kvs, char *list_name, char *element);
ReturnValue lists_rpop(KeyValueStore *kvs, char *list_name);
ReturnValue lists_length(KeyValueStore *kvs, char *list_name);
ReturnValue lists_move(KeyValueStore *kvs, char *source, char *destination,
                       int wherefrom, int whereto);
ReturnValue lists_trim(KeyValueStore *kvs, char *list_name, int start, int end);

#endif
