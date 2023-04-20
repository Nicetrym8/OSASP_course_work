#include "./gui/main_screen.h"


int main() {
  setlocale(LC_CTYPE, "");
  initscr();
  start_color();
  use_default_colors();
  scrollok(stdscr, FALSE);
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  init_pair(1, COLOR_BLACK, COLOR_CYAN);
  init_pair(2, COLOR_WHITE, COLOR_BLACK);
  init_pair(3, COLOR_WHITE, COLOR_BLUE);
  read_settings();
  render_main_window_gui();
  return 0;
}