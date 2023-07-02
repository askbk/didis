#include "common.h"

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
