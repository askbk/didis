#ifndef COMMON_H
#define COMMON_H

#define TYPE_ERROR_MSG                                                         \
  "WRONGTYPE Operation against a key holding the wrong kind of value"

#define TYPECHECK_DATASTRUCTURE(datastructure, type_label)                     \
  do {                                                                         \
    if ((datastructure) == NULL)                                               \
      return make_nil();                                                       \
    if ((datastructure)->type != (type_label))                                 \
      return make_error(TYPE_ERROR_MSG);                                       \
  } while (0)

#define TYPECHECK_DATASTRUCTURE_RETURN_ZERO_IF_NULL(datastructure, type_label) \
  do {                                                                         \
    if ((datastructure) == NULL) {                                             \
      return make_integer(0);                                                  \
    }                                                                          \
    if ((datastructure)->type != (type_label))                                 \
      return make_error(TYPE_ERROR_MSG);                                       \
  } while (0)

enum return_type {
  ERR_RETURN,
  STR_RETURN,
  INT_RETURN,
  OK_RETURN,
  NIL_RETURN,
  ARRAY_RETURN
};
enum datastructure { STRING, LIST, NIL, SET };

typedef struct ReturnValue {
  enum return_type type;

  char *string;
  long integer;
  char *error_message;
  char **array;
} ReturnValue;

ReturnValue make_string(char *string);
ReturnValue make_integer(long integer);
ReturnValue make_nil();
ReturnValue make_ok();
ReturnValue make_error(char *error);
ReturnValue make_array(char **array);

int min(int a, int b);
int max(int a, int b);

#endif
