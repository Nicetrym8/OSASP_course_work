#include "main_screen.h"


/*****************SETTINGS********************/

static void init_form();
static void cleanup();
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
static void on_scroll_right();
static void on_scroll_left();


static const key_handler CONTROL_KEYS_HANDLERS_LIST[] = {
   {KEY_F(3),exit_route},
   {KEY_F(2), on_about_handler},
   {KEY_F(1), on_settings_handler},
   {KEY_BACKSPACE, on_del_char_handler},
   {KEY_RESIZE,on_resize_handler},
   {ENTER_KEY,on_submit_handler},
   {KEY_DOWN,on_key_down_handler},
   {KEY_UP,on_key_up_handler},
   {KEY_RIGHT,on_scroll_right},
   {KEY_LEFT,on_scroll_left},
};
static const render_routes RENDER_LIST[] = {
    render_search_bar,
    render_main_window,
    render_key_map,
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

extern char preferences[PARAM_QUANTITY][PATH_MAX];
//

void render_main_window_gui() {
    main_window_gui_init();

    while ((ch = wgetch(search_form_win))) {

        default_key_handler(CONTROL_KEYS_HANDLERS_LIST, ARRAY_SIZE(CONTROL_KEYS_HANDLERS_LIST));
        if (ch != -1) {
            form_driver(search_form, ch);
            form_driver(search_form, REQ_VALIDATION);
            curs_set(1);
        }
    }
}
static void cleanup() {
    unpost_form(search_form);
    delwin(form_sub(search_form));
    free_form(search_form);
    free_field(field[0]);
    field[0] = NULL;
    delwin(search_form_win);
    search_form_win = NULL;
    search_form = NULL;
}
static void exit_route() {
    cleanup();
    endwin();
    exit(EXIT_SUCCESS);
}
static void on_del_char_handler() {
    form_driver(search_form, REQ_DEL_PREV);
    form_driver(search_form, REQ_VALIDATION);
    curs_set(1);
}
static void on_scroll_right() {
    form_driver(search_form, REQ_NEXT_CHAR);
    curs_set(1);
}
static void on_scroll_left() {
    form_driver(search_form, REQ_PREV_CHAR);
    curs_set(1);
}
static void on_about_handler() {
    about();
    cleanup();
    refresher_handler();
}
static void on_settings_handler() {
    settings();
    cleanup();
    refresher_handler();
}
static void on_resize_handler() {
    static screen_size prev;
    getmaxyx(search_form_win, prev.max_y, prev.max_x);
    strcpy(last_request, field_buffer(field[0], 0));
    if (search_form != NULL && (prev.max_x != scr_size.max_x || scr_size.max_y < 6)) {
        cleanup();
    }
    refresher_handler();
    //prev = scr_size;
}
static void on_key_down_handler() {
    if (current_pos < total_lines - 1)current_pos++;
    render_main_window();
    curs_set(0);
}
static void on_key_up_handler() {
    if (current_pos > 0)current_pos--;
    render_main_window();
    curs_set(0);
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
    strcpy(last_request, field_buffer(field[0], 0));
    if (fpipe != NULL) {
        pclose(fpipe);
        fpipe = NULL;
    }
    current_pos = 0;
    refresher_handler();
}
static void init_form() {
    if (search_form == NULL) {
        int rows = 1, cols = 1;
        field[0] = new_field(1, scr_size.max_x - 2, 0, 0, 0, 0);
        field[1] = NULL;
        set_field_back(field[0], COLOR_PAIR(3));
        set_field_fore(field[0], COLOR_PAIR(3));
        field_opts_off(field[0], O_AUTOSKIP);
        search_form = new_form(field);
        scale_form(search_form, &rows, &cols);
        search_form_win = newwin(rows + 2, cols + 2, 0, 0);
        keypad(search_form_win, TRUE);
        wbkgd(search_form_win, COLOR_PAIR(3));
        set_form_win(search_form, search_form_win);
        set_form_sub(search_form, subwin(search_form_win, rows, cols, 1, 1));
        post_form(search_form);
    }
}
static void render_search_bar() {

    screen_size current_box_size;
    getmaxyx(search_form_win, current_box_size.max_y, current_box_size.max_x);
    init_form();
    if (scr_size.max_x != current_box_size.max_x) {
        wresize(search_form_win, 3, scr_size.max_x);
    }
    box(search_form_win, 0, 0);
    set_field_buffer(field[0], 0, last_request);
    form_driver(search_form, REQ_END_LINE);
    wrefresh(search_form_win);

}

static void render_main_window() {

    static buf_t buf;
    static WINDOW* main_pad = NULL;
    WINDOW* box_win = NULL;
    size_t i = 0;
    screen_size current_box_size;
    if (main_pad == NULL) {
        main_pad = newpad(VISIBLE_MAX, scr_size.max_x);
        wbkgd(main_pad, COLOR_PAIR(3));
    }
    box_win = newwin(scr_size.max_y - 4, scr_size.max_x, 3, 0);
    wbkgd(box_win, COLOR_PAIR(3));
    getmaxyx(main_pad, current_box_size.max_y, current_box_size.max_x);
    if ((scr_size.max_y != current_box_size.max_y) || (scr_size.max_x != current_box_size.max_x)) {
        // wclear(box_win);
        wresize(main_pad, VISIBLE_MAX, scr_size.max_x);
    }

    if (fpipe == NULL) {

        fpipe = get_query_result_file(trimwhitespace(field_buffer(field[0], 0)));
        wclear(main_pad);
        total_lines = 0;
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
    wmove(main_pad, current_pos + 1, 0);
    winstr(main_pad, buf);
    mvwaddstr(main_pad, current_pos + 1, 0, buf);
    box(box_win, 0, 0);
    wrefresh(box_win);
    prefresh(main_pad, current_pos, 0, 4, 1, scr_size.max_y - 3, scr_size.max_x - 2);
    delwin(box_win);

}




