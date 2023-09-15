#include "lists.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Frees the node's allocated memory.
static void delete_list_node(struct ListNode *node) {
  free(node->value);
  free(node);
}

// Frees the list and all its list nodes.
static void delete_list(struct List *list) {
  struct ListNode *n = list->head;
  if (n == NULL) {
    free(list);
    return;
  }
  while ((n = n->right) != NULL) {
    delete_list_node(n->left);
  }
  delete_list_node(list->tail);
  free(list);
}

static void lists_free(struct Datastructure *l) { delete_list(l->data); }

static struct Datastructure *make_lists_datastructure(struct List *l) {
  struct Datastructure *d = malloc(sizeof(*d));
  d->data = l;
  d->free = lists_free;
  d->type = LIST;
  return d;
}

static struct ListNode *new_list_node(struct ListNode *left,
                                      struct ListNode *right, element value) {
  struct ListNode *node = malloc(sizeof(*node));
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

static struct List *new_list(void) {
  struct List *list = malloc(sizeof(*list));
  list->head = NULL;
  list->tail = NULL;
  list->length = 0;
  return list;
}

static void list_lpush(struct List *list, element value) {
  struct ListNode *n = new_list_node(NULL, list->head, value);
  list->head = n;
  ++(list->length);
  if (list->length == 1)
    list->tail = n;
}

static void list_rpush(struct List *list, element value) {
  struct ListNode *n = new_list_node(list->tail, NULL, value);
  list->tail = n;
  ++(list->length);
  if (list->length == 1)
    list->head = n;
}

static element list_pop(struct List *list, list_end wherefrom) {
  struct ListNode *end = wherefrom == LEFT ? list->head : list->tail;

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
  } else {
    list->head = NULL;
    list->tail = NULL;
  }
  delete_list_node(end);
  --(list->length);
  return v;
}

static struct Datastructure *lists_add_list(struct KeyValueStore *kv,
                                            kv_key list_name) {
  struct Datastructure *d = make_lists_datastructure(new_list());
  kv_store_set_entry(kv, list_name, d);
  return kv_store_get_entry(kv, list_name);
}

static int is_empty_list(struct List *l) { return l->length == 0; }

static struct ReturnValue lists_push(struct KeyValueStore *kv, kv_key list_name,
                                     element value, list_end whereto) {
  struct Datastructure *d = kv_store_get_entry(kv, list_name);
  if (d == NULL)
    d = lists_add_list(kv, list_name);
  else if (d->type != LIST)
    return make_error(TYPE_ERROR_MSG);
  struct List *list = d->data;
  if (whereto == LEFT)
    list_lpush(list, value);
  else
    list_rpush(list, value);
  return make_integer(list->length);
}

static void print(struct List *l) {
  printf("[");
  struct ListNode *n = l->head;
  while (n) {
    printf("%s, ", n->value);
    n = n->right;
  }
  printf("]\n");
}

void lists_print(struct KeyValueStore *kvs, kv_key list_name) {
  struct List *l = kv_store_get_entry(kvs, list_name)->data;
  print(l);
}

struct ReturnValue lists_lpush(struct KeyValueStore *kv, kv_key list_name,
                               element value) {
  return lists_push(kv, list_name, value, LEFT);
}

struct ReturnValue lists_rpush(struct KeyValueStore *kv, kv_key list_name,
                               element value) {
  return lists_push(kv, list_name, value, RIGHT);
}

struct ReturnValue lists_length(struct KeyValueStore *kv, kv_key list_name) {
  struct Datastructure *d = kv_store_get_entry(kv, list_name);
  if (d == NULL)
    return make_integer(0);
  struct List *l = d->data;
  return make_integer(l->length);
}

static struct ReturnValue lists_pop(struct KeyValueStore *kvs, kv_key list_name,
                                    list_end wherefrom) {
  struct Datastructure *d = kv_store_get_entry(kvs, list_name);
  TYPECHECK_DATASTRUCTURE_RETURN_NIL_IF_NULL(d, LIST);

  struct List *l = d->data;
  struct ReturnValue result = make_string(list_pop(l, wherefrom));
  if (is_empty_list(l)) {
    kv_store_delete_entry(kvs, list_name);
  }
  return result;
}

struct ReturnValue lists_lpop(struct KeyValueStore *kvs, kv_key list_name) {
  return lists_pop(kvs, list_name, LEFT);
}

struct ReturnValue lists_rpop(struct KeyValueStore *kvs, kv_key list_name) {
  return lists_pop(kvs, list_name, RIGHT);
}

struct ReturnValue lists_move(struct KeyValueStore *kvs, kv_key src_key,
                              kv_key dest_key, list_end wherefrom,
                              list_end whereto) {
  struct Datastructure *d = kv_store_get_entry(kvs, src_key);
  TYPECHECK_DATASTRUCTURE_RETURN_NIL_IF_NULL(d, LIST);

  struct List *src = d->data;
  element v = list_pop(src, wherefrom);
  if (is_empty_list(src)) {
    kv_store_delete_entry(kvs, src_key);
  }
  lists_push(kvs, dest_key, v, whereto);
  return make_string(v);
}

static struct ListNode *find_list_node_at_index(struct List *l, int index) {
  struct ListNode *current = l->head;
  for (int i = 0; i < index; ++i) {
    current = current->right;
  }
  return current;
}

// Deletes all nodes to the left of node in the list.
// Node must be set as the list head after calling this function.
static void delete_all_to_left(struct ListNode *node) {
  if (node->left == NULL)
    return;
  delete_all_to_left(node->left);
  delete_list_node(node->left);
  node->left = NULL;
}

static void delete_all_to_right(struct ListNode *node) {
  if (node->right == NULL)
    return;
  delete_all_to_right(node->right);
  delete_list_node(node->right);
  node->right = NULL;
}

static int negative_to_positive_index(int index, int list_length) {
  if (index >= 0)
    return index;
  return max(list_length + index, 0);
}

static int trim_should_delete(int start_index, int end_index, int list_length) {
  if (start_index >= list_length)
    return 1;
  return end_index < start_index;
}

struct ReturnValue lists_trim(struct KeyValueStore *kvs, kv_key list_name,
                              int start, int end) {
  struct Datastructure *d = kv_store_get_entry(kvs, list_name);
  TYPECHECK_DATASTRUCTURE_RETURN_NIL_IF_NULL(d, LIST);

  struct List *list = d->data;
  if (trim_should_delete(start, end, list->length)) {
    kv_store_delete_entry(kvs, list_name);
    return make_ok();
  }

  int canonical_start = negative_to_positive_index(start, list->length);
  int canonical_end =
      min(negative_to_positive_index(end, list->length), list->length - 1);

  struct ListNode *new_head = find_list_node_at_index(list, canonical_start);
  struct ListNode *new_tail = find_list_node_at_index(list, canonical_end);
  delete_all_to_left(new_head);
  delete_all_to_right(new_tail);
  list->head = new_head;
  list->tail = new_tail;
  list->length = canonical_end - canonical_start + 1;
  return make_ok();
}

struct ReturnValue lists_range(struct KeyValueStore *kvs, kv_key list_name,
                               int start, int end) {
  struct Datastructure *d = kv_store_get_entry(kvs, list_name);
  TYPECHECK_DATASTRUCTURE_RETURN_NIL_IF_NULL(d, LIST);

  struct List *list = d->data;
  int canonical_start = negative_to_positive_index(start, list->length);
  int canonical_end =
      min(negative_to_positive_index(end, list->length), list->length - 1);

  int return_length = canonical_end - canonical_start + 1;
  char **array = malloc(sizeof(*array) * return_length);

  struct ListNode *curr = find_list_node_at_index(list, canonical_start);
  for (int i = 0; i < return_length; ++i) {
    array[i] = malloc(sizeof(array[i]) * strlen(curr->value) + 1);
    strcpy(array[i], curr->value);
    curr = curr->right;
  }

  return make_array(array, return_length);
}
