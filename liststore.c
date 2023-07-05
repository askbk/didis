#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "liststore.h"

static ListNode* new_list_node(ListNode* left, ListNode* right, char* value) {
  ListNode* node = malloc(sizeof(*node));
  node->left = left;
  node->right = right;
  node->value = malloc(strlen(value) + 1);
  strcpy(node->value, value);
  return node;
}

// static void remove_list_node(ListNode* node) {
//   node->left->right = node->right;
//   node->right->left = node->left;
// }

static void delete_list_node(ListNode* node) {
  free(node->value);
  free(node);
}

static List* new_list(void) {
  List* list = malloc(sizeof(*list));
  list->head = NULL;
  list->tail = NULL;
  list->length = 0;
  return list;
}

static void delete_list(List* list) {
  ListNode* n = list->head;
  while ((n = n->right) != NULL) {
    delete_list_node(n->left);
  }
  delete_list_node(list->tail);
  free(list);
}

static void list_lpush(List* list, char* value) {
  ListNode* n = new_list_node(NULL, list->head, value);
  list->head = n;
  ++(list->length);
  if (list->length == 1) list->tail = n;
}

ListStore* new_list_store(void) {
  ListStore* ls = malloc(sizeof(*ls));
  ls->count = 0;
  ls->keys = malloc(sizeof(char*));
  ls->lists = malloc(sizeof(List));
  return ls;
}

void delete_list_store(ListStore* ls) {
  for (int i = 0; i < ls->count; ++i) {
    free(ls->keys[i]);
    delete_list(ls->lists[i]);
  }
  free(ls->keys);
  free(ls->lists);
  free(ls);
}

static int list_store_add_list(ListStore* ls, char* list_name) {
  int last = ls->count;
  ++(ls->count);
  ls->keys = realloc(ls->keys, ls->count);
  ls->lists = realloc(ls->lists, ls->count);
  ls->keys[last] = malloc(strlen(list_name) + 1);
  strcpy(ls->keys[last], list_name);
  ls->lists[last] = new_list();
  return last;
}

static int key_compare(kv_key k1, kv_key k2) { return strcmp(k1, k2) == 0; }

static int find_key_index(ListStore* ls, char* list_name) {
  for (int i = 0; i < ls->count; ++i) {
    if (key_compare(list_name, ls->keys[i])) return i;
  }

  return -1;
}

ReturnValue list_store_lpush(ListStore* ls, char* list_name, char* value) {
  int index = find_key_index(ls, list_name);
  if (index == -1) index = list_store_add_list(ls, list_name);
  List* list = ls->lists[index];
  list_lpush(list, value);
  return make_integer(list->length);
}