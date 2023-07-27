#include "lists.h"
#include <stdio.h>
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

static ListNode *new_list_node(ListNode *left, ListNode *right, char *value) {
  ListNode *node = malloc(sizeof(*node));
  node->left = left;
  node->right = right;
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

static void list_lpush(List *list, char *value) {
  ListNode *n = new_list_node(NULL, list->head, value);
  list->head = n;
  ++(list->length);
  if (list->length == 1)
    list->tail = n;
}

static int lists_add_list(KeyValueStore *kv, char *list_name) {
  Datastructure *d = make_lists_datastructure(new_list());
  kv_store_set_entry(kv, list_name, d);
  return kv_store_find_key_index(kv, list_name);
}

ReturnValue lists_lpush(KeyValueStore *kv, char *list_name, char *value) {
  int index = kv_store_find_key_index(kv, list_name);
  if (index == -1)
    index = lists_add_list(kv, list_name);
  List *list = kv->datastructures[index]->data;
  list_lpush(list, value);
  return make_integer(list->length);
}

ReturnValue lists_length(KeyValueStore *kv, char *list_name) {
  int index = kv_store_find_key_index(kv, list_name);
  if (index == -1)
    return make_integer(0);
  List *list = kv->datastructures[index]->data;
  return make_integer(list->length);
}
