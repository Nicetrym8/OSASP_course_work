#include "find_executor.h"

control_struct control_word;

void set_flag(PARAMETR flag, char* restrict expr) {
    control_word.options[get_index_by_param(flag)] = (parametr){ flag,expr };
}
void clear_flag(PARAMETR flag) {
    control_word.options[get_index_by_param(flag)] = (parametr){ NO_PARAM,NULL };
}
void create_exec_str(char* restrict buf, char* restrict path) {
    strcpy(buf, "find");
    if (path != NULL) {
        strcat(buf, " ");
        strcat(buf, path);
    }
    for (int i = 0; i < PARAM_QUANTITY;i++) {
        if (control_word.options[i].flag != NO_PARAM) {
            strcat(buf, " ");
            strcat(buf, get_str_opt_by_param(control_word.options[i].flag));
            if (control_word.options[i].expr != NULL) {
                strcat(buf, " ");
                strcat(buf, control_word.options[i].expr);
            }
        }
    }
    strcat(buf, " 2>/dev/null");
}
FILE* get_query_result_file(char* restrict path) {
    static char command_buffer[PATH_MAX];
    FILE* fpipe;
    create_exec_str(command_buffer, path);

    if (0 == (fpipe = (FILE*)popen(command_buffer, "r")))
    {
        perror("popen() failed.");
        exit(EXIT_FAILURE);
    }

    return fpipe;
}