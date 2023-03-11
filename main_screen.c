#include "main_screen.h"



/*****************SETTINGS********************/

static void exit_route();
static void on_del_char_handler();
static void on_resize_handler();
static void on_key_down_handler();
static void on_key_up_handler();
static void on_about_handler();
static void render_search_bar();
static void main_window_gui_init();
static void on_submit_handler();
static void refresher_handler();
static void render_main_window();
static void on_settings_handler();

static const key_handler CONTROL_KEYS_HANDLERS_LIST[] = {
   {KEY_F(3),exit_route},
   {KEY_F(2), on_about_handler},
   {KEY_F(1), on_settings_handler},
   {KEY_BACKSPACE, on_del_char_handler},
   {KEY_RESIZE,on_resize_handler},
   {ENTER_KEY,on_submit_handler},
   {KEY_DOWN,on_key_down_handler},
   {KEY_UP,on_key_up_handler},
};
static const render_routes RENDER_LIST[] = {
    render_key_map,
    render_main_window,
    render_search_bar
};
/*****************END SETTINGS******************/
extern int ch;
extern screen_size scr_size;
static FIELD* field[2];
static FORM* search_form = NULL;
static WINDOW* search_form_win = NULL;
static buf_t last_request;
static int current_pos;
static FILE* fpipe = NULL;
static int total_lines;
//

void render_main_window_gui() {
    main_window_gui_init();

    while ((ch = wgetch(search_form_win))) {

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
}

static void exit_route() {
    unpost_form(search_form);
    free_form(search_form);
    free_field(field[0]);
    endwin();
    exit(EXIT_SUCCESS);
}
static void on_del_char_handler() {
    form_driver(search_form, REQ_DEL_PREV);
    form_driver(search_form, REQ_END_LINE);
    form_driver(search_form, REQ_VALIDATION);
}
static void on_about_handler() {
    about();
    refresher_handler();
}
static void on_settings_handler() {
    settings();
    refresher_handler();
}
static void on_resize_handler() {
    strcpy(last_request, field_buffer(field[0], 0));
    refresher_handler();
}
static void on_key_down_handler() {
    if (current_pos < total_lines - 1)current_pos++;
    render_main_window();
}
static void on_key_up_handler() {
    if (current_pos > 0)current_pos--;
    render_main_window();
}

static void refresher_handler() {
    getmaxyx(stdscr, scr_size.max_y, scr_size.max_x);
    for (size_t i = 0; i < ARRAY_SIZE(RENDER_LIST);i++) {
        RENDER_LIST[i]();
    }
}
static void main_window_gui_init() {
    refresher_handler();
    curs_set(0);
}

static void on_submit_handler() {
    if (fpipe != NULL) {
        pclose(fpipe);
        fpipe = NULL;
    }
    current_pos = 0;
    strcpy(last_request, field_buffer(field[0], 0));
    refresher_handler();
}
static void render_search_bar() {
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

static void render_main_window() {
    static buf_t buf;
    static WINDOW* main_pad = NULL;
    size_t i = 0;
    WINDOW* box_win = newwin(scr_size.max_y - 4, scr_size.max_x, 3, 0);
    if (main_pad == NULL)
        main_pad = newpad(VISIBLE_MAX, scr_size.max_x);
    wbkgd(main_pad, COLOR_PAIR(3));
    wbkgd(box_win, COLOR_PAIR(3));
    box(box_win, 0, 0);
    wrefresh(box_win);

    if (fpipe == NULL) {

        fpipe = get_query_result_file(last_request);
        total_lines = 0;
        werase(main_pad);
    }
    while (fpipe != NULL && (total_lines - current_pos) < scr_size.max_y - 6 && fgets(buf, sizeof buf, fpipe) && total_lines < VISIBLE_MAX) {
        mvwaddstr(main_pad, total_lines, 0, buf);
        total_lines++;
    }
    wmove(main_pad, current_pos, 0);
    winstr(main_pad, buf);
    wattron(main_pad, COLOR_PAIR(1));
    mvwaddstr(main_pad, current_pos, 0, buf);
    wattroff(main_pad, COLOR_PAIR(1));
    //set_field_buffer(field[0], 0, buf);
    wmove(main_pad, current_pos + 1, 0);
    winstr(main_pad, buf);
    mvwaddstr(main_pad, current_pos + 1, 0, buf);
    //box(main_pad, 0, 0);
    prefresh(main_pad, current_pos, 0, 4, 1, scr_size.max_y - 3, scr_size.max_x - 2);
    delwin(box_win);
}




