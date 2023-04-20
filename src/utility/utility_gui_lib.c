#include "utility_gui_lib.h"

int ch;
bool exit_flag = FALSE;
screen_size scr_size;
static const toolbar TOOLBAR_NAMES_AND_KEYS[] = {
    {EXECUTE_GUI,"Enter"},
    {ADDITIONAL_COMMAND_GUI,"F1"},
    {ABOUT_GUI,"F2"},
    {EXIT_GUI,"F3"},
};
void default_key_handler(const key_handler* restrict control_key_handlers, size_t size) {
    for (size_t i = 0; i < size;i++) {
        if (control_key_handlers[i].key == ch) {
            control_key_handlers[i].handler();
            ch = -1;
            break;
        }
    }
}
void render_key_map() {
    static WINDOW* win = NULL;
    if (win == NULL) {
        win = newwin(1, scr_size.max_x, scr_size.max_y - 1, 0);
        wbkgd(win, COLOR_PAIR(1));
        wrefresh(win);
    }
    wclear(win);
    mvwin(win, scr_size.max_y - 1, 0);
    wrefresh(win);
    for (size_t i = 0; i < ARRAY_SIZE(TOOLBAR_NAMES_AND_KEYS);i++) {
        wattron(win, COLOR_PAIR(2));
        mvwprintw(win, 0, i * (scr_size.max_x / ARRAY_SIZE(TOOLBAR_NAMES_AND_KEYS)), "%s", TOOLBAR_NAMES_AND_KEYS[i].key_name);
        wattron(win, COLOR_PAIR(1));
        mvwprintw(win, 0, i * (scr_size.max_x / ARRAY_SIZE(TOOLBAR_NAMES_AND_KEYS)) + strlen(TOOLBAR_NAMES_AND_KEYS[i].key_name), " %s", TOOLBAR_NAMES_AND_KEYS[i].name);
    }
    // toolbar_render(win, TOOLBAR_NAMES_AND_KEYS, 4);
    wrefresh(win);
}
char* trimwhitespace(char* restrict str) {
    char* end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0)
        return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}