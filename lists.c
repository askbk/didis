#include "lists.h"
#include "common.h"
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
static element list_pop(List *list, list_end wherefrom) {
  ListNode *end = wherefrom == LEFT ? list->head : list->tail;

  element v = malloc(strlen(end->value) + 1);
  strcpy(v, end->value);

  if (list->length > 1) {
    if (wherefrom == LEFT) {
      end->right->left = NULL;
      list->head = list->head->right;
    } else {
      end->left->right = NULL;
      list->tail = list->tail->left;
    }
  }
  delete_list_node(end);
  --(list->length);
  return v;
}
static element list_lpop(List *list) { return list_pop(list, LEFT); }

static element list_rpop(List *list) { return list_pop(list, RIGHT); }

static Datastructure *lists_add_list(KeyValueStore *kv, kv_key list_name) {
  Datastructure *d = make_lists_datastructure(new_list());
  kv_store_set_entry(kv, list_name, d);
  return kv_store_get_entry(kv, list_name);
}

static int is_empty_list(List *l) { return l->length == 0; }

static int lists_push(KeyValueStore *kv, kv_key list_name, element value,
                      list_end whereto) {
  Datastructure *d = kv_store_get_entry(kv, list_name);
  if (d == NULL)
    d = lists_add_list(kv, list_name);
  List *list = d->data;
  if (whereto == LEFT)
    list_lpush(list, value);
  else
    list_rpush(list, value);
  return list->length;
}

ReturnValue lists_lpush(KeyValueStore *kv, kv_key list_name, element value) {
  return make_integer(lists_push(kv, list_name, value, LEFT));
}

ReturnValue lists_rpush(KeyValueStore *kv, kv_key list_name, element value) {
  return make_integer(lists_push(kv, list_name, value, RIGHT));
}

ReturnValue lists_length(KeyValueStore *kv, kv_key list_name) {
  Datastructure *d = kv_store_get_entry(kv, list_name);
  if (d == NULL)
    return make_integer(0);
  List *l = d->data;
  return make_integer(l->length);
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

ReturnValue lists_move(KeyValueStore *kvs, kv_key src_key, kv_key dest_key,
                       list_end wherefrom, list_end whereto) {
  Datastructure *d = kv_store_get_entry(kvs, src_key);
  if (d == NULL)
    return make_nil();
  List *src = d->data;
  if (is_empty_list(src))
    return make_nil();
  element v = list_pop(src, wherefrom);
  lists_push(kvs, dest_key, v, whereto);
  return make_string(v);
}
