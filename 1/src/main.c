#include "../include/tar.h"
#include "../include/libs.h"
#include "../include/errors.h"

#define GOAL_LENGTH 1 // Длина строки, обозначающей цель программы, без учета '\0'

// Основная программа:
int main(int argc, char* argv[]){

    const char *func_name = "main";

    // target_path хранит путь к целевому файлу:
    // - в случае задачи архивация, целевой файл - это папка;
    // - в случае задачи разархивация, целевой файл - это архив.
    char target_path[MAX_PATH_LENGTH] = "";  

    // goal служит для выбора пользователем задачи:
    // "0" - пользователь не выбрал задачу;
    // "1" - задача - архивация;
    // "2" - задача - разархивация. 
    char goal[2] = "0"; 

    // tar_path хранит путь к папке, в которую сохраняются архивы. 
    // 
    // Инициализиаруется значением по умолчанию, может быть
    // переопределено пользователем при запуске программы.
    char tar_path[MAX_PATH_LENGTH] = "./";
    
    // untar_path хранит путь к папке, в которую архивы разархивируются.
    // 
    // Инициализиаруется значением по умолчанию, может быть
    // переопределено пользователем при запуске программы.
    char untar_path[MAX_PATH_LENGTH] = "./";

    int opt; // Очередной считываемый аргумент командной строки
    while ((opt = getopt(argc, argv, ":g:w:t:u:")) != -1) { // Перебор аргументов и их значений 

        // Запись в переменную, соответствующую аргументу, или вывод ошибки:
        switch(opt) {
        case 'g': // Задача программы

            // Проверка на переполнение:
            if(strlen(optarg) > GOAL_LENGTH){

                // Вывод ошибки:
                raiseErrAndSource(func_name, "Введена слишком длинная задача", optarg);
                return -1;

            }

            // Запись:
            snprintf(goal, sizeof(goal), "%s", optarg); 

            break;

        case 'w': // Путь к целевому файлу

            // Проверка на переполнение:
            if(strlen(optarg) > MAX_PATH_LENGTH){

                // Вывод ошибки:
                raiseErrAndSource(func_name, "Слишком длинный путь к целевому файлу", optarg);
                return -1;

            }

            // Запись:
            snprintf(target_path, sizeof(target_path), "%s", optarg);

            break;         

        case 't': // Путь к папке, в которой создаются архивы

            // Проверка на переполнение:
            if(strlen(optarg) > MAX_PATH_LENGTH){

                // Вывод ошибки:
                raiseErrAndSource(func_name, "Слишком длинный путь к папке для архивов", optarg);
                return -1;

            }

            // Запись:
            snprintf(tar_path, sizeof(tar_path), "%s", optarg);

            break;

        case 'u': // Путь к папке, куда идет разархивация

            // Проверка на переполнение:
            if(strlen(optarg) > MAX_PATH_LENGTH){

                // Вывод ошибки:
                raiseErrAndSource(func_name, "Слишком длинный путь к папке для разархивации", optarg);
                return -1;

            }

            // Запись:
            snprintf(untar_path, sizeof(untar_path), "%s", optarg);
            
            break;

        case ':': // Аргумент без значения

            raiseErrAndSource(func_name, "Не хватает значения для аргумента", argv[optind-1]);
            return -1;

        case '?': // Неизвестный аргумент

            raiseErrAndSource(func_name, "Введен неизвестный аргумент", argv[optind-1]);
            return -1;

        } // switch. Запись в переменную, соответствующую консольному аргументу, или вывод ошибки.

    } // while. Перебор консольных аргументов и их значений. 

    // Вывод заданных параметров:
    printf("\n%s\n", "\t- Параметры -\n");
    printf("%s%s\n", "Целевой путь: ", target_path);
    printf("%s%s\n", "Задача программы: ", goal);
    printf("%s%s\n", "Путь архивации: ", tar_path);
    printf("%s%s\n", "Путь разархивации: ", untar_path);

    // Запуск основной логики в соотвествии с целью - архивация или разархивация:
    switch(goal[0]){
        case '1': // Архивация

            printf("\n%s\n", "\t- Архивация -\n");

            return tar(target_path, tar_path);

        case '2': // Разархивация

            printf("\n%s\n", "\t- Разархивация -\n");

            return untar(target_path, untar_path);

        default:

            raiseErrAndSource(func_name, "Задача программы должна быть 1 или 2, но имеет значение", goal);
            return -1;

    } // switch. Запуск основной логики в соответствии с целью.

} // main