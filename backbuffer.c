#include "backbuffer.h"

buffer *buffer_new() {
    buffer *b = malloc(sizeof(buffer));
    b->head = NULL;
    b->end = NULL;
    b->selected = NULL;

    b->pos = (position) {.cursorX = 0, .cursorY = 0, .line_num = 0, .offset = 0};
    return b;
}

void buffer_insert_head(buffer *handle) {
    line *new_line = line_new();
    new_line->next = handle->head;
    if (handle->head != NULL) {
        handle->head->prev = new_line;
    }
    handle->head = new_line;
    if (handle->pos.line_num == 0) {
        handle->end = new_line;
    }
    handle->selected = new_line;
    new_line->line_num = 1;
    handle->pos.line_num += 1;

    //Increment nums after this one
    line *tmp = handle->selected->next;
    while (tmp != NULL) {
        tmp->line_num += 1;
        tmp = tmp->next;
    }
    free(tmp);
}

void buffer_insert_kth(buffer *handle, int idx) {
    line *new_line = line_new();
    line *tmp = handle->selected;

    if (handle->selected->line_num > idx) {
        while (tmp != handle->head) {
            if (tmp->line_num == idx) {
                break;
            }
            tmp = tmp->prev;
        }
    } else {
        while (tmp != handle->end) {
            if (tmp->line_num == idx) {
                break;
            }
            tmp = tmp->next;
        }

    }

    if (tmp == handle->head) {
        return buffer_insert_head(handle);
    }

    if (tmp == handle->end) {
        return buffer_insert_end(handle);
    }

    handle->selected = new_line;

    new_line->prev = tmp;
    new_line->next = tmp->next;
    new_line->next->prev = new_line;
    tmp->next = new_line;


    new_line->line_num = new_line->prev->line_num + 1;
    handle->pos.line_num = new_line->line_num;

    tmp = handle->selected->next;
    while (tmp != NULL) {
        tmp->line_num += 1;
        tmp = tmp->next;
    }


}

void buffer_insert_end(buffer *handle) {
    line *new_line = line_new();
    handle->end->next = new_line;
    new_line->prev = handle->end;
    handle->end = new_line;

    handle->selected = new_line;
    new_line->line_num = new_line->prev->line_num + 1;
    handle->pos.line_num += 1;

}

void buffer_insert_after(buffer *handle, int idx) {
    line *new_line = line_new();
    line *tmp = handle->selected;


    if (handle->selected->line_num > idx) {
        while (tmp != handle->head) {
            if (tmp->line_num == idx) {
                break;
            }
            tmp = tmp->prev;
        }
    } else {
        while (tmp != handle->end) {
            if (tmp->line_num == idx) {
                break;
            }
            tmp = tmp->next;
        }

    }

    if (tmp == handle->head) {

        new_line->next = handle->head;
        if (handle->head != NULL) {
            handle->head->prev = new_line;
        }
        handle->head = new_line;
        handle->selected = new_line;
        return;
    } else if (tmp == handle->end) {
        handle->end->next = new_line;
        new_line->prev = handle->end;
        handle->end = new_line;
        return;
    }
    new_line->next = tmp->next;
    tmp->next->prev = new_line;
    new_line->prev = tmp->prev;

    handle->selected = new_line;

    new_line->line_num = new_line->prev->line_num + 1;
    handle->pos.line_num = new_line->line_num;

    tmp = handle->selected->next;
    while (tmp != NULL) {
        tmp->line_num += 1;
        tmp = tmp->next;
    }
}

void buffer_append_empty_line(buffer *handle) {
    line *new_line = line_new();

    // insert head
    if (handle->head == NULL) {
        handle->head = new_line;
        handle->end = new_line;
        handle->selected = new_line;
        new_line->line_num = 1;
        handle->pos.line_num = 1;
    } else {
        handle->selected = new_line;
        handle->end->next = new_line;
        new_line->prev = handle->end;
        handle->end = new_line;

        new_line->line_num = new_line->prev->line_num + 1;
        handle->pos.line_num = new_line->line_num;

    }
}

//void buffer_insert_empty_line_to(buffer *handle, int to) {
//    if (to > handle->end->line_num) {
//        return;
//    }
//
//    if(handle->head == NULL){
//        buffer_append_empty_line(handle);
//        return;
//    }
//
//
//    line *new_line = line_new();
//    line *tmp = handle->selected;
//
//
//    if (handle->selected->line_num > to) {
//        while (tmp != handle->head) {
//            if (tmp->line_num == to - 1) {
//                break;
//            }
//            tmp = tmp->prev;
//        }
//    } else {
//        while (tmp != handle->end) {
//            if (tmp->line_num == to - 1) {
//                break;
//            }
//            tmp = tmp->next;
//        }
//
//    }
//
////    Vertex newVtx = new Vertex(input)
////    newVtx.next = sonra, sonra.prev = newVtx
////    once.next = newVtx, newVtx.prev = once
//    handle->selected = new_line;
//
//    new_line->next = tmp->next;
//    tmp->next->prev = new_line;
//    tmp->prev->next = new_line;
//    new_line->prev = tmp->prev;
//    //SİLME
////    handle->selected = new_line;
////    new_line->prev = tmp;
////    new_line->next = tmp->next;
////    new_line->next->prev = new_line;
////    tmp->next = new_line;
//
//
////    newNode->PREV = curPtr
////
////    newNode->NEXT = curPtr->NEXT
////
////    newNode->NEXT->PREV = newNode
////
////    curPtr->NEXT = newNode
//
//
//    new_line->line_num = new_line->prev->line_num + 1;
//    handle->pos.line_num = new_line->line_num;
//
//    tmp = handle->selected->next;
//    while (tmp != NULL) {
//        tmp->line_num += 1;
//        tmp = tmp->next;
//    }
//
////    handle->end->line_num += 1;
//}


void buffer_put(buffer *handle, char c) {
    line_insert(handle->selected, c);
}

void buffer_puts(buffer *handle, char *str) {
    for (int i = 0; i < strlen(str); i++) {
        line_insert(handle->selected, str[i]);
    }
}

void buffer_move_cursor_up(buffer *handle) {
    if (handle->head != NULL) {
        if (handle->selected->prev != NULL) {
            handle->selected = handle->selected->prev;
            //handle->pos.line_num = handle->selected->line_num;

            //FIXME:
            handle->selected->gbuf->cursor = 0;
            handle->selected->gcursor = 0;
        }
    }
}

void buffer_move_cursor_down(buffer *handle) {
    if (handle->head != NULL) {
        if (handle->selected->next != NULL) {
            handle->selected = handle->selected->next;
            //handle->pos.line_num = handle->selected->line_num;
            //FIXME:
            handle->selected->gbuf->cursor = 0;
            handle->selected->gcursor = 0;

        }
    }
}


void buffer_move_cursor_left(buffer *handle) {
    if (handle->head != NULL) {
        if (handle->selected != NULL) {
            line_move_cursor_left(handle->selected, -1);
        }
    }
}

void buffer_move_cursor_right(buffer *handle) {
    if (handle->head != NULL) {
        if (handle->selected != NULL) {
            line_move_cursor_right(handle->selected, 1);
        }
    }
}

void buffer_delete(buffer *handle) {
    line_delete(handle->selected);
}


//if empty, do nothing
//        temp = head
//head = head.next
//delete temp
//if (head != null) head.prev = null
void buffer_remove_line_head(buffer *handle) {
    if (handle->head != NULL) {
        line *tmp = handle->head;
        handle->head = handle->head->next;
        //free tmp
        if (handle->head != NULL) {
            handle->head->prev = NULL;
        }

        handle->selected = handle->head;
        handle->pos.line_num--;


        int i = 1;
        tmp = handle->head;
        while (tmp != NULL) {
            tmp->line_num = i;
            tmp = tmp->next;
            i++;
        }
    }
}

//if empty, do nothing
//temp = tail
//tail = tail.prev
//tail.next = null
//delete temp
void buffer_remove_line_end(buffer *handle) {
    if (handle->head != NULL) {
        line *tmp = handle->end;
        handle->end = handle->end->prev;
        handle->end->next = NULL;
        //free tmp

        handle->selected = handle->end;
        handle->pos.line_num--;

    }
}

void buffer_remove_line_selected(buffer *handle) {
    line *tmp = handle->selected;

    if (tmp == handle->head) {
        return buffer_remove_line_head(handle);
    }

    if (tmp == handle->end) {
        return buffer_remove_line_end(handle);
    }

    if (tmp->next) {
        tmp->next->prev = tmp->prev;
    }
    if (tmp->prev) {
        tmp->prev->next = tmp->next;
    }

    handle->selected = tmp->prev;
    handle->pos.line_num--;
    //free tmp
}

void buffer_remove_line_kth(buffer *handle, int idx) {
    line *tmp = handle->selected;

    if (handle->selected->line_num > idx) {
        while (tmp != handle->head) {
            if (tmp->line_num == idx) {
                break;
            }
            tmp = tmp->prev;
        }
    } else {
        while (tmp != handle->end) {
            if (tmp->line_num == idx) {
                break;
            }
            tmp = tmp->next;
        }

    }

    if (tmp == handle->head) {
        return buffer_remove_line_head(handle);
    }

    if (tmp == handle->end) {
        return buffer_remove_line_end(handle);
    }

    if (tmp->next) {
        tmp->next->prev = tmp->prev;
    }
    if (tmp->prev) {
        tmp->prev->next = tmp->next;
    }

    handle->selected = tmp->prev;
    //free tmp



}

line *line_new() {
    line *n = malloc(sizeof(line));
    n->gbuf = gap_buffer_new();
    n->next = NULL;
    n->prev = NULL;
    n->line_num = 0;
    n->gcursor = 0;
    n->length = 0;

    return n;
}

void line_insert(line *line_handle, char c) {
    gap_buffer_insert(line_handle->gbuf, c);
    line_handle->gcursor++;
    line_handle->length++;
}

void line_move_cursor_left(line *line_handle, int idx) {
    if (idx > 0) {
        return;
    }

    int res = gap_buffer_distance_to_start(line_handle->gbuf);
    int old_cursor = line_handle->gbuf->cursor;

    gap_buffer_move_cursor(line_handle->gbuf, idx);
    if (line_handle->gbuf->cursor != old_cursor) {
        line_handle->gcursor--;

    }
}

void line_move_cursor(line *line_handle, int idx) {
    int old_cursor = line_handle->gbuf->cursor;
    gap_buffer_move_cursor(line_handle->gbuf, idx);
    if (line_handle->gbuf->cursor != old_cursor) {
        line_handle->gcursor += idx;
    }

}

void line_move_cursor_right(line *line_handle, int idx) {
    if (idx < 0) {
        return;
    }
    int res = gap_buffer_distance_to_start(line_handle->gbuf);
    int old_cursor = line_handle->gbuf->cursor;

    gap_buffer_move_cursor(line_handle->gbuf, idx);
    if (line_handle->gbuf->cursor != old_cursor) {
        line_handle->gcursor++;

    }
}

void line_delete(line *line_handle) {
    if (gap_buffer_distance_to_start(line_handle->gbuf) != 0) {
        gap_buffer_delete(line_handle->gbuf);
        line_handle->gcursor--;
        line_handle->length--;
    }

}

int line_is_empty(line *line_handle) {
    return gap_buffer_is_empty(line_handle->gbuf);
}

