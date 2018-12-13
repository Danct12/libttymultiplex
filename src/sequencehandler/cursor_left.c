// Copyright (c) 2018 Daniel Abrecht
// SPDX-License-Identifier: AGPL-3.0-or-later

#include <errno.h>
#include <internal/pane.h>

int tym_i_csq_cursor_left(struct tym_i_pane_internal* pane){
  if(pane->sequence.integer_count > 1){
    errno = ENOENT;
    return -1;
  }
  unsigned x = 1;
  if(pane->sequence.integer_count)
    x = pane->sequence.integer[0];
  unsigned new_x = 0;
  if(x < pane->cursor.x)
    new_x = pane->cursor.x - x;
  tym_i_pane_cursor_set_cursor(pane, new_x, pane->cursor.y);
  return 0;
}
