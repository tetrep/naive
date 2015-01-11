#ifndef TYPES_HEADER
#define TYPES_HEADER

#include <unistd.h>

#define MAX_TOKEN_SIZE 1 << 18

typedef enum {
  lextoken_null,
  lextoken_nop,
  lextoken_operator,
  lextoken_symbol,
  lextoken_int,
  lextoken_tok_delim,
  lextoken_expr_open,
  lextoken_expr_close,
  lextoken_str_delim,
  lextoken_eoe,
} lextoken_type;

struct lextoken {
  lextoken_type type;
  char val[MAX_TOKEN_SIZE];
  size_t used;
  size_t allocated;
};

struct expression {
  struct lextoken op;
  struct lextoken lhs;
  struct lextoken rhs;
};

struct expression_stack_node {
  struct expression expr;
  struct expression_stack_node *prev;
};

struct expression_stack {
  struct expression_stack_node *back;
  unsigned int size;
};

#endif
