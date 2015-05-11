#ifndef BTRSTR_HEADER
#define BTRSTR_HEADER

#include "types.h"

char btrstr_cmp(struct lextoken *lt1, struct lextoken *lt2);
char btrstr_cpy(struct lextoken *lhs_p, struct lextoken *rhs_p);
char btrstr_make(struct lextoken *lex_target, char *cstr, size_t cstr_len);
char btrstr_calloc(struct lextoken *dest_p, size_t size);

struct lextoken alloc_sized_token(size_t size);
struct lextoken make_empty_token();
struct expression alloc_empty_expression();

char free_token(struct lextoken token);
char free_expression(struct expression expr);

void lextoken_append_char(char c);
#endif
