#include "find_executor.h"

control_struct control_word;

char preferences[PARAM_QUANTITY][PATH_MAX];
int checkboxes[CHECKBOXES_QUANTITY];

void read_settings() {
    FILE* settings = fopen(SETTINGS_PATH, "rb");
    if (settings == NULL)
        return;
    for (size_t i = 0;i < ARRAY_SIZE(preferences);i++) {
        fread(preferences[i], sizeof(char), PATH_MAX, settings);
    }
}
void write_settings() {
    FILE* settings = fopen(SETTINGS_PATH, "wb");
    if (settings == NULL) {
        perror("Cannot open or create settings file");
        exit(errno);
    }
    for (size_t i = 0;i < ARRAY_SIZE(preferences);i++) {
        fwrite(preferences[i], sizeof(char), PATH_MAX, settings);
    }
}

void set_flag(PARAMETR flag, char* restrict expr) {
    control_word.options[get_index_by_param(flag)] = (parametr){ flag,expr };
}
void clear_flag(PARAMETR flag) {
    control_word.options[get_index_by_param(flag)] = (parametr){ NO_PARAM,NULL };
}
void create_exec_str(char* restrict buf, char* restrict path, char* restrict query) {
    char parser_buf[PATH_MAX] = "";
    static char parser_query[PATH_MAX];
    FILE* fpipe;
    strcpy(buf, "find");
    strcpy(parser_query, "./parser ");
    if (path != NULL) {
        strcat(buf, " ");
        strcat(buf, path);
    }
    if (query != NULL) {
        strcat(buf, " ");
        strcat(parser_query, query);
        if (0 == (fpipe = (FILE*)popen(parser_query, "r")))
        {
            perror("popen() failed");
            exit(EXIT_FAILURE);
        }
        fgets(parser_buf, sizeof parser_buf, fpipe);
        strcat(buf, parser_buf);
        pclose(fpipe);
    }
    strcat(buf, " 2>/dev/null");

}
FILE* get_query_result_file(char* restrict path) {
    static char command_buffer[PATH_MAX] = "";
    static char query[PATH_MAX] = "";
    static char buffer[256] = "";
    size_t current_pos = 0;
    size_t prev_pos = 0;
    FILE* fpipe;
    FILE* test = fopen("./test.txt", "w");
    strcpy(query, "'");
    while (1) {
        if (preferences[get_index_by_param(QUERY_FORMAT)][current_pos] == '&' || preferences[get_index_by_param(QUERY_FORMAT)][current_pos] == '|' || preferences[get_index_by_param(QUERY_FORMAT)][current_pos] == '\0') {
            strncpy(buffer, preferences[get_index_by_param(QUERY_FORMAT)] + prev_pos, current_pos - prev_pos);
            strcat(query, buffer);
            strcat(query, " ");
            for (size_t i = 0;i < ARRAY_SIZE(tokens);i++) {
                if (strstr(buffer, tokens[i])) {
                    strcat(query, preferences[i]);
                    strcat(query, " ");
                    break;
                }
            }
            if (preferences[get_index_by_param(QUERY_FORMAT)][current_pos] == '\0')break;
            prev_pos = current_pos;
        }
        current_pos++;
    }
    strcat(query, "'");
    fputs(query, test);


    create_exec_str(command_buffer, path, query);
    fputs("\n", test);
    fputs(command_buffer, test);
    fclose(test);
    if (0 == (fpipe = (FILE*)popen(command_buffer, "r")))
    {
        perror("popen() failed");
        exit(EXIT_FAILURE);
    }

    return fpipe;
}