#ifndef BAHADIR_PROLAB_BACKBUFFER_H
#define BAHADIR_PROLAB_BACKBUFFER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gapbuffer/src/lib/gap_buffer.h"

typedef struct line_s line;

struct line_s {
    gap_T gbuf;     // gap buffer for line
    line *next;     // next line
    line *prev;
    int line_num;
    int gcursor;
    int length;
};

typedef struct {
    int offset;     // column offset
    int cursorX;    // X value for cursor
    int cursorY;    // Y value for cursor
    int line_num;   // currently selected line
} position;

typedef struct {
    line *head;     // first line in buffer
    line *selected; // active line
    line *end;      // last line in buffer
    position pos;
    char * filename;
} buffer;

/*
 *  BUFFER STUFF
 */

buffer *buffer_new();

void buffer_insert_head(buffer *handle);

void buffer_insert_end(buffer *handle);

void buffer_insert_kth(buffer *handle, int idx);

void buffer_append_empty_line(buffer *);

void buffer_insert_empty_line_to(buffer *, int);

void buffer_put(buffer *handle, char c);

void buffer_puts(buffer *, char *);

void buffer_insert_after(buffer *handle, int idx);


//void buffer_move_cursor(buffer* handle, int, int);

void buffer_move_cursor_up(buffer *);

void buffer_move_cursor_down(buffer *);

void buffer_move_cursor_left(buffer *);

void buffer_move_cursor_right(buffer *);

void buffer_delete(buffer *handle);

void buffer_remove_line_head(buffer *handle);
void buffer_remove_line_end(buffer *handle);
void buffer_remove_line_kth(buffer *handle, int idx);
void buffer_remove_line_selected(buffer *handle);



/*
 *  LINE STUFF
 */

line *line_new();

void line_insert(line *, char);

void line_delete(line *);

void line_move_cursor_right(line *line_handle, int idx);

void line_move_cursor_left(line *line_handle, int idx);

void line_move_cursor(line *line_handle, int idx);

int line_is_empty(line *);

#endif //BAHADIR_PROLAB_BACKBUFFER_H
