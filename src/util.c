#include "headers/headers.h"

char btrstr_cmp (struct lextoken *lt1_p, struct lextoken *lt2_p) {
  char ret = 0;
  // make sure we're not comparing any nulls
  if (lt1_p && lt2_p && lt1_p->val && lt2_p->val) {
    // make sure they're the same size
    if (lt1_p->used == lt2_p->used) {
      // assume we're correct
      ret = 1;
      for (int i = 0; i < lt1_p->used; i++) {
        // break if we ever don't match
        if (ret = (lt1_p->val[i] != lt2_p->val[i])) { break; }
      }
    }
    else
    { ret = 0; }
  }
  else
  { ret = 0; }

  return ret;
}

char btrstr_cpy (struct lextoken *src_p, struct lextoken *dest_p) {
  // make sure we can fit in dest
  if (dest_p->allocated < src_p->used) {
    // TODO check for errors when they can exist
    btrstr_calloc(dest_p, src_p->used);
  }

  // we fit, so simply copy
  // TODO can this error? (osx) man page says: "The memmove() function returns the original value of dst."
  memmove(dest_p->val, src_p->val, src_p->used);

  // update size
  dest_p->used = src_p->used;

  return 0;
}

char btrstr_append (struct lextoken *src_p, char c) {
  // check size_t overflow
  if (src_p->used > (src_p->allocated + src_p->used)) {
    // too big!
    fprintf(stderr, "error: lextoken too large to append character");
    return -1;
  }
  if (src_p->allocated) {
  }
}

char btrstr_make (struct lextoken *dest_p, char *cstr, size_t cstr_len) {

  // make sure we can fit
  if (lex_target->allocated < cstr_len) {
    // TODO check for errors when they can exist
    btrstr_calloc(dest_p, cstr_len);
  }

  // we fit, so simply copy
  // TODO can this error? (osx) man page says: "The memmove() function returns the original value of dst."
  memmove(dest_p->val, cstr, cstr_len);

  // update size
  dest_p->used = cstr_len;

  return 0;
}

char btrstr_calloc (struct lextoken *dest_p, size_t size) {
  // free our dest, if needed
  if (NULL != dest_p->val) {
    free(dest_p->val);
    dest_p->val = NULL;
    dest_p->used = 0;
    dest_p->allocated = 0;
  }

  // allocate more memory
  if (NULL == (dest_p->val = calloc(size))) {
    fprintf(stderr, "error: failed to allocate %u bytes\n", size);
    // TODO we'll probably want to gracefully handle errors at some point
    abort();
  }

  // update our container's metadata
  dest_p->used = 0;
  dest_p->allocated = size;

  return 0;
}

struct lextoken alloc_sized_token (size_t size) {
  struct lextoken allocated_token = make_empty_token();

  // do we have anything we need to allocate?
  if (0 != size) {
    // TODO error handling once available
    btrstr_calloc(&allocated_token, size);
  }

  return allocated_token;
}

struct lextoken make_empty_token () {
  struct lextoken empty_token;
  empty_token.type = lextoken_null;
  empty_token.val = NULL;
  empty_token.used = 0;
  empty_token.allocated = 0;

  return empty_token;
}

void lextoken lextoken_realloc (struct lextoken *lt_p, size_t new_size) {
}

struct expression alloc_empty_expression () {
  struct expression empty_expression;
  empty_expression.op = make_empty_token();
  empty_expression.lhs = make_empty_token();
  empty_expression.rhs = make_empty_token();

  return empty_expression;
}

char free_token (struct lextoken token) {
  if (NULL != token.val) {
    free(token.val);
  }

  token.type = lextoken_null;
  token.used = 0;
  token.allocated = 0;
  return 0;
}

void lextoken_append_char (struct lextoken *lt_p, char c) {
  if (lt_p) {
    // make sure we can fit it
    if (lt_p->used == lt_p->allocated) {
      lt_p = lextoken_realloc(lt_p, lt_p->allocated*2);
    }

    lt_p->val[lt_p->used] = c;
    lt_p->used += 1;
  }
}
