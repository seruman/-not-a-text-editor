#ifndef BAHADIR_PROLAB_TUI_H
#define BAHADIR_PROLAB_TUI_H


#include <termbox.h>
#include <stdbool.h>
#include "backbuffer.h"
FILE *output;

void tui_insert(struct tb_event ev, buffer*);
void tui_delete(buffer*);
void tui_update(buffer *);
void tui_set_cursor(position pos);

void tui_move_cursor_right(buffer *);
void tui_move_cursor_left(buffer *);
void tui_move_cursor_up(buffer *);
void tui_move_cursor_down(buffer *);
void tui_save(buffer *);

#endif //BAHADİR_PROLAB_TUİ_H
