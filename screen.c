#include "screen.h"



/*****************SETTINGS********************/
const toolbar TOOLBAR_NAMES_AND_KEYS[] = {
    {EXECUTE_GUI,"Enter",KEY_ENTER, },
    {ADDITIONAL_COMMAND_GUI,"F1",KEY_F(1)},
    {ABOUT_GUI,"F2", KEY_F(2)},
    {EXIT_GUI,"F3", KEY_F(3)}
};
void on_del_char_handler();
void on_resize_handler();
void on_key_down_handler();
void on_key_up_handler();
const key_handler CONTROL_KEYS_HANDLERS_LIST[] = {
   {KEY_BACKSPACE, on_del_char_handler},
   {KEY_RESIZE,on_resize_handler},
   {ENTER_KEY,on_submit_handler},
   {KEY_DOWN,on_key_down_handler},
   {KEY_UP,on_key_up_handler},
};
const render_routes RENDER_LIST[] = {
    render_key_map,
    render_main_window,
    render_search_bar
};
/*****************END SETTINGS******************/
screen_size scr_size;
FIELD* field[2];
FORM* search_form = NULL;
WINDOW* search_form_win = NULL;
buf_t last_request;
int current_pos;
FILE* fpipe = NULL;
int total_lines;
//
void on_del_char_handler() {
    form_driver(search_form, REQ_DEL_PREV);
    form_driver(search_form, REQ_END_LINE);
    form_driver(search_form, REQ_VALIDATION);
}
void on_resize_handler() {
    strcpy(last_request, field_buffer(field[0], 0));
    refresher_handler();
}
void on_key_down_handler() {
    if (current_pos < total_lines - 1)current_pos++;
    render_main_window();
}
void on_key_up_handler() {
    if (current_pos > 0)current_pos--;
    render_main_window();
}
void default_key_handler() {

}
void render_main_window_gui() {
    int ch;
    main_window_gui_init();
    curs_set(0);
    while ((ch = wgetch(search_form_win)) != KEY_F(1)) {

        for (size_t i = 0; i < ARRAY_SIZE(CONTROL_KEYS_HANDLERS_LIST);i++) {
            if (CONTROL_KEYS_HANDLERS_LIST[i].key == ch) {
                CONTROL_KEYS_HANDLERS_LIST[i].handler();
                ch = -1;
                break;
            }
        }
        if (ch != -1) {
            form_driver(search_form, ch);
            form_driver(search_form, REQ_VALIDATION);
        }
    }
    unpost_form(search_form);
    free_form(search_form);
    free_field(field[0]);
    endwin();
}

/* Un post form and free the memory */

void refresher_handler() {
    getmaxyx(stdscr, scr_size.max_y, scr_size.max_x);
    for (size_t i = 0; i < ARRAY_SIZE(RENDER_LIST);i++) {
        RENDER_LIST[i]();
    }
}
void main_window_gui_init() {
    refresher_handler();

}
void on_submit_handler() {
    if (fpipe != NULL) {
        pclose(fpipe);
        fpipe = NULL;
    }
    current_pos = 0;
    strcpy(last_request, field_buffer(field[0], 0));
    refresher_handler();
}
void render_search_bar() {
    int rows, cols;
    if (search_form != NULL) {
        unpost_form(search_form);
        free_form(search_form);
        free_field(field[0]);
        delwin(search_form_win);
    }
    field[0] = new_field(1, scr_size.max_x - 2, 0, 0, 0, 0);
    field[1] = NULL;

    /* Set field options */
    set_field_back(field[0], COLOR_PAIR(3));
    set_field_fore(field[0], COLOR_PAIR(3));
    field_opts_off(field[0], O_AUTOSKIP); /* Don't go to next field when this */
    /* Field is filled up 		*/

    /* Create the form and post it */
    search_form = new_form(field);

    /* Calculate the area required for the form */
    scale_form(search_form, &rows, &cols);

    /* Create the window to be associated with the form */
    search_form_win = newwin(rows + 2, cols + 2, 0, 0);
    keypad(search_form_win, TRUE);

    /* Set main window and sub window */
    set_form_win(search_form, search_form_win);
    set_form_sub(search_form, derwin(search_form_win, rows, cols, 1, 1));

    /* Print a border around the main window and print a title */
    wbkgd(search_form_win, COLOR_PAIR(3));
    box(search_form_win, 0, 0);
    refresh();

    post_form(search_form);
    wrefresh(search_form_win);
    set_field_buffer(field[0], 0, last_request);
    form_driver(search_form, REQ_END_LINE);
    refresh();
}
void render_key_map() {
    WINDOW* win = newwin(1, scr_size.max_x, scr_size.max_y - 1, 0);
    wbkgd(win, COLOR_PAIR(1));
    refresh();

    for (size_t i = 0; i < ARRAY_SIZE(TOOLBAR_NAMES_AND_KEYS);i++) {
        wattron(win, COLOR_PAIR(2));
        mvwprintw(win, 0, i * (scr_size.max_x / ARRAY_SIZE(TOOLBAR_NAMES_AND_KEYS)), "%s", TOOLBAR_NAMES_AND_KEYS[i].key_name);
        wattron(win, COLOR_PAIR(1));
        mvwprintw(win, 0, i * (scr_size.max_x / ARRAY_SIZE(TOOLBAR_NAMES_AND_KEYS)) + strlen(TOOLBAR_NAMES_AND_KEYS[i].key_name), " %s", TOOLBAR_NAMES_AND_KEYS[i].name);
    }
    wrefresh(win);
    delwin(win);
}
void render_main_window() {
    static buf_t buf;
    static WINDOW* main_pad = NULL;
    size_t i = 0;
    WINDOW* box_win = newwin(scr_size.max_y - 4, scr_size.max_x, 3, 0);
    wbkgd(main_pad, COLOR_PAIR(3));
    wbkgd(box_win, COLOR_PAIR(3));
    box(box_win, 0, 0);
    wrefresh(box_win);
    if (fpipe == NULL) {

        fpipe = get_query_result_file(last_request);
        total_lines = 0;
        if (main_pad != NULL)
            delwin(main_pad);
        main_pad = newpad((32 * PATH_MAX) / scr_size.max_x, scr_size.max_x);
        wbkgd(main_pad, COLOR_PAIR(3));

    }
    while (fpipe != NULL && i < scr_size.max_y - 6 && fgets(buf, sizeof buf, fpipe)) {
        i++;
        mvwaddstr(main_pad, total_lines, 0, buf);
        total_lines++;
    }
    wmove(main_pad, current_pos, 0);
    winstr(main_pad, buf);
    wattron(main_pad, COLOR_PAIR(1));
    mvwaddstr(main_pad, current_pos, 0, buf);
    wattroff(main_pad, COLOR_PAIR(1));
    set_field_buffer(field[0], 0, buf);
    wmove(main_pad, current_pos + 1, 0);
    winstr(main_pad, buf);
    mvwaddstr(main_pad, current_pos + 1, 0, buf);
    //box(main_pad, 0, 0);
    prefresh(main_pad, current_pos, 0, 4, 1, scr_size.max_y - 3, scr_size.max_x - 2);
    delwin(box_win);
}




