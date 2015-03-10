#ifndef BTRSTR_HEADER
#define BTRSTR_HEADER

#include "types.h"

char btrstr_cmp (struct lextoken *lt1, struct lextoken *lt2);
char btrstr_cpy (struct lextoken *lhs_p, struct lextoken *rhs_p);
char btrstr_make (struct lextoken *lex_target, char *cstr);
char btrstr_calloc (struct lextoken *dest_p, size_t size);
#endif
