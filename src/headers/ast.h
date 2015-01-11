#ifndef AST_HEADER
#define AST_HEADER

#include "types.h"

#define EXPR_DEPTH_LIMIT 1024

struct expression_stack_node _expr_stack_node_list[EXPR_DEPTH_LIMIT];
struct expression_stack _expr_stack;

void push_expr(struct expression expr);
struct expression pop_expr();

#endif
