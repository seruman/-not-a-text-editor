#include <termbox.h>
#include "backbuffer.h"
#include "tui.h"

int main(int argc, char *argv[]) {



    buffer *buf = buffer_new();
    buffer_insert_head(buf);
    output = fopen("log.txt", "w");


    if (argc > 1) {
        FILE *text_file = fopen(argv[1], "r");
        char *line = NULL;
        size_t len = 0;
        ssize_t read;

        if (text_file == NULL)
            exit(EXIT_FAILURE);

        while ((read = getline(&line, &len, text_file)) != -1) {
            line[read - 1] = '\0';
            buffer_puts(buf, line);
            buffer_insert_end(buf);
            buf->pos.cursorY += 1;
            buf->pos.cursorX = 0;
        }
        buf->filename = argv[1];

        fclose(text_file);
    }else{
        char filename[20];
        printf("Dosya adi [untitled]: ");
        scanf("%s", filename);

        buf->filename = filename;

    }


    int ret = tb_init();
    if (ret) {
        fprintf(stderr, "tb_init() failed with error code %d\n", ret);
        return 1;
    }
    tb_set_cursor(0, 0);


    tui_update(buf);

    struct tb_event ev;
    while (tb_poll_event(&ev)) {
        switch (ev.type) {
            case TB_EVENT_KEY:
                switch (ev.key) {
                    case TB_KEY_ESC:
                        goto done;
                        break;
                    case TB_KEY_ARROW_LEFT:
                        // MOVE CURSOR LEFT
                        tui_move_cursor_left(buf);
                        break;
                    case TB_KEY_ARROW_RIGHT:
                        // MOVE CURSOR RIGHT
                        tui_move_cursor_right(buf);
                        break;
                    case TB_KEY_ARROW_UP:
                        tui_move_cursor_up(buf);
                        break;
                    case TB_KEY_ARROW_DOWN:
                        tui_move_cursor_down(buf);
                        break;
                    case TB_KEY_BACKSPACE2:
                        tui_delete(buf);
                        break;
                    case TB_KEY_CTRL_S:
                        tui_save(buf);
                        break;
                    default:
                        tui_insert(ev, buf);
                }
                break;
        }
        tb_present();

    }
    done:
    //buffer free
    //TODO: Implement buffer_free(bufer*)
    tb_shutdown();


    return 0;
}