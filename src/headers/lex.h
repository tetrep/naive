#ifndef LEX_HEADER
#define LEX_HEADER

#include <unistd.h>

#include "types.h"

struct lextoken _empty_token;
struct expression _empty_expression;
char _max_token_size[MAX_TOKEN_SIZE];


int lex(int fd, char *read_buffer, size_t read_buffer_size);
lextoken_type lex_char(char c);

void build_token(struct lextoken *lt_p, lextoken_type ltt, char c);

void open_expr(struct expression *expr_p);
struct lextoken close_expr(struct expression *expr_p);

ssize_t buffered_read(int fd, void *buffer, size_t buffer_size);

void print_prompt();

#endif
