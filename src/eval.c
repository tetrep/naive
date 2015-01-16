#include "headers/headers.h"

struct lextoken eval_expr (struct expression expr) {
  struct lextoken tok = _empty_token;
  if (lextoken_operator == expr.op.type) {

    // match against hex operator
    if (btrstr_cmp(&_hex_expr, &expr.op)) {
      hex_str(&expr);
    }
    // match agains b64 operator
    else if (btrstr_cmp(&_b64_expr, &expr.op)) {
      b64_str(&expr);
    }


    // TODO: deprectated, should all be compartmentalized into functions
    switch (expr.op.val[0]) {
      case '+':
        if (expr.lhs.type == expr.rhs.type) {
          tok.type = expr.lhs.type;
          *(tok.val) = *((int64_t*)expr.lhs.val) + *((int64_t*)expr.rhs.val);
        }
        break;
      case '-':
        if (expr.lhs.type == expr.rhs.type) {
          *(tok.val) = *((int64_t*)expr.lhs.val) - *((int64_t*)expr.rhs.val);
        }
        break;
      case '*':
        if (expr.lhs.type == expr.rhs.type) {
          *(tok.val) = *((int64_t*)expr.lhs.val) * *((int64_t*)expr.rhs.val);
        }
        break;
      case '/':
        if (expr.lhs.type == expr.rhs.type) {
          *(tok.val) = *((int64_t*)expr.lhs.val) / *((int64_t*)expr.rhs.val);
        }
        break;
      case '=':
      // return LHS if we can't identify operator
      default:
        tok = expr.lhs;
    }
  }
  // simply return operators that are not operators, we assume
  // they're results of expressions
  else {
    tok = expr.op;
  }

  return tok;
}

struct lextoken eval_print_expr (struct expression expr) {
  struct lextoken val = eval_expr(expr);
  print_tok(val);
  printf("\n");
  print_prompt();
  
  return val;
}

void print_expr(struct expression expr) {
  printf("[");
  print_tok(expr.op);
  printf(",");
  print_tok(expr.lhs);
  printf(",");
  print_tok(expr.rhs);
  printf("]\n");
}

void print_tok(struct lextoken tok) {
  if (tok.val) {
    switch (tok.type) {
      case lextoken_int:
        printf("%lli", *((int64_t*)tok.val));
        break;
      case lextoken_operator:
      case lextoken_symbol:
        tok.val[tok.used] = '\0';
        printf("%s", tok.val);
      case lextoken_null:
        // NOP
        //fprintf(stderr, "null token\n");
        break;
      case lextoken_tok_delim:
        fprintf(stderr, "delim token\n");
        break;
      case lextoken_expr_open:
        fprintf(stderr, "open expr token\n");
        break;
      case lextoken_expr_close:
        fprintf(stderr, "close expr token\n");
        break;
      case lextoken_str_delim:
        fprintf(stderr, "str delim token\n");
        break;
      case lextoken_eoe:
      default:
        // NOP
        //fprintf(stderr, "error, cannot print %u\n", tok.type);
        break;
    }
  }
}

void hex_str (struct expression *expr_p) {
}

void b64_str (struct expression *expr_p) {
}
