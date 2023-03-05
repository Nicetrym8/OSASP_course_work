#include "screen.h"



/*****************SETTINGS********************/
const toolbar TOOLBAR_NAMES_AND_KEYS[] = {
    {EXECUTE_GUI,"Enter",KEY_ENTER, },
    {ADDITIONAL_COMMAND_GUI,"F1",KEY_F(1)},
    {ABOUT_GUI,"F2", KEY_F(2)},
    {EXIT_GUI,"F3", KEY_F(3)}
};

const render_routes render_list[] = {
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
char* memstr_buf = NULL;
size_t memstr_size;
FILE* memstream = NULL;
int current_pos;
FILE* fpipe = NULL;
fpos_t memstream_offset;
void render_gui() {
    int ch;
    gui_init();
    curs_set(0);
    while ((ch = wgetch(search_form_win)) != KEY_F(1))
    {

        switch (ch)
        {
        case KEY_BACKSPACE:
            /* Go to previous field */
            form_driver(search_form, REQ_DEL_PREV);
            form_driver(search_form, REQ_END_LINE);
            form_driver(search_form, REQ_VALIDATION);
            break;
        case KEY_RESIZE:
            strcpy(last_request, field_buffer(field[0], 0));
            refresher_handler();
            break;
        case 10:
            on_resize_or_submit_handler();
            break;
        case KEY_DOWN:
            current_pos++;
            refresher_handler();
            break;
        default:
            /* If this is a normal character, it gets */
            /* Printed				  */
            form_driver(search_form, ch);

            form_driver(search_form, REQ_VALIDATION);
            break;
        }
    }

    /* Un post form and free the memory */
    unpost_form(search_form);
    fclose(memstream);
    free(memstr_buf);
    free_form(search_form);
    free_field(field[0]);
    endwin();
}
void refresher_handler() {
    getmaxyx(stdscr, scr_size.max_y, scr_size.max_x);
    for (int i = 0; i < ARRAY_SIZE(render_list);i++) {
        render_list[i]();
    }
}
void gui_init() {
    setlocale(LC_CTYPE, "");
    initscr();
    start_color();
    use_default_colors();
    scrollok(stdscr, FALSE);
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    init_pair(1, COLOR_BLACK, COLOR_CYAN);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLUE);
    refresher_handler();
}
void on_resize_or_submit_handler() {
    if (fpipe != NULL) {
        pclose(fpipe);
        fpipe = NULL;
        fclose(memstream);
        free(memstr_buf);
        memstr_size = 0;
        memstream = NULL;
    }
    current_pos = 0;
    strcpy(last_request, field_buffer(field[0], 0));
    fpipe = get_query_result_file(last_request);
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

    for (int i = 0; i < ARRAY_SIZE(TOOLBAR_NAMES_AND_KEYS);i++) {
        wattron(win, COLOR_PAIR(2));
        mvwprintw(win, 0, i * (scr_size.max_x / ARRAY_SIZE(TOOLBAR_NAMES_AND_KEYS)), "%s", TOOLBAR_NAMES_AND_KEYS[i].key_name);
        wattron(win, COLOR_PAIR(1));
        mvwprintw(win, 0, i * (scr_size.max_x / ARRAY_SIZE(TOOLBAR_NAMES_AND_KEYS)) + strlen(TOOLBAR_NAMES_AND_KEYS[i].key_name), " %s", TOOLBAR_NAMES_AND_KEYS[i].name);
    }
    wrefresh(win);
    delwin(win);
}
void print_record_to_screen(WINDOW* win, char* restrict buf, size_t* i, int* scroll) {
    /* if (*scroll > 0) {
         (*scroll)--;
         //wrefresh(win);
         return;
     } */

    if (*i == current_pos)
        wattron(win, COLOR_PAIR(1));
    mvwprintw(win, (*i) + 1, 1, "%.*s", scr_size.max_x - 2, buf);
    if (strlen(buf) > scr_size.max_x - 2) {
        (*i)++;

        mvwprintw(win, (*i) + 1, 1, "%.*s", scr_size.max_x - 2, (buf + scr_size.max_x - 2));
    }
    wrefresh(win);
    wattroff(win, COLOR_PAIR(1));
    (*i)++;
}
void render_main_window() {
    static buf_t buf;
    size_t i = 0;

    WINDOW* win = newwin(scr_size.max_y - 4, scr_size.max_x, 3, 0);
    int scroll = current_pos - (scr_size.max_y - 6);
    wbkgd(win, COLOR_PAIR(3));
    if (memstream == NULL) {
        memstream = open_memstream(&memstr_buf, &memstr_size);
    }

    rewind(memstream);
    while (fgets(buf, sizeof buf, memstream) && i < scr_size.max_y - 6)
        print_record_to_screen(win, buf, &i, &scroll);
    fseek(memstream, 0, SEEK_END);
    while (fpipe != NULL && i < scr_size.max_y - 6 && fgets(buf, sizeof buf, fpipe)) {
        fputs(buf, memstream);
        print_record_to_screen(win, buf, &i, &scroll);
    }


    box(win, 0, 0);
    wrefresh(win);
    delwin(win);
}
void form_events_handler(FORM* form, int ch) {
    switch (ch)
    {
    case KEY_DOWN:
        /* Go to next field */
        form_driver(form, REQ_NEXT_FIELD);
        /* Go to the end of the present buffer */
        /* Leaves nicely at the last character */
        form_driver(form, REQ_END_LINE);
        break;
    case KEY_UP:
        /* Go to previous field */
        form_driver(form, REQ_PREV_FIELD);
        form_driver(form, REQ_END_LINE);
        break;
    case KEY_BACKSPACE:
        /* Go to previous field */
        form_driver(form, REQ_DEL_PREV);
        form_driver(form, REQ_END_LINE);
        form_driver(form, REQ_VALIDATION);
        break;
    default:
        /* If this is a normal character, it gets */
        /* Printed				  */
        form_driver(form, ch);
        form_driver(form, REQ_VALIDATION);
        break;
    }
}



