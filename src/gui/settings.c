#include "settings.h"


extern int ch;
extern screen_size scr_size;
extern bool exit_flag;
extern char preferences[PARAM_QUANTITY][PATH_MAX];
extern bool checkbox[CHECKBOXES_QUANTITY];

static WINDOW *settings_form_win = NULL;
static FORM *settings_form = NULL;
static size_t current_setting = 0;
static screen_size prev_size;

static void cleanup();
static void on_settings_resize_handler();
static void on_settings_exit_handler();
static void on_settings_next_field();
static void settings_refresher_handler();
static void render_settings();
static void on_settings_prev_field();
static void on_settings_del_char_handler();
static void on_about_handler();
static void on_checkbox_handler();
static void update_field_buffer();
static void on_settings_scroll_right();
static void on_settings_scroll_left();
typedef struct buffer_settings {
    const PARAMETR flag;
    const char *ui_name;
    char *field_buffer;
}buffer_settings;
typedef struct buffer_checkbox {
    const CHECKBOXES flag;
    const char *ui_name;
    bool *checked;
}buffer_checkbox;
static buffer_checkbox checkboxes[] = {
    {TYPE_F,FILE_TYPE_GUI,&checkbox[TYPE_F]},
    {TYPE_D,DIR_TYPE_GUI,&checkbox[TYPE_D]},
    {TYPE_L,SYMLINK_TYPE_GUI,&checkbox[TYPE_L]},
};
static buffer_settings fields_buffer[] = {
   {NAME,NAME_GUI,preferences[get_index_by_param(NAME)]},
   {GROUP,GROUP_GUI,preferences[get_index_by_param(GROUP)]},
   {USER,USER_GUI,preferences[get_index_by_param(USER)]},
   {REGEX,REGEXP_GUI,preferences[get_index_by_param(REGEX)]},
   {PERM,PERM_GUI,preferences[get_index_by_param(PERM)]},
   {SIZE,SIZE_GUI,preferences[get_index_by_param(SIZE)]},
   {QUERY_FORMAT,QUERY_STRING_GUI,preferences[get_index_by_param(QUERY_FORMAT)]},
};
static FIELD *settings_field[ARRAY_SIZE(fields_buffer) + 1];
static const key_handler SETTINGS_CONTROL_KEYS_HANDLERS[] = {
    {KEY_RESIZE, on_settings_resize_handler},
    {KEY_F(3), on_settings_exit_handler},
    {KEY_F(2),on_about_handler},
    {KEY_DOWN,on_settings_next_field},
    {KEY_LEFT,on_settings_scroll_left},
    {KEY_RIGHT,on_settings_scroll_right},
    {KEY_UP,on_settings_prev_field},
    {KEY_BACKSPACE,on_settings_del_char_handler},
    {ENTER_KEY,on_checkbox_handler},
};
static const render_routes SETTINGS_RENDER_LIST[] = {
    render_key_map,
    //render_settings_window,
    render_settings,
};

void settings() {
    exit_flag = FALSE;
    settings_refresher_handler();
    while ((ch = wgetch(settings_form_win))) {
        default_key_handler(SETTINGS_CONTROL_KEYS_HANDLERS, ARRAY_SIZE(SETTINGS_CONTROL_KEYS_HANDLERS));
        if (ch != -1) {
            form_driver(settings_form, ch);
            form_driver(settings_form, REQ_VALIDATION);
            update_field_buffer();
            curs_set(1);
            wrefresh(settings_form_win);
        }
        if (exit_flag) {
            cleanup();
            return;
        }
    }
}
static void update_field_buffer() {
    if (current_setting < ARRAY_SIZE(settings_field) - 1) {
        strcpy(fields_buffer[current_setting].field_buffer, field_buffer(settings_field[current_setting], 0));
        trimwhitespace(fields_buffer[current_setting].field_buffer);
    }

}
static void on_settings_scroll_right() {
    form_driver(settings_form, REQ_NEXT_CHAR);
    curs_set(1);
    wrefresh(settings_form_win);
}
static void on_settings_scroll_left() {
    form_driver(settings_form, REQ_PREV_CHAR);
    curs_set(1);
    wrefresh(settings_form_win);
}
static void on_settings_del_char_handler() {
    form_driver(settings_form, REQ_DEL_PREV);
    form_driver(settings_form, REQ_VALIDATION);
    update_field_buffer();
    curs_set(1);
    wrefresh(settings_form_win);
}
static void on_about_handler() {
    cleanup();
    about();
    settings_refresher_handler();
}
static void on_checkbox_handler() {
    if (current_setting > ARRAY_SIZE(settings_field) - 2) {
        *(checkboxes[current_setting - ARRAY_SIZE(settings_field) + 1].checked) = !(*(checkboxes[current_setting - ARRAY_SIZE(settings_field) + 1].checked));
        render_settings();
    }
}
static void on_settings_next_field() {
    if (current_setting < ARRAY_SIZE(settings_field) - 2 + ARRAY_SIZE(checkboxes)) {

        current_setting++;
        render_settings();
        // form_driver(settings_form, REQ_NEXT_FIELD);
         //  form_driver(settings_form, REQ_NEXT_FIELD);
         //  form_driver(settings_form, REQ_END_LINE);

    }
}
static void on_settings_prev_field() {
    if (current_setting > 0) {
        current_setting--;
        render_settings();
        // form_driver(settings_form, REQ_PREV_FIELD);
        // form_driver(settings_form, REQ_END_LINE);

    }
}
static void settings_refresher_handler() {
    getmaxyx(stdscr, scr_size.max_y, scr_size.max_x);
    for (size_t i = 0; i < ARRAY_SIZE(SETTINGS_RENDER_LIST);i++) {
        SETTINGS_RENDER_LIST[i]();
    }
}
static void render_settings() {

    static WINDOW *box_win = NULL;
    int rows, cols;


    if (box_win == NULL) {
        box_win = newwin(scr_size.max_y - 1, scr_size.max_x, 0, 0);
        wbkgd(box_win, COLOR_PAIR(3));
        wrefresh(box_win);
    }
    //cleanup();
    if (settings_form_win == NULL) {
        settings_form_win = newpad((int)(ARRAY_SIZE(settings_field) + ARRAY_SIZE(checkboxes)), scr_size.max_x - 2);
        wbkgd(settings_form_win, COLOR_PAIR(3));
        keypad(settings_form_win, TRUE);
    }
    unpost_form(settings_form);
    for (size_t i = 0;i < ARRAY_SIZE(settings_field) - 1;i++) {
        if (settings_field[i] == NULL)
            settings_field[i] = new_field(1, 3 * scr_size.max_x / 4, i, 1, 0, 0);

        if (current_setting == i) {
            set_field_back(settings_field[i], COLOR_PAIR(1) | A_UNDERLINE);
            set_field_fore(settings_field[i], COLOR_PAIR(1));
            wattron(settings_form_win, COLOR_PAIR(1));
            field_opts_on(settings_field[i], O_ACTIVE);
        }
        else {
            set_field_back(settings_field[i], COLOR_PAIR(3) | A_UNDERLINE);
            set_field_fore(settings_field[i], COLOR_PAIR(3));
            field_opts_off(settings_field[i], O_ACTIVE);
        }
        field_opts_off(settings_field[i], O_AUTOSKIP);

        set_field_buffer(settings_field[i], 0, fields_buffer[i].field_buffer);
        mvwaddstr(settings_form_win, 1 + i, 1, fields_buffer[i].ui_name);
        wattroff(settings_form_win, COLOR_PAIR(1));
    }
    for (size_t i = 0;i < ARRAY_SIZE(checkboxes);i++) {
        if (current_setting == ARRAY_SIZE(settings_field) - 1 + i)
            wattron(settings_form_win, COLOR_PAIR(1));
        mvwprintw(settings_form_win, i + ARRAY_SIZE(settings_field), 1, "%s %s", *(checkboxes[i].checked) ? "[x]" : "[ ]", checkboxes[i].ui_name);
        wattroff(settings_form_win, COLOR_PAIR(1));
    }

    settings_field[ARRAY_SIZE(settings_field) - 1] = NULL;
    if (settings_form == NULL) {
        settings_form = new_form(settings_field);
        scale_form(settings_form, &rows, &cols);
        set_form_win(settings_form, settings_form_win);
        set_form_sub(settings_form, derwin(settings_form_win, rows, cols, 1, scr_size.max_x / 4 - 3));
    }
    post_form(settings_form);
    if ((scr_size.max_y != prev_size.max_y) || (prev_size.max_x != scr_size.max_x) || (scr_size.max_y - 3 < (int)(ARRAY_SIZE(settings_field) - 1))) {
        wclear(box_win);
        wresize(box_win, scr_size.max_y - 1, scr_size.max_x);
        box(box_win, 0, 0);
        wrefresh(box_win);
    }
    prefresh(settings_form_win, (int)current_setting < scr_size.max_y - 3 ? 1 : current_setting + 1, 0, 1, 1, scr_size.max_y - 3, scr_size.max_x - 2);
    curs_set(0);
    if (current_setting < ARRAY_SIZE(settings_field) - 1) {
        set_current_field(settings_form, settings_field[current_setting]);
    }

    form_driver(settings_form, REQ_END_LINE);
    prev_size = scr_size;


}
static void cleanup() {
    if (settings_form != NULL) {
        unpost_form(settings_form);
        delwin(form_sub(settings_form));
        free_form(settings_form);
        for (size_t i = 0;i < ARRAY_SIZE(settings_field);i++) {
            free_field(settings_field[i]);
            settings_field[i] = NULL;
        }
        delwin(settings_form_win);
        settings_form_win = NULL;
        settings_form = NULL;
        prev_size = (screen_size){ 0,0 };
    }
}
static void on_settings_resize_handler() {
    screen_size temp;
    getmaxyx(settings_form_win, temp.max_y, temp.max_x);
    if (temp.max_x != scr_size.max_x) {
        cleanup();
    }
    settings_refresher_handler();
}
static void on_settings_exit_handler() {
    write_settings();
    exit_flag = TRUE;
}