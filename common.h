#ifndef COMMON_H
#define COMMON_H

typedef char *kv_key;
typedef char *kv_value;

enum return_type { ERR_RETURN, STR_RETURN, INT_RETURN, OK_RETURN, NIL_RETURN };

typedef struct ReturnValue {
  enum return_type type;

  char *string;
  long integer;
  char *error_message;
} ReturnValue;

ReturnValue make_string(char *string);
ReturnValue make_integer(long integer);
ReturnValue make_nil();
ReturnValue make_ok();
ReturnValue make_error(char *error);

#endif