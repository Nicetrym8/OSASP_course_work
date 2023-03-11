#include "about.h"

extern int ch;
extern screen_size scr_size;
extern bool exit_flag;

static void render_about_window();
static void on_about_resize_handler();
static void on_about_exit_handler();
static void about_refresher_handler();

const key_handler ABOUT_CONTROL_KEYS_HANDLERS[] = {
    {KEY_RESIZE, on_about_resize_handler},
    {KEY_F(3), on_about_exit_handler},
    {KEY_F(1),settings}
};
const render_routes ABOUT_RENDER_LIST[] = {
    render_key_map,
    render_about_window,
};
void about() {
    exit_flag = FALSE;
    clear();
    about_refresher_handler();
    while ((ch = wgetch(stdscr))) {
        for (size_t i = 0; i < ARRAY_SIZE(ABOUT_CONTROL_KEYS_HANDLERS);i++) {
            if (ABOUT_CONTROL_KEYS_HANDLERS[i].key == ch) {
                ABOUT_CONTROL_KEYS_HANDLERS[i].handler();
                ch = -1;
                break;
            }
        }
        if (exit_flag) return;
    }
}
static void on_about_exit_handler() {
    exit_flag = TRUE;
}
static void about_refresher_handler() {
    getmaxyx(stdscr, scr_size.max_y, scr_size.max_x);
    for (size_t i = 0; i < ARRAY_SIZE(ABOUT_RENDER_LIST);i++) {
        ABOUT_RENDER_LIST[i]();
    }
}
static void on_about_resize_handler() {
    about_refresher_handler();
}
static void render_about_window() {
    WINDOW* description_win = newwin(scr_size.max_y - 5, 3 * scr_size.max_x / 4, 2, scr_size.max_x / 8);
    WINDOW* box_win = newwin(scr_size.max_y - 1, scr_size.max_x, 0, 0);
    wbkgd(box_win, COLOR_PAIR(3));
    wbkgd(description_win, COLOR_PAIR(3));
    box(box_win, 0, 0);
    mvwprintw(description_win, 0, 3 * scr_size.max_x / 8 - strlen(PROGRAM_NAME) / 2, PROGRAM_NAME);
    mvwaddstr(description_win, 2, 0, DESCRIPTION);
    wrefresh(box_win);
    wrefresh(description_win);
    delwin(description_win);
    delwin(box_win);
}
