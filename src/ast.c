#include "headers/headers.h"

void push_expr(struct expression expr) {
  // create a new node
  struct expression_stack_node node = new_expression_stack_node(expr);
  // save the new node into the list of nodes
  node.prev = _expr_stack.back;
  _expr_stack_node_list[_expr_stack.size] = node;

  // add the node to the stack
  _expr_stack.back = &(_expr_stack_node_list[_expr_stack.size]);
  _expr_stack.size += 1;
}
struct expression pop_expr() {
  if (_expr_stack.size == 0) {
    return _empty_expression;
  }
  // grab the expr from the last node
  struct expression expr = (_expr_stack.back)->expr;
  // set the last node of the stack to the previous node
  // of the current last node
  _expr_stack.back = (_expr_stack.back)->prev;
  _expr_stack.size -= 1;

  return expr;
}

struct expression_stack_node new_expression_stack_node (struct expression expr) {
  struct expression_stack_node node;

  node.expr = expr;

  return node;
}
