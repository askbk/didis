#include <stdio.h>
#include <string.h>
#include "minunit.h"
#include "didis.h"

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
  char *v = kv_store_get(kv, "hello");
  mu_assert("kv_get should return value that was inserted",
            strcmp("world", v) == 0);

  mu_assert("kv_get should return NULL when key does not exist",
            kv_store_get(kv, "blah") == NULL);
  delete_kv_store(kv);
  return 0;
}

static char *test_kv_exists() {
  KeyValueStore *kv = new_kv_store();
  kv_store_add(kv, "hello", "world");
  mu_assert("kv_store_key_exists should return 1 when key exists",
            kv_store_key_exists(kv, "hello"));
  mu_assert("kv_store_key_exists should return 0 when key does not exist",
            !kv_store_key_exists(kv, "fdsafdsa"));
  delete_kv_store(kv);
  return 0;
}

static char *test_kv_delete() {
  KeyValueStore *kv = new_kv_store();
  kv_store_add(kv, "a", "hello world");
  kv_store_add(kv, "ab", "yoyo");
  kv_store_delete(kv, "a");
  mu_assert("kv_store_delete should delete element from store",
            !kv_store_key_exists(kv, "a"));
  mu_assert("Deleting same key twice should be fine",
            kv_store_delete(kv, "a") == 0);
  delete_kv_store(kv);
  return 0;
}

static char *all_tests() {
  mu_run_test(test_kv_add);
  mu_run_test(test_kv_get);
  mu_run_test(test_kv_exists);
  mu_run_test(test_kv_delete);
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