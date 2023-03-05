#ifndef CONFIG_H_
#define CONFIG_H_


#define TITLE "find GUI "
#define VERSION "v0.1"
//Название меток для UI
#define FILE_TYPE_PARAMS_GUI "Тип"
#define EXECUTE_GUI "Выполнить"
#define ADDITIONAL_COMMAND_GUI "Параметры"
#define ABOUT_GUI "О программе"
#define SEARCH_GUI "Поиск"
#define REGEXP_GUI "Регулярное выражение"
#define NAME_GUI "Формат имени"
#define GROUP_GUI "Группа"
#define PERM_GUI "Доступ"
#define SIZE_GUI "Размер файла"
#define USER_GUI "Пользователь"
#define SYMLINK_GUI "Символьные ссылки"
#define FOLLOW_GUI "Следовать"
#define NOT_FOLLOW_GUI "Не Следовать"
#define EXIT_GUI "Выйти"

#define name_and_version_str(name, version) name version
#define PROGRAM_NAME name_and_version_str(TITLE,VERSION)

#define EXIT_KEY KEY_F(10)
#endif
