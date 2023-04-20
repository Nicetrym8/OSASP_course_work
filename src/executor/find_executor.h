#ifndef FIND_EXECUTOR_H
#define FIND_EXECUTOR_H
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "../utility/utility.h"
#include "../config.h"
#include <ctype.h>
#include <errno.h>



static const char* const flag_str_name[] = {
    "-name",
    "-group",
    "-path",
    "-perm",
    "-regex",
    "-size",
    "-user",
    "-type",
};
typedef enum PARAMETR {
    NO_PARAM,
    NAME,
    GROUP,
    PATH,
    PERM,
    REGEX,
    SIZE,
    USER,
    UID,
    QUERY_FORMAT,
    PARAMETERS_END,
} PARAMETR;
typedef enum CHECKBOXES {
    TYPE,
    CHECKBOXES_END,
} CHECKBOXES;

static const char* const tokens[] = {
    "{name}",
    "{group}",
    "{path}",
    "{perm}",
    "{regex}",
    "{size}",
    "{user}",
    "{uid}",
};
#define get_index_by_param(__X) __X-1
#define get_str_opt_by_param(__X) flag_str_name[get_index_by_param(__X)]
#define PARAM_QUANTITY PARAMETERS_END-1
#define CHECKBOXES_QUANTITY CHECKBOXES_END - 1

typedef struct parametr {
    PARAMETR flag;
    char* expr;
}parametr;
typedef struct control_struct {
    parametr options[PARAM_QUANTITY];
}control_struct;

void write_settings();
void read_settings();

/**
 * @brief Установить выбранный флаг параметров
 *
 * @param flag флаг для установки
 * @param expr аргумент параметра
 */
void set_flag(PARAMETR flag, char* restrict expr);
/**
 * @brief Сбросить флаг параметра
 *
 * @param flag флаг для сброса
 */
void clear_flag(PARAMETR flag);
/**
 * @brief Создать строку вызова find
 *
 * @param buf: выходной буфер
 * @param path: путь для find
 */
void create_exec_str(char* restrict buf, char* restrict path, char* restrict query);
/**
 * @brief Получить результирующий файл
 * @param path: путь для find
 * @return FILE* с результатом выполнения find
 * @note Закрытие файла на пользователе.
 *
 */
FILE* get_query_result_file(char* restrict path);
#endif