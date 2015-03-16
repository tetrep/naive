#include "headers/headers.h"

// lexes from a given file descriptor until EOF
int lex (int fd, char *read_buffer, size_t read_buffer_size) {
  // init state
  struct lex_state state_p[1];
  state_p->c = state_p->in_str = state_p->in_str_esc = 0;
  state_p->ltt = lextoken_null;

  ssize_t bytes_read = 0;
  struct expression expr = alloc_empty_expression();
  struct lextoken *tok_p = NULL;

  // we need to save a stack of which part of the expression we're parsing
  struct lextoken *tok_p_stack[EXPR_DEPTH_LIMIT];
  uint64_t expr_depth = 0;

  // parse tokens as operator, then lhs, then rhs
  tok_p = &(expr.op);

  // fill read buffer and lex it, until EOF
  while (0 != (bytes_read = buffered_read(fd, read_buffer, read_buffer_size))) {
    for (ssize_t i = 0; i < bytes_read; i++) {
      state_p->c = read_buffer[i];
      switch (state_p->ltt = lex_char(state_p)) {

        case lextoken_nop:
          // NOP
          break;

        case lextoken_expr_open:
          tok_p_stack[expr_depth] = tok_p;
          expr_depth += 1;
          open_expr(&expr);
          tok_p = &(expr.op);
          break;

        case lextoken_expr_close:
          if (expr_depth) {
            expr_depth -= 1;
            tok_p = tok_p_stack[expr_depth];
            (*tok_p) = close_expr(&expr);
          } else {
            tok_p = &(expr.op);
          }
          break;

        case lextoken_operator:
        case lextoken_symbol:
        case lextoken_int:
          build_token(tok_p, state_p);
          break;

        case lextoken_str_delim:
          break;

        case lextoken_tok_delim:
          // time for a new token
          if (tok_p ==  &(expr.op)) {
            tok_p = &(expr.lhs);
          } else if (tok_p == &(expr.lhs)) {
            tok_p = &(expr.rhs);
          } else {
            perror("invalid lexer state; comma encountered after RHS of expression");
            abort();
          }
        break;
        default:
          //fprintf(stderr, "invalid character encountered: %i\n", read_buffer[i]);
          eval_print_expr(expr);
          // new expression!
          tok_p = &(expr.op);
          expr = alloc_empty_expression();
      }
    }
  }

  // we've hit EOF, we're done
  return 0;
}

void build_token(struct lextoken* lt, struct lex_state *state_p) {
  if (lextoken_null == lt->type || lextoken_nop == lt->type) {
    // we don't have a type, so adopt whatever we've been given
    lt->type = state_p->ltt;
  }

  switch (lt->type) {
    // dat nop
    case lextoken_nop:
      break;

    case lextoken_operator:
      // only build the token if it's the same type
      if (lt->type == state_p->ltt) {
        // totes memory manage
        lt->val[lt->used] = state_p->c;
        lt->used += 1;
      }
      break;

    case lextoken_symbol:
      // only build the token if it's the same type
      if (lt->type == state_p->ltt) {
        // totes memory manage
        lt->val[lt->used] = state_p->c;
        lt->used += 1;
      }
      break;

    case lextoken_int:
      // only build the token if it's the same type
      if (lt->type == state_p->ltt) {
        // totes memory manage
        int64_t *ptr = (int64_t*) lt->val;
        *ptr *= 10;
        *ptr += state_p->c - 48;
      }
      break;

    case lextoken_expr_open:
    case lextoken_expr_close:
    case lextoken_str_delim:
    case lextoken_eoe:
    default:
      // NOP
      break;
  }
}

// save expr to stack and then wipe it
void open_expr(struct expression *expr_p) {
  push_expr(*expr_p);
  *(expr_p) = alloc_empty_expression();
}

// eval expr, overwrite it with expr popped from stack
// and return value of eval
struct lextoken close_expr(struct expression *expr_p) {
  struct lextoken exp = eval_expr(*expr_p);
  (*expr_p) = pop_expr();

  return exp;
}

lextoken_type lex_char (struct lex_state *state_p) {
  lextoken_type ltt;

  switch (state_p->c) {
    case '\\':
      ltt = lextoken_str_esc;
      break;
    case '"':
      ltt = lextoken_str_delim;
      break;
    case ' ':
      ltt = lextoken_tok_delim;
      break;
    case '[':
      ltt = lextoken_expr_open;
      break;
    case ']':
      ltt = lextoken_expr_close;
      break;
    case '+':
    case '-':
    case '*':
    case '/':
    case '=':
    case '|':
      ltt = lextoken_operator;
      break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      ltt = lextoken_int;
      break;
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'v':
    case 'w':
    case 'x':
    case 'y':
    case 'z':
      ltt = lextoken_symbol;
      break;
    default:
      // no matches
      ltt = lextoken_eoe;
  }

  // str logic
  ltt = lex_str_logic(ltt, state_p);

  return ltt;
}

lextoken_type lex_str_logic (lextoken_type ltt, struct lex_state *state_p) {
  // don't do anything if we're not in a str
  if (state_p->in_str) {
    // check if we're the escapee
    if (state_p->in_str_esc) {
      ltt = lextoken_symbol;
      state_p->in_str_esc = 0;
    }
    // check if we're the escaper
    else if (lextoken_str_esc == ltt) {
      ltt = lextoken_nop;
      state_p->in_str_esc = 1;
    }
    // check if we're ending the str
    else if (lextoken_str_delim == ltt) {
      ltt = lextoken_nop;
      state_p->in_str = 0;
    }
    // nothing special
    else {
      ltt = lextoken_symbol;
    }
  }
  // are we starting a string?
  else if (lextoken_str_delim == ltt) {
    ltt = lextoken_nop;
    state_p->in_str = 1;
  }
  // str esc is just symbol outside a str
  else if (lextoken_str_esc == ltt) {
    ltt = lextoken_symbol;
  }

  return ltt;
}

// fill a buffer
ssize_t buffered_read (int fd, void* buffer, size_t buffer_size) {
  ssize_t ret = -1;
  if (-1 == (ret = read(fd, buffer, buffer_size))) {
    perror("could not read from fd");
    abort();
  }

  return ret;
}

void print_prompt() {
  printf(">");
  fflush(stdout);
}

// sets up file descriptor for lexer
int main (int argc, char *argv[]) {
  // read stdin by default
  int fd = 0;
  // read from file if given one
  if (2 == argc) {
    if(-1 == (fd = open(argv[1], O_RDONLY))) {
      // we couldn't open the file
      perror("could not open file");
      abort();
    }
  } else if (2 < argc) {
    // unknown arguments given
    perror("unknown arguments given");
    abort();
  }

  // prompt
  print_prompt();

  // read buffer
  char buffer[1 << 20];
  return lex(fd, buffer, sizeof(buffer));
}
