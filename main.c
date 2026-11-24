#include <ctype.h>
#include <ncurses.h>
#include <string.h>

static void draw_hline_clean(int y, int x_start, int x_end) {
    if (y < 0 || y >= LINES || x_end < x_start) {
        return;
    }
    if (x_start < 0) x_start = 0;
    if (x_end >= COLS) x_end = COLS - 1;
    mvhline(y, x_start, ACS_HLINE, x_end - x_start + 1);
}

static void draw_ui(int ch, int has_key) {
    const char *title = "KEYBOARD TESTER";
    const char *footer = "BY QUARTZ - version V1.1";

    erase();


    if (LINES >= 8 && COLS >= 36) {
        attron(COLOR_PAIR(1));
        box(stdscr, 0, 0);


        int header_y = 2;
        mvaddch(header_y, 0, ACS_LTEE);
        draw_hline_clean(header_y, 1, COLS - 2);
        mvaddch(header_y, COLS - 1, ACS_RTEE);

        int title_x = (COLS - (int)strlen(title)) / 2;
        if (title_x < 2) title_x = 2;
        mvprintw(1, title_x, "%s", title);

        int mid_y = LINES / 2 - 2;
        if (mid_y < header_y + 1) mid_y = header_y + 1;

        int line_width = COLS - 10;
        if (line_width < 16) line_width = COLS - 4;
        int line_x = (COLS - line_width) / 2;
        if (line_x < 1) line_x = 1;

        const char *label_detected = "DETECTED KEY";
        mvprintw(mid_y, (COLS - (int)strlen(label_detected)) / 2, "%s", label_detected);

        int line1_y = mid_y + 1;
        draw_hline_clean(line1_y, line_x, line_x + line_width - 1);

        int char_y = line1_y + 1;
        if (!has_key) {
            const char *placeholder = "-";
            mvprintw(char_y, (COLS - (int)strlen(placeholder)) / 2, "%s", placeholder);
        } else if (ch >= 0 && ch <= 255 && isprint((unsigned char)ch)) {
            mvprintw(char_y, COLS / 2, "%c", (char)ch);
        } else {
            const char *kname = keyname(ch);
            if (!kname) kname = "NON-PRINT";
            mvprintw(char_y, (COLS - (int)strlen(kname)) / 2, "%s", kname);
        }

        int line2_y = char_y + 1;
        draw_hline_clean(line2_y, line_x, line_x + line_width - 1);

        int ascii_y = line2_y + 1;
        char ascii_buf[64];
        if (!has_key) {
            snprintf(ascii_buf, sizeof(ascii_buf), "ascii value []");
        } else {
            snprintf(ascii_buf, sizeof(ascii_buf), "ascii value [%d]", ch);
        }
        mvprintw(ascii_y, (COLS - (int)strlen(ascii_buf)) / 2, "%s", ascii_buf);


        int footer_x = COLS - (int)strlen(footer) - 2;
        if (footer_x < 1) footer_x = 1;
        mvprintw(LINES - 2, footer_x, "%s", footer);

        attroff(COLOR_PAIR(1));
    } else {
        mvprintw(0, 0, "samll window  (min. 36x8)");
    }

    refresh();
}

int main(void) {
    int ch = 0;
    int has_key = 0;

    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    if (has_colors()) {
        start_color();
        use_default_colors();

        init_pair(1, COLOR_WHITE, -1);
    }

    draw_ui(0, 0);

    while ((ch = getch()) != KEY_F(10)) {
        if (ch == KEY_RESIZE) {
            draw_ui(ch, has_key);
            continue;
        }
        has_key = 1;
        draw_ui(ch, has_key);
    }

    endwin();
    return 0;
}