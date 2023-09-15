#ifndef COMMON_H
#define COMMON_H

#define TYPE_ERROR_MSG                                                         \
  "WRONGTYPE Operation against a key holding the wrong kind of value"

#define TYPECHECK_DATASTRUCTURE(datastructure, type_label)                     \
  do {                                                                         \
    if ((datastructure) != NULL && (datastructure)->type != (type_label)) {    \
      return make_error(TYPE_ERROR_MSG);                                       \
    }                                                                          \
  } while (0)

#define TYPECHECK_DATASTRUCTURE_RETURN_NIL_IF_NULL(datastructure, type_label)  \
  do {                                                                         \
    if ((datastructure) == NULL) {                                             \
      return make_nil();                                                       \
    }                                                                          \
    TYPECHECK_DATASTRUCTURE((datastructure), (type_label));                    \
  } while (0)

#define TYPECHECK_DATASTRUCTURE_RETURN_ZERO_IF_NULL(datastructure, type_label) \
  do {                                                                         \
    if ((datastructure) == NULL) {                                             \
      return make_integer(0);                                                  \
    }                                                                          \
    TYPECHECK_DATASTRUCTURE((datastructure), (type_label));                    \
  } while (0)
#define TYPECHECK_DATASTRUCTURE_RETURN_EMPTY_ARRAY_IF_NULL(datastructure,      \
                                                           type_label)         \
  do {                                                                         \
    if ((datastructure) == NULL) {                                             \
      return make_array(NULL, 0);                                              \
    }                                                                          \
    TYPECHECK_DATASTRUCTURE((datastructure), (type_label));                    \
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

struct ReturnValue {
  enum return_type type;

  char *string;
  long integer;
  char *error_message;
  char **array;
  int array_length;
};

struct ReturnValue make_string(char *string);
struct ReturnValue make_integer(long integer);
struct ReturnValue make_nil();
struct ReturnValue make_ok();
struct ReturnValue make_error(char *error);
struct ReturnValue make_array(char **array, int array_length);

int min(int a, int b);
int max(int a, int b);

int values_are_equal(char *a, char *b);

#endif
