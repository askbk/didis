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

typedef struct {
  char **keys;
  List **lists;
  int count;
} ListStore;

ListStore *new_list_store(void);
ReturnValue lists_lpush(KeyValueStore *kvs, kv_key list_name, char *element);
ReturnValue list_store_lpop(KeyValueStore *kvs, char *list_name);
ReturnValue list_store_rpush(KeyValueStore *kvs, char *list_name,
                             char *element);
ReturnValue list_store_rpop(KeyValueStore *kvs, char *list_name);
ReturnValue lists_length(KeyValueStore *kvs, char *list_name);
ReturnValue list_store_move(KeyValueStore *kvs, char *source, char *destination,
                            int wherefrom, int whereto);
ReturnValue list_store_trim(KeyValueStore *kvs, char *list_name, int start,
                            int end);

#endif
