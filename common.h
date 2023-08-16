#ifndef COMMON_H
#define COMMON_H

#define TYPE_ERROR_MSG                                                         \
  "WRONGTYPE Operation against a key holding the wrong kind of value"

enum return_type { ERR_RETURN, STR_RETURN, INT_RETURN, OK_RETURN, NIL_RETURN };
enum datastructure { STRING, LIST, NIL };

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
int min(int a, int b);
int max(int a, int b);

#endif
