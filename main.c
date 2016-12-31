#include <termbox.h>
#include "backbuffer.h"
#include "tui.h"

int main(int argc, char *argv[]) {



    // Boş Buffer oluştu
    buffer *buf = buffer_new();

    //Buffer linked list olarak tutuluyor, başa boş satır eklendi
    buffer_insert_head(buf);

    output = fopen("log.txt", "w");

    // eğer dosya ismi parametre olarak geldiyse
    if (argc > 1) {
        FILE *text_file = fopen(argv[1], "r");
        // satırın uzunluğunu bilmediğim için pointer olarak tuttum
        char *line = NULL;
        size_t len = 0;
        ssize_t read;

        // dosya açılamadıysa
        if (text_file == NULL)
            exit(EXIT_FAILURE);

        while ((read = getline(&line, &len, text_file)) != -1) {
            // getline okuduktan sonra stringi bitirmiyor, o yüzden biz ekliyoruz
            line[read - 1] = '\0';

            // bufferda daha önce oluşturulan satıra stringi at
            buffer_puts(buf, line);

            // okumaya devam edebiliriz, o yüzden sona yeni bir satır ekle
            buffer_insert_end(buf);

            // arayüzde imleci bir aşağıya indir
            buf->pos.cursorY += 1;
            buf->pos.cursorX = 0;
        }

        // dosya ismini daha sonra kullanmak için kaydet
        buf->filename = argv[1];

        fclose(text_file);
    }else{
        char filename[20];
        printf("Dosya adi [untitled]: ");
        scanf("%s", filename);

        buf->filename = filename;

    }

    // termbox'ı başlat
    int ret = tb_init();
    if (ret) {
        fprintf(stderr, "tb_init() failed with error code %d\n", ret);
        return 1;
    }
    // başlangıçta arayüz imleci sol üste
    tb_set_cursor(0, 0);

    // bufferdaki karakterleri arayüze yaz
    tui_update(buf);

    struct tb_event ev;
    // tb_poll_event bloklayan bir çağrı, döngü sürekli dönmüyor
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
        // termboxı güncelle
        tb_present();

    }
    done:
    //buffer free
    //TODO: Implement buffer_free(bufer*)
    tb_shutdown();


    return 0;
}