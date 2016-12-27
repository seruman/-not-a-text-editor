
#include "tui.h"
#include "backbuffer.h"


void tui_insert(struct tb_event ev, buffer *buf) {
    tb_clear();


    char out;
    bool newline = false;

    //unicode to ascii
    tb_utf8_unicode_to_char(&out, ev.ch);

    // Handle special character
    if (ev.key == TB_KEY_SPACE) {
        out = ' ';
    } else if (ev.key == TB_KEY_TAB) {
        out = '\t';
    } else if (ev.key == TB_KEY_ENTER) {
        out = '\n';
        newline = true;
    }


    if (newline) {

        if (buf->pos.line_num == 1) {
            buffer_insert_end(buf);
        } else {
            buffer_insert_kth(buf, buf->selected->line_num + 1);
        }
        buf->pos.cursorY += 1;
        buf->pos.cursorX = 0;
    } else {
        buf->pos.cursorX += 1;
        buffer_put(buf, out);

    }


    //update termbox
    tui_update(buf);
}

void tui_delete(buffer *buf) {

    if (line_is_empty(buf->selected)) {
        if (buf->pos.line_num != 1) {
            buffer_remove_line_selected(buf);
            buf->pos.cursorX = buf->selected->gcursor;

            if (buf->pos.cursorY - 1 >= 0) {
                buf->pos.cursorY -= 1;
            }
            tui_update(buf);
            return;
        }
    }


    if (buf->pos.cursorX > 0) {
        buf->pos.cursorX--;

    }
    buffer_delete(buf);


    tui_update(buf);
}

void tui_update(buffer *buf) {
    tb_clear();
    line *tmp = buf->head;
    int y = 0;
    int x;

    while (tmp != NULL) {


        int i = 0;
        x = 0;
        gap_T gb = tmp->gbuf;
        while (i <= gb->end) {
            if (i >= gb->gap_start && i <= gb->gap_end) {
                i++;
                continue;
            } else {
                if (gb->buffer[i] != '\0') {


                    uint32_t out;
                    tb_utf8_char_to_unicode(&out, &gb->buffer[i]);
                    tb_change_cell(x, y, out, TB_WHITE, TB_DEFAULT);
                    x += 1;
                }
            }
            i++;
        }
        y++;

        gap_buffer_print(tmp->gbuf, output);
        fprintf(output, "-----linenum: %d length:%d , gcursor : %d \n ", tmp->line_num, tmp->length, tmp->gcursor);
        tmp = tmp->next;
    }
    fprintf(output, "buf.pos.x = %d, buf.pos.y = %d\n", buf->pos.cursorX, buf->pos.cursorY);
    fprintf(output, "selected line : %d, \n", buf->selected->line_num);
    fprintf(output, "total line: %d\n", buf->pos.line_num);

    tui_set_cursor(buf->pos);
    fprintf(output, "############################\n");


}

void tui_move_cursor_up(buffer *buf) {
    if (buf->head != NULL && buf->selected->line_num > 1) {
        buffer_move_cursor_up(buf);
        buf->pos.cursorX = 0;
        buf->pos.cursorY -= 1;

        tui_set_cursor(buf->pos);
        tui_update(buf);

    }

}

void tui_move_cursor_down(buffer *buf) {
    if (buf->pos.cursorY + 1 < buf->pos.line_num) {
        buffer_move_cursor_down(buf);
        buf->pos.cursorX = 0;
        buf->pos.cursorY += 1;

        tui_set_cursor(buf->pos);
        tui_update(buf);

    }


}

void tui_move_cursor_right(buffer *buf) {
    buffer_move_cursor_right(buf);
    buf->pos.cursorX = buf->selected->gcursor;
    tui_set_cursor(buf->pos);
    tui_update(buf);


}

void tui_move_cursor_left(buffer *buf) {
    buffer_move_cursor_left(buf);
    buf->pos.cursorX = buf->selected->gcursor;
    tui_set_cursor(buf->pos);
    tui_update(buf);
}

void tui_set_cursor(position pos) {
    tb_set_cursor(pos.cursorX, pos.cursorY);
}


void tui_save(buffer *buf) {
    FILE *output = fopen(buf->filename, "w");
    line *tmp = buf->head;
    int y = 0;
    int x;

    while (tmp != NULL) {


        int i = 0;
        x = 0;
        gap_T gb = tmp->gbuf;
        while (i <= gb->end) {
            if (i >= gb->gap_start && i <= gb->gap_end) {
                i++;
                continue;
            } else {
                if (gb->buffer[i] != '\0') {

                    fprintf(output, "%c", gb->buffer[i]);
                    x += 1;
                }
            }
            i++;
        }
        y++;
        fprintf(output, "%c", '\n');

        tmp = tmp->next;
    }
}
