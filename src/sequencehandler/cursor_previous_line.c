// Copyright (c) 2018 Daniel Abrecht
// SPDX-License-Identifier: AGPL-3.0-or-later

#include <errno.h>
#include <internal/pane.h>

int tym_i_csq_cursor_previous_line(struct tym_i_pane_internal* pane){
  if(pane->sequence.integer_count > 1){
    errno = ENOENT;
    return -1;
  }
  struct tym_i_pane_screen_state* screen = &pane->screen[pane->current_screen];
  unsigned y = 1;
  if(pane->sequence.integer_count)
    y = pane->sequence.integer[0];
  unsigned new_y = 0;
  if(screen->cursor.y - y > 0)
    new_y = screen->cursor.y - y;
  tym_i_pane_cursor_set_cursor(pane, 0, new_y, TYM_I_SMB_ORIGIN_MODE);
  return 0;
}
