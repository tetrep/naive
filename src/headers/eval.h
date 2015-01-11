#ifndef EVAL_HEADER
#define EVAL_HEADER

#include "types.h"

struct lextoken eval_expr(struct expression expr);
struct lextoken eval_print_expr(struct expression expr);
void print_expr(struct expression expr);
void print_tok(struct lextoken tok);

#endif
