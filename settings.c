#include "settings.h"


extern int ch;
extern screen_size scr_size;
extern bool exit_flag;

static void on_settings_resize_handler();
static void on_settings_exit_handler();
static void settings_refresher_handler();
static void render_settings_window();

const key_handler SETTINGS_CONTROL_KEYS_HANDLERS[] = {
    {KEY_RESIZE, on_settings_resize_handler},
    {KEY_F(3), on_settings_exit_handler},
    {KEY_F(2),about}
};
const render_routes SETTINGS_RENDER_LIST[] = {
    render_key_map,
    render_settings_window,
};
void settings() {
    exit_flag = FALSE;
    clear();
    settings_refresher_handler();
    while ((ch = wgetch(stdscr))) {
        for (size_t i = 0; i < ARRAY_SIZE(SETTINGS_CONTROL_KEYS_HANDLERS);i++) {
            if (SETTINGS_CONTROL_KEYS_HANDLERS[i].key == ch) {
                SETTINGS_CONTROL_KEYS_HANDLERS[i].handler();
                ch = -1;
                break;
            }
        }
        if (exit_flag) return;
    }
}
static void settings_refresher_handler() {
    getmaxyx(stdscr, scr_size.max_y, scr_size.max_x);
    for (size_t i = 0; i < ARRAY_SIZE(SETTINGS_RENDER_LIST);i++) {
        SETTINGS_RENDER_LIST[i]();
    }
}
static void render_settings_window() {
    WINDOW* box_win = newwin(scr_size.max_y - 1, scr_size.max_x, 0, 0);
    wbkgd(box_win, COLOR_PAIR(3));
    box(box_win, 0, 0);
    wrefresh(box_win);
    delwin(box_win);
}
static void on_settings_resize_handler() {
    settings_refresher_handler();
}
static void on_settings_exit_handler() {
    exit_flag = TRUE;
}