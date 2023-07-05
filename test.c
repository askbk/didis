#include <stdio.h>
#include <string.h>
#include "minunit.h"
#include "keyvaluestore.h"
#include "liststore.h"

int tests_run = 0;

static char *test_kv_add() {
  KeyValueStore *kv = new_kv_store();
  mu_assert("error, nonempty kv store", kv->size == 0);
  kv_store_add(kv, "a", "hello world");
  kv_store_add(kv, "ab", "yoyo");
  kv_store_add(kv, "ab", "yoyo1");
  mu_assert("size != 2 after adding 2 keys", kv->size == 2);
  delete_kv_store(kv);
  return 0;
}

static char *test_kv_get() {
  KeyValueStore *kv = new_kv_store();
  kv_store_add(kv, "hello", "world");
  char *v = kv_store_get(kv, "hello").string;
  mu_assert("kv_get should return value that was inserted",
            strcmp("world", v) == 0);

  mu_assert("kv_get should return NIL when key does not exist",
            kv_store_get(kv, "blah").type == NIL_RETURN);
  delete_kv_store(kv);
  return 0;
}

static char *test_kv_exists() {
  KeyValueStore *kv = new_kv_store();
  kv_store_add(kv, "hello", "world");
  mu_assert("kv_store_key_exists should return 1 when key exists",
            kv_store_key_exists(kv, "hello").integer);
  mu_assert("kv_store_key_exists should return 0 when key does not exist",
            !kv_store_key_exists(kv, "fdsafdsa").integer);
  delete_kv_store(kv);
  return 0;
}

static char *test_kv_delete() {
  KeyValueStore *kv = new_kv_store();
  kv_store_add(kv, "a", "hello world");
  kv_store_add(kv, "ab", "yoyo");
  kv_store_delete(kv, "a");
  mu_assert("kv_store_delete should delete element from store",
            !kv_store_key_exists(kv, "a").integer);
  mu_assert("Deleting same key twice should be fine",
            kv_store_delete(kv, "a").integer == 0);
  delete_kv_store(kv);
  return 0;
}

static char *test_kv_increment() {
  KeyValueStore *kv = new_kv_store();
  kv_store_add(kv, "counter", "123");
  kv_store_increment(kv, "counter");
  mu_assert("kv_store_increment should increment integers",
            strcmp(kv_store_get(kv, "counter").string, "124") == 0);

  kv_store_increment(kv, "a");
  mu_assert("kv_store_increment should create new keys if key does not exist",
            strcmp(kv_store_get(kv, "a").string, "1") == 0);

  kv_store_add(kv, "string", "hello world");
  mu_assert("kv_store increment should return error if value is not an integer",
            kv_store_increment(kv, "string").type == ERR_RETURN);
  return 0;
}

static char *test_ls_lpush() {
  ListStore *ls = new_list_store();
  mu_assert("non-existing lists should have lengt 0",
            list_store_length(ls, "fdsfodj").integer == 0);
  mu_assert("list_store_lpush should increase list length",
            list_store_lpush(ls, "mylist", "hello world").integer == 1);
  mu_assert("list_store_length should return list length",
            list_store_length(ls, "mylist").integer == 1);
  delete_list_store(ls);
  return 0;
}

static char *all_tests() {
  mu_run_test(test_kv_add);
  mu_run_test(test_kv_get);
  mu_run_test(test_kv_exists);
  mu_run_test(test_kv_delete);
  mu_run_test(test_kv_increment);

  mu_run_test(test_ls_lpush);
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
