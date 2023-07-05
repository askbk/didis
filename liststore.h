#include "common.h"
#ifndef LISTSTORE_H
#define LISTSTORE_H

typedef struct ListNode {
  struct ListNode* left;
  struct ListNode* right;
  char* value;
} ListNode;

typedef struct {
  ListNode* head;
  ListNode* tail;
  int length;
} List;

typedef struct {
  char** keys;
  List** lists;
  int count;
} ListStore;

ListStore* new_list_store(void);
void delete_list_store(ListStore* list_store);
ReturnValue list_store_lpush(ListStore* list_store, char* list_name,
                             char* element);
ReturnValue list_store_lpop(ListStore* list_store, char* list_name);
ReturnValue list_store_rpush(ListStore* list_store, char* list_name,
                             char* element);
ReturnValue list_store_rpop(ListStore* list_store, char* list_name);
ReturnValue list_store_length(ListStore* list_store, char* list_name);
ReturnValue list_store_move(ListStore* list_store, char* source,
                            char* destination, int wherefrom, int whereto);
ReturnValue list_store_trim(ListStore* list_store, char* list_name, int start,
                            int end);

#endif