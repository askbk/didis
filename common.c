#include "common.h"
#include <stdlib.h>
#include <string.h>

void free_returnvalue(struct ReturnValue *r) {
  if (r->type == ARRAY_RETURN) {
    for (int i = 0; i < r->array_length; ++i) {
      free(r->array[i]);
    }
    free(r->array);
  }
  if (r->type == ERR_RETURN) {
    free(r->error_message);
  }
  if (r->type == STR_RETURN) {
    free(r->string);
  }
  free(r);
}

struct ReturnValue *make_string(char *string) {
  struct ReturnValue *r = malloc(sizeof *r);
  r->type = STR_RETURN;
  r->string = malloc((sizeof *r->string) * (strlen(string) + 1));
  strcpy(r->string, string);
  return r;
}

struct ReturnValue *make_integer(long integer) {
  struct ReturnValue *r = malloc(sizeof *r);
  r->type = INT_RETURN;
  r->integer = integer;
  return r;
}

struct ReturnValue *make_nil() {
  struct ReturnValue *r = malloc(sizeof *r);
  r->type = NIL_RETURN;
  return r;
}

struct ReturnValue *make_ok() {
  struct ReturnValue *r = malloc(sizeof *r);
  r->type = OK_RETURN;
  return r;
}

struct ReturnValue *make_error(char *error) {
  struct ReturnValue *r = malloc(sizeof *r);
  r->type = ERR_RETURN;
  r->error_message = malloc((sizeof *r->error_message) * (strlen(error) + 1));
  strcpy(r->error_message, error);
  return r;
}

struct ReturnValue *make_array(char **array, int array_length) {
  struct ReturnValue *r = malloc(sizeof *r);
  r->type = ARRAY_RETURN;
  r->array = malloc((sizeof *r->array) * array_length);
  memcpy(r->array, array, array_length);
  r->array_length = array_length;
  for (int i = 0; i < array_length; ++i) {
    r->array[i] = malloc((sizeof *array) * (strlen(array[i]) + 1));
    strcpy(r->array[i], array[i]);
  }
  return r;
};

int min(int a, int b) { return a < b ? a : b; }
int max(int a, int b) { return a > b ? a : b; }

int values_are_equal(char *a, char *b) { return strcmp(a, b) == 0; }
