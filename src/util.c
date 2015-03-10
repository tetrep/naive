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

char btrstr_calloc(struct lextoken *dest_p, size_t size) {
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
