#include "common.h"
#include "keyvaluestore.h"
#include "lists.h"
#include "minunit.h"
#include "sets.h"
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
  ReturnValue mylist = lists_range(kv, "mylist", 0, 99);
  mu_assert("list should have length 3", mylist.array_length == 3);
  mu_assert("first list element should be the first rpushed",
            strcmp(mylist.array[0], "hello world") == 0);
  mu_assert("second list element should be the second rpushed",
            strcmp(mylist.array[1], "a") == 0);
  mu_assert("this list element should be the third rpushed",
            strcmp(mylist.array[2], "b") == 0);

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

static char *test_incorrect_type_handling() {
  KeyValueStore *kv = new_kv_store();
  strings_set(kv, "string1", "hwllo");
  mu_assert("calling function on wrong datastructure should return error",
            lists_rpush(kv, "string1", "test").type == ERR_RETURN);
  mu_assert("calling sets_add on wrong datastructure returns error",
            sets_add(kv, "string1", "fdsfds").type == ERR_RETURN);
  return 0;
}

static char *test_lists_trim() {
  KeyValueStore *kv = new_kv_store();
  lists_rpush(kv, "list1", "a");
  lists_rpush(kv, "list1", "b");
  lists_rpush(kv, "list1", "c");
  // list1: a,b,c
  lists_trim(kv, "list1", 0, 99);
  mu_assert("lists_trim should not modify list when range includes whole list",
            lists_length(kv, "list1").integer == 3);
  lists_trim(kv, "list1", 1, 1);
  mu_assert("lists_trim should trim list to specified range (inclusive)",
            lists_length(kv, "list1").integer == 1);
  // list1: b
  lists_rpush(kv, "list1", "a");
  lists_rpush(kv, "list1", "b");
  // list1: b,a,b
  ReturnValue beforetrimmed = lists_range(kv, "list1", 0, 99);
  mu_assert("lists_range should return the list items in correct order",
            strcmp(beforetrimmed.array[0], "b") == 0 &&
                strcmp(beforetrimmed.array[1], "a") == 0 &&
                strcmp(beforetrimmed.array[2], "b") == 0);
  lists_trim(kv, "list1", -99, -2);
  mu_assert("lists_trim should support negative indeces",
            lists_length(kv, "list1").integer == 2);
  ReturnValue trimmed = lists_range(kv, "list1", 0, 99);
  mu_assert("lists_range returns the correct number of items",
            trimmed.array_length == 2);
  mu_assert("lists_range returns the correct items",
            strcmp(trimmed.array[0], "b") == 0 &&
                strcmp(trimmed.array[1], "a") == 0);
  return 0;
}

static char *test_sets_basic_commands() {
  KeyValueStore *kv = new_kv_store();
  mu_assert("empty keys should have cardinality 0",
            sets_cardinality(kv, "f12").integer == 0);
  mu_assert("sets_cardinality return type on empty sets should be integer",
            sets_cardinality(kv, "rdsardsa").type == INT_RETURN);
  ReturnValue r = sets_add(kv, "set", "hello");
  mu_assert(
      "sets_add should return the number of new elements added to the set",
      r.integer == 1);
  mu_assert("set cardinality should increase after adding member",
            sets_cardinality(kv, "set").integer == 1);
  mu_assert("the same value is not added twice",
            sets_add(kv, "set", "hello").integer == 0);
  mu_assert("set size stays the same when the same element is added twice",
            sets_cardinality(kv, "set").integer == 1);
  mu_assert("sets_remove should return 0 when called with non-existent key",
            sets_remove(kv, "fdsafdsafff", "fdsafd").integer == 0);
  mu_assert("sets_remove should return 0 when called with member which does "
            "not belong to the set",
            sets_remove(kv, "set", "fdsfdsfdsfds").integer == 0);
  mu_assert(
      "sets_remove should return the number of elements removed from the set",
      sets_remove(kv, "set", "hello").integer == 1);
  mu_assert("sets_ismember should return 0 if the set does not exist",
            sets_ismember(kv, "wwww", "test").integer == 0);
  sets_add(kv, "set2", "value1");
  sets_add(kv, "set2", "value2");
  sets_add(kv, "set2", "value3");
  int card_before_remove = sets_cardinality(kv, "set2").integer;
  mu_assert(
      "sets_ismember should return 0 if the value is not a member of the set",
      sets_ismember(kv, "set2", "blahblah").integer == 0);
  mu_assert("sets_ismember should return 1 if the value is a member of the set",
            sets_ismember(kv, "set2", "value1").integer == 1);
  sets_remove(kv, "set2", "value1");
  mu_assert("Removing an element should decrease the cardinality of the set",
            sets_cardinality(kv, "set2").integer == card_before_remove - 1);
  mu_assert("sets_ismember should_return 0 for values that have been removed "
            "from the set",
            sets_ismember(kv, "set2", "value1").integer == 0);

  return 0;
}

static char *test_sets_complex_commands() {
  // KeyValueStore *kv = new_kv_store();
  // test with two empty sets -> empty
  // mu_assert("intersection of two empty sets returns no elements",
  // sets_intersection(kv, "set1", "set2"). test with one empty set -> empty
  // test intersection of 1,2,3, and 4,5,6 -> should be empty
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
  mu_run_test(test_lists_trim);

  mu_run_test(test_sets_basic_commands);
  mu_run_test(test_sets_complex_commands);

  mu_run_test(test_incorrect_type_handling);
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
