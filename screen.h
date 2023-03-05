#ifndef SCREEN_H_
#define SCREEN_H_
#define _DEFAULT_SOURCE
#include <limits.h>
#include <ncurses.h>
#include <form.h>
#include <string.h>
#include <locale.h>
#include "config.h"
#include "find_executor.h"


#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
typedef struct screen_size {
    int max_x;
    int max_y;
}screen_size;

typedef struct toolbar {
    const char* name;
    const char* key_name;
    int key;
}toolbar;

typedef char buf_t[PATH_MAX];
typedef void (*render_routes)();

void render_key_map();
void render_main_window();
void render_search_bar();
void gui_init();
void form_events_handler(FORM* form, int ch);
void render_gui();
void on_resize_or_submit_handler();
void refresher_handler();
#endif