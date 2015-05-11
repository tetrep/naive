#ifndef LEX_HEADER
#define LEX_HEADER

#include <unistd.h>

#include "types.h"

int lex(int fd, char *read_buffer, size_t read_buffer_size);
lextoken_type lex_char(struct lex_state *state_p);
lextoken_type lex_str_logic(lextoken_type ltt, struct lex_state *state_p);

void build_token(struct lextoken *lt_p, struct lex_state *state_p);

struct expression open_expr(struct expression *expr_p);
struct lextoken close_expr(struct expression *expr_p);

ssize_t buffered_read(int fd, void *buffer, size_t buffer_size);

void print_prompt();

#endif
