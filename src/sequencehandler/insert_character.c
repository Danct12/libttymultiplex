// Copyright (c) 2018 Daniel Abrecht
// SPDX-License-Identifier: AGPL-3.0-or-later

#include <errno.h>
#include <internal/pane.h>
#include <internal/backend.h>

int tym_i_csq_insert_character(struct tym_i_pane_internal* pane){
  if(pane->sequence.integer_count > 1){
    errno = ENOENT;
    return -1;
  }
  unsigned n = 1;
  if(pane->sequence.integer_count)
    n = pane->sequence.integer[0];
  if(n == 0) n = 1;
  struct tym_i_pane_screen_state* screen = &pane->screen[pane->current_screen];
  unsigned y = screen->cursor.y;
  unsigned x = screen->cursor.x;
  unsigned w = TYM_RECT_SIZE(pane->absolute_position, CHARFIELD, TYM_AXIS_HORIZONTAL);
  unsigned h = TYM_RECT_SIZE(pane->absolute_position, CHARFIELD, TYM_AXIS_VERTICAL);
  if(w == 0 || h == 0)
    return 0;
  if(x >= w)
    x = w-1;
  if(y >= h)
    y = h-1;
  if(n > w - x)
    n = w - x;
  while(n--)
    tym_i_backend->pane_set_character(pane, (struct tym_i_cell_position){.x=x,.y=y}, screen->character_format, 1, " ", true);
  return 0;
}
