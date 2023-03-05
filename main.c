#include "screen.h"
#include "config.h"
#include <ncurses.h>
#include <form.h>

//init_pair(1, COLOR_BLACK, COLOR_CYAN);
  // init_pair(2, COLOR_WHITE, COLOR_BLACK);
   //init_pair(3, COLOR_WHITE, COLOR_BLUE);



void print_in_middle(WINDOW* win, int starty, int startx, int width, char* string, chtype color);

int main()
{
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
    render_main_window_gui();
    return 0;
}

void print_in_middle(WINDOW* win, int starty, int startx, int width, char* string, chtype color)
{
    int length, x, y;
    float temp;

    if (win == NULL)
        win = stdscr;
    getyx(win, y, x);
    if (startx != 0)
        x = startx;
    if (starty != 0)
        y = starty;
    if (width == 0)
        width = 80;

    length = strlen(string);
    temp = (width - length) / 2;
    x = startx + (int)temp;
    wattron(win, color);
    mvwprintw(win, y, x, "%s", string);
    wattroff(win, color);
    refresh();
}