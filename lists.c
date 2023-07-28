#include "lists.h"
#include <stdlib.h>
#include <string.h>

static void delete_list_node(ListNode *node) {
  free(node->value);
  free(node);
}

static void delete_list(List *list) {
  ListNode *n = list->head;
  while ((n = n->right) != NULL) {
    delete_list_node(n->left);
  }
  delete_list_node(list->tail);
  free(list);
}

static void lists_free(Datastructure *l) { delete_list(l->data); }

static Datastructure *make_lists_datastructure(List *l) {
  Datastructure *d = malloc(sizeof(*d));
  d->data = l;
  d->free = lists_free;
  return d;
}

static ListNode *new_list_node(ListNode *left, ListNode *right, element value) {
  ListNode *node = malloc(sizeof(*node));
  node->left = left;
  if (left)
    left->right = node;
  node->right = right;
  if (right)
    right->left = node;
  node->value = malloc(strlen(value) + 1);
  strcpy(node->value, value);
  return node;
}

static List *new_list(void) {
  List *list = malloc(sizeof(*list));
  list->head = NULL;
  list->tail = NULL;
  list->length = 0;
  return list;
}

static void list_lpush(List *list, element value) {
  ListNode *n = new_list_node(NULL, list->head, value);
  list->head = n;
  ++(list->length);
  if (list->length == 1)
    list->tail = n;
}

static void list_rpush(List *list, element value) {
  ListNode *n = new_list_node(list->tail, NULL, value);
  list->tail = n;
  ++(list->length);
  if (list->length == 1)
    list->head = n;
}

static element list_lpop(List *list) {
  ListNode *head = list->head;
  element v = malloc(strlen(list->head->value) + 1);
  strcpy(v, list->head->value);

  list->head->right->left = NULL;
  list->head = list->head->right;
  delete_list_node(head);
  --(list->length);
  return v;
}

static element list_rpop(List *list) {
  ListNode *tail = list->tail;
  element v = malloc(strlen(list->tail->value) + 1);
  strcpy(v, list->tail->value);

  list->tail->left->right = NULL;
  list->tail = list->tail->left;
  delete_list_node(tail);
  --(list->length);
  return v;
}

static int lists_add_list(KeyValueStore *kv, kv_key list_name) {
  Datastructure *d = make_lists_datastructure(new_list());
  kv_store_set_entry(kv, list_name, d);
  return kv_store_find_key_index(kv, list_name);
}

ReturnValue lists_lpush(KeyValueStore *kv, kv_key list_name, element value) {
  int index = kv_store_find_key_index(kv, list_name);
  if (index == -1)
    index = lists_add_list(kv, list_name);
  List *list = kv->datastructures[index]->data;
  list_lpush(list, value);
  return make_integer(list->length);
}

ReturnValue lists_rpush(KeyValueStore *kv, kv_key list_name, element value) {
  int index = kv_store_find_key_index(kv, list_name);
  if (index == -1)
    index = lists_add_list(kv, list_name);
  List *list = kv->datastructures[index]->data;
  list_rpush(list, value);
  return make_integer(list->length);
}

ReturnValue lists_length(KeyValueStore *kv, kv_key list_name) {
  int index = kv_store_find_key_index(kv, list_name);
  if (index == -1)
    return make_integer(0);
  List *list = kv->datastructures[index]->data;
  return make_integer(list->length);
}

ReturnValue lists_lpop(KeyValueStore *kvs, kv_key list_name) {
  Datastructure *d = kv_store_get_entry(kvs, list_name);
  List *l = d->data;
  return make_string(list_lpop(l));
}

ReturnValue lists_rpop(KeyValueStore *kvs, kv_key list_name) {
  Datastructure *d = kv_store_get_entry(kvs, list_name);
  List *l = d->data;
  return make_string(list_rpop(l));
}
