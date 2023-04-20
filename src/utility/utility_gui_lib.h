#ifndef UTILITY_GUI_LIB_H
#define UTILITY_GUI_LIB_H
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif
#include <curses.h>
#include <string.h>
#include <linux/limits.h>
#include <ctype.h>
#include "../config.h"
#include "utility.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define ENTER_KEY 10
typedef int control_key;
typedef struct screen_size {
    int max_x;
    int max_y;
}screen_size;

typedef struct toolbar {
    const char* name;
    const char* key_name;
}toolbar;

typedef char buf_t[PATH_MAX];
typedef void (*render_routes)();
typedef render_routes event_handler;
typedef struct key_handler {
    control_key key;
    event_handler handler;
}key_handler;

void default_key_handler(const key_handler* restrict control_key_handlers, size_t size);
char* trimwhitespace(char* str);
void render_key_map();
#endif