// Copyright (c) 2018 Daniel Abrecht
// SPDX-License-Identifier: AGPL-3.0-or-later

#include <errno.h>
#include <internal/pane.h>


int tym_i_csq_disable(struct tym_i_pane_internal* pane){
  if(pane->sequence.integer_count != 1){
    errno = EINVAL;
    return -1;
  }
  struct tym_i_pane_screen_state* screen = &pane->screen[pane->current_screen];
  enum tym_i_decset_decres code = pane->sequence.integer[0];
  switch(code){
    case TYM_I_DSDR_APPLICATION_CURSOR_KEYS: screen->cursor_key_mode = TYM_I_CURSOR_KEY_MODE_NORMAL; break;
    case TYM_I_DSDR_MOUSE_MODE_X10:
    case TYM_I_DSDR_MOUSE_MODE_NORMAL:
    case TYM_I_DSDR_MOUSE_MODE_BUTTON:
    case TYM_I_DSDR_MOUSE_MODE_ANY: pane->mouse_mode = TYM_I_MOUSE_MODE_OFF; break;
    case TYM_I_DSDR_ALTERNATE_SCREEN_1:
    case TYM_I_DSDR_ALTERNATE_SCREEN_2:
    case TYM_I_DSDR_ALTERNATE_SCREEN_3: return tym_i_pane_set_screen(pane, TYM_I_SCREEN_DEFAULT);
    case TYM_I_DSDR_AUTO_WRAP_MODE: screen->wraparound_mode_off = true; break;
    case TYM_I_DSDR_ORIGIN_MODE: {
      screen->origin_mode = false;
      tym_i_pane_set_cursor_position( pane,
        TYM_I_SCP_PM_ORIGIN_RELATIVE, 0,
        TYM_I_SCP_SMM_NO_SCROLLING, TYM_I_SCP_PM_ORIGIN_RELATIVE, 0,
        TYM_I_SCP_SCROLLING_REGION_UNCROSSABLE, false
      );
    } break;
    case TYM_I_DSDR_APPLICATION_KEYPAD: screen->keypad_mode = TYM_I_KEYPAD_MODE_NORMAL; break;
  }
  return 0;
}
