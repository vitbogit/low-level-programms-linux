#include "../include/libs.h"
#include "../include/errors.h"

int RunProcessAndSaveResult(char* output, char* command){

    const char *func_name = "RunProcessAndSaveResult"; // Для логов ошибок

	FILE *result_file = NULL; // Сюда запишется результат выполнения команды

    result_file = popen(command, "r"); // Получение результата выполнения команды
    if(result_file == NULL){
        raiseErr(func_name, "Не удалось выполнить popen() для команды");
        return -1;
    }

    fgets(output, MAX_PATH_LENGTH, result_file); // Передача результата в output

    return 0;
}

void removeFileExtensionFromString(const char* myStr) {
    char * tmp = malloc(MAX_PATH_LENGTH);
    sprintf(tmp, "%s", "");
    char * pos;
    pos = strrchr(myStr, '.');
    size_t length;
    length = pos-myStr;
    strncat(tmp, myStr, length);
    strcpy((char *)myStr, tmp);
    free(tmp);
}

int GetDirSize(char *output, char *dir_name){

    const char *func_name = "GetDirSize"; // Для логов ошибок

    char *dir_size = malloc(MAX_PATH_LENGTH); // Размер директории

    // Выполним команду для получения размера директории:
    char *command = malloc(MAX_PATH_LENGTH); 
    sprintf(command, "du -bs '%s' | cut -f1", dir_name);
    if(RunProcessAndSaveResult(dir_size, command) == -1){
        raiseErr(func_name, "Не удалось выполнить команду и получить размер файла");
        return -1;
    }
    free(command);

    // Передаем в output полученный размер:
    strcpy(output, dir_size);    
    if (strlen(output)>1){  // Удаляем лишний символ переноса строки
        output[strlen(output)-1] = '\0';
    }

    // Функция будет возвращать int значение размера:
    int int_dir_size = atoi(dir_size);

    free(dir_size);
    
    return int_dir_size;
}

int PrintDirSize(const char *dir){

    const char *func_name = "PrintDirSize"; // Для логов ошибок

    // Получение размера файла:
    char *str_dir_size = malloc(MAX_PATH_LENGTH);
    int int_dir_size = GetDirSize(str_dir_size, (char *)dir);  
    if (int_dir_size == -1){
        raiseErr(func_name, "Не удалось получить размер файла");
        return -1;
    }	  

    // Вывод размера файла:
    printf("•Пакуем %s (%s байт)\n", dir, str_dir_size);
    
    free(str_dir_size);

    return int_dir_size;
}