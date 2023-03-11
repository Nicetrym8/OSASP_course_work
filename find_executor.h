#ifndef FIND_EXECUTOR_H_
#define FIND_EXECUTOR_H_
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static const char* const flag_str_name[] = {
    "-P",
    "-L",
    "-H",
    "-name",
    "-group",
    "-path",
    "-perm",
    "-readable",
    "-regex",
    "-size",
    "-type",
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
void create_exec_str(char* restrict buf, char* restrict path);
/**
 * @brief Получить результирующий файл
 * @param path: путь для find
 * @return FILE* с результатом выполнения find
 * @note Закрытие файла на пользователе см. mclose()
 *
 */
FILE* get_query_result_file(char* restrict path);
#endif