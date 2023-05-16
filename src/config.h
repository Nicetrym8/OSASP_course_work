#ifndef CONFIG_H
#define CONFIG_H

#define TITLE "find CURSES"
#define VERSION "v1.0"
#define DESCRIPTION "Данная программа представляет собой простую оболочку над утилитой find. Разработана Nicetrym8, как курсовая работа по дисциплине ОСиСП. Распространяется по лицензии MIT (https://mit-license.org/). Copyright (c) 2023 Nicetrym8."
//Название меток для UI
#define FILE_TYPE_GUI "Отображать файлы"
#define DIR_TYPE_GUI "Отображать каталоги"
#define SYMLINK_TYPE_GUI "Отображать символьные ссылки"
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
#define QUERY_STRING_GUI "Формат запроса"
#define EXIT_GUI "Выйти/Назад"

#define name_and_version_str(name, version) name " " version
#define PROGRAM_NAME name_and_version_str(TITLE,VERSION)

#define SETTINGS_PATH "./userdata"
#define VISIBLE_MAX 1024
#endif
