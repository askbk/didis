#include "common.h"
#include <string.h>

struct ReturnValue make_string(char *string) {
  struct ReturnValue r = {.type = STR_RETURN, .string = string};
  return r;
}

struct ReturnValue make_integer(long integer) {
  struct ReturnValue r = {.type = INT_RETURN, .integer = integer};
  return r;
}

struct ReturnValue make_nil() {
  struct ReturnValue r = {.type = NIL_RETURN};
  return r;
}

struct ReturnValue make_ok() {
  struct ReturnValue r = {.type = OK_RETURN};
  return r;
}

struct ReturnValue make_error(char *error) {
  struct ReturnValue r = {.type = ERR_RETURN, .error_message = error};
  return r;
}

struct ReturnValue make_array(char **array, int array_length) {
  struct ReturnValue r = {
      .type = ARRAY_RETURN, .array = array, .array_length = array_length};
  return r;
}

int min(int a, int b) { return a < b ? a : b; }
int max(int a, int b) { return a > b ? a : b; }

int values_are_equal(char *a, char *b) { return strcmp(a, b) == 0; }
