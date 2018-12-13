// Copyright (c) 2018 Daniel Abrecht
// SPDX-License-Identifier: AGPL-3.0-or-later

#include <errno.h>
#include <internal/pane.h>

int tym_i_csq_cursor_down(struct tym_i_pane_internal* pane){
  if(pane->sequence.integer_count > 1){
    errno = ENOENT;
    return -1;
  }
  unsigned y = 1;
  if(pane->sequence.integer_count)
    y = pane->sequence.integer[0];
  unsigned new_y = pane->coordinates.position[TYM_P_CHARFIELD][1].axis[1].value.integer - pane->coordinates.position[TYM_P_CHARFIELD][0].axis[1].value.integer - 1;
  if(pane->cursor.y + y < new_y)
    new_y = pane->cursor.y + y;
  tym_i_pane_cursor_set_cursor(pane, pane->cursor.x, new_y);
  return 0;
}
