#include "keyvaluestore.h"
#include "lists.h"
#include "minunit.h"
#include "strings.h"
#include <stdio.h>
#include <string.h>

int tests_run = 0;

static char *test_strings_set() {
  KeyValueStore *kv = new_kv_store();
  mu_assert("error, nonempty kv store", kv->size == 0);
  strings_set(kv, "a", "hello world");
  strings_set(kv, "ab", "yoyo");
  strings_set(kv, "ab", "yoyo1");
  mu_assert("size != 2 after adding 2 keys", kv->size == 2);
  delete_kv_store(kv);
  return 0;
}

static char *test_strings_get() {
  KeyValueStore *kv = new_kv_store();
  strings_set(kv, "hello", "world");
  char *v = strings_get(kv, "hello").string;
  mu_assert("kv_get should return value that was inserted",
            strcmp("world", v) == 0);

  mu_assert("kv_get should return NIL when key does not exist",
            strings_get(kv, "blah").type == NIL_RETURN);
  delete_kv_store(kv);
  return 0;
}

static char *test_kv_exists() {
  KeyValueStore *kv = new_kv_store();
  strings_set(kv, "hello", "world");
  mu_assert("kv_store_key_exists should return 1 when key exists",
            kv_store_key_exists(kv, "hello").integer);
  mu_assert("kv_store_key_exists should return 0 when key does not exist",
            !kv_store_key_exists(kv, "fdsafdsa").integer);
  delete_kv_store(kv);
  return 0;
}

static char *test_kv_delete() {
  KeyValueStore *kv = new_kv_store();
  strings_set(kv, "a", "hello world");
  strings_set(kv, "ab", "yoyo");
  kv_store_delete_entry(kv, "a");
  mu_assert("strings_delete should delete element from store",
            !kv_store_key_exists(kv, "a").integer);
  mu_assert("Deleting same key twice should be fine",
            kv_store_delete_entry(kv, "a").integer == 0);
  delete_kv_store(kv);
  return 0;
}

static char *test_strings_increment() {
  KeyValueStore *kv = new_kv_store();
  strings_set(kv, "counter", "123");
  strings_increment(kv, "counter");
  mu_assert("strings_increment should increment integers",
            strcmp(strings_get(kv, "counter").string, "124") == 0);

  strings_increment(kv, "a");
  mu_assert("strings_increment should create new keys if key does not exist",
            strcmp(strings_get(kv, "a").string, "1") == 0);

  strings_set(kv, "string", "hello world");
  mu_assert("kv_store increment should return error if value is not an integer",
            strings_increment(kv, "string").type == ERR_RETURN);
  return 0;
}

static char *test_lists_lpush_lpop() {
  KeyValueStore *kv = new_kv_store();
  mu_assert("non-existing lists should have lengt 0",
            lists_length(kv, "fdsfodj").integer == 0);
  mu_assert("lists_lpush should increase list length",
            lists_lpush(kv, "mylist", "hello world").integer == 1);
  mu_assert("list_store_length should return list length",
            lists_length(kv, "mylist").integer == 1);

  lists_lpush(kv, "mylist", "a");
  lists_lpush(kv, "mylist", "b");

  mu_assert("lists_lpop should return head of list",
            strcmp(lists_lpop(kv, "mylist").string, "b") == 0);

  mu_assert("lists_lpop should reduce list length",
            lists_length(kv, "mylist").integer == 2);
  delete_kv_store(kv);
  return 0;
}

static char *test_lists_rpush_rpop() {
  KeyValueStore *kv = new_kv_store();
  mu_assert("lists_rpush should increase list length",
            lists_rpush(kv, "mylist", "hello world").integer == 1);

  lists_rpush(kv, "mylist", "a");
  lists_rpush(kv, "mylist", "b");

  mu_assert("lists_rpop should return tail of list",
            strcmp(lists_rpop(kv, "mylist").string, "b") == 0);

  mu_assert("lists_rpop should reduce list length",
            lists_length(kv, "mylist").integer == 2);
  delete_kv_store(kv);
  return 0;
}

static char *test_lists_move() {
  KeyValueStore *kv = new_kv_store();
  // list1: a
  // list2: b, c
  // move list1 list2 left left -> list2: a, b, c. list1: empty
  lists_rpush(kv, "list1", "a");
  lists_rpush(kv, "list2", "b");
  lists_rpush(kv, "list2", "c");
  mu_assert("lists_move should return the element being popped/pushed",
            strcmp(lists_move(kv, "list1", "list2", LEFT, LEFT).string, "a") ==
                0);
  mu_assert("lists_move should remove the element from the source list",
            lists_length(kv, "list1").integer == 0);
  mu_assert("lists_move should add the element to the destination list at the "
            "specified end",
            strcmp(lists_lpop(kv, "list2").string, "a") == 0);
  return 0;
}
static char *all_tests() {
  mu_run_test(test_strings_set);
  mu_run_test(test_strings_get);
  mu_run_test(test_kv_exists);
  mu_run_test(test_kv_delete);
  mu_run_test(test_strings_increment);

  mu_run_test(test_lists_lpush_lpop);
  mu_run_test(test_lists_rpush_rpop);
  mu_run_test(test_lists_move);
  return 0;
}

int main(int argc, char **argv) {
  char *result = all_tests();
  if (result != 0) {
    printf("%s\n", result);
  } else {
    printf("ALL TESTS PASSED\n");
  }
  printf("Tests run: %d\n", tests_run);

  return result != 0;
}
