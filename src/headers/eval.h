#ifndef EVAL_HEADER
#define EVAL_HEADER

#include "types.h"

struct lextoken eval_expr(struct expression expr);
struct lextoken eval_print_expr(struct expression expr);

void print_expr(struct expression expr);
void print_tok(struct lextoken tok);

// operator logic
void hex_str(struct expression *expr_p);
void b64_str(struct expression *expr_p);

#endif
