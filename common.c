#include "common.h"
#include <string.h>

ReturnValue make_string(char *string) {
  ReturnValue r = {.type = STR_RETURN, .string = string};
  return r;
}

ReturnValue make_integer(long integer) {
  ReturnValue r = {.type = INT_RETURN, .integer = integer};
  return r;
}

ReturnValue make_nil() {
  ReturnValue r = {.type = NIL_RETURN};
  return r;
}

ReturnValue make_ok() {
  ReturnValue r = {.type = OK_RETURN};
  return r;
}

ReturnValue make_error(char *error) {
  ReturnValue r = {.type = ERR_RETURN, .error_message = error};
  return r;
}

ReturnValue make_array(char **array, int array_length) {
  ReturnValue r = {
      .type = ARRAY_RETURN, .array = array, .array_length = array_length};
  return r;
}

int min(int a, int b) { return a < b ? a : b; }
int max(int a, int b) { return a > b ? a : b; }

int values_are_equal(char *a, char *b) { return strcmp(a, b) == 0; }
