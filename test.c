
#define _DEFAULT_SOURCE
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* flag_str_name[] = {
    "-P",
    "-L",
    "-H",
    "-name",
    "-group",
    "-gid",
    "-lname",
    "-path",
    "-perm",
    "-readable",
    "-regex",
    "-size",
    "-type",
    "-uid",
    "-user",
    "-exec",
    "-execdir",
};
typedef enum PARAMETR {
    NO_PARAM,
    NEVER_FOLLOW_SYMLINK,
    FOLLOW_SYMLINK,
    FOLLOW_SYMLINK_ARGS,
    NAME,
    GROUP,
    GID,
    LNAME,
    PATH,
    PERM,
    READABLE,
    REGEX,
    SIZE,
    TYPE,
    UID,
    USER,
    EXEC,
    EXECDIR,
    END,
} PARAMETR;

#define get_index_by_param(__X) __X-1
#define get_str_opt_by_param(__X) flag_str_name[get_index_by_param(__X)]
#define PARAM_QUANTITY END-1

typedef struct parametr {
    PARAMETR flag;
    char* expr;
}parametr;
typedef struct control_struct {
    parametr options[PARAM_QUANTITY];
}control_struct;

control_struct control_word;

void set_flag(PARAMETR flag, char* restrict expr) {
    control_word.options[get_index_by_param(flag)] = (parametr){ flag,expr };
}
void clear_flag(PARAMETR flag) {
    control_word.options[get_index_by_param(flag)] = (parametr){ NO_PARAM,NULL };
}
void create_exec_str(char* restrict buf) {
    strcpy(buf, "find");
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
}
FILE* get_query_result_file() {
    static char command_buffer[PATH_MAX];
    FILE* fpipe;
    create_exec_str(command_buffer);
    if (0 == (fpipe = (FILE*)popen(command_buffer, "r")))
    {
        perror("popen() failed.");
        exit(EXIT_FAILURE);
    }
    return fpipe;
}
int main()
{
    FILE* fpipe;
    char command_buffer[PATH_MAX];
    char c = 0;
    set_flag(TYPE, "d,f");
    set_flag(FOLLOW_SYMLINK, NULL);
    create_exec_str(command_buffer);
    printf("%s\n", command_buffer);
    if (0 == (fpipe = (FILE*)popen(command_buffer, "r")))
    {
        perror("popen() failed.");
        exit(EXIT_FAILURE);
    }

    while (fread(&c, sizeof c, 1, fpipe))
    {
        printf("%c", c);
    }

    pclose(fpipe);

    return EXIT_SUCCESS;
}