#include "../include/tar.h"
#include "../include/libs.h"
#include "../include/constant_tmp_file.h"
#include "../include/additional.h"
#include "../include/errors.h"


int packFile(const char *source_file_name, const char *new_file_name, int archive_descriptor, int depth, int header_descriptor, int *total_archived_count) {

    const char *func_name = "packFile"; // Для логов ошибок

    // Вывод информационного сообщения в консоль об архивируемом файле.
    // При выводе игнорируются временные файлы.
    if (strcmp(source_file_name, TMP_FILE_NAME) != 0){ // если имена различны

        // Отступ в консоли соответствует глубине подкаталога:
        for(int i=0;i<depth;i++){ 
            printf("\t");
        }

        // Вывод имени файла и размеров:
        PrintDirSize(source_file_name);

    }
    
    // Проверка, что архив готов к использованию:
    if (archive_descriptor == -1) {
        raiseErr(func_name, "Файл архива не был вовремя создан");
        return -1;
    }

    // Открытие источника данных:
    int descriptorSourceFile = open(source_file_name, O_RDONLY);
    if (descriptorSourceFile == -1) {
        raiseErrno(func_name, "Не удалось открыть источник данных", source_file_name);
        return -1;
    }

    // При архивации файл может быть наделен новым именем
    // Если новое имя указано (непустое), записываем его, иначе оставляем имя источника
    if (strcmp("", new_file_name)==0) {

        // Запись имени источника в архив:
        if (write(archive_descriptor, source_file_name, MAX_PATH_LENGTH) == -1) {
            raiseErrnoBypassPath(func_name, "Не удалось записать имя источника в архив");
            return -1;
        }

    } else {

        // Запись нового имени в архив (актуально для папок, которые переписываются
        // из временного в файла с другим именем):
        if (write(archive_descriptor, new_file_name, MAX_PATH_LENGTH) == -1) {
            raiseErrnoBypassPath(func_name, "Не удалось записать новое имя файла в архив");
            return -1;
        }
    }

    // Получение размера файла исходных данных:
    struct stat statRecord;
    if (stat(source_file_name, &statRecord) == -1) {
        raiseErrno(func_name, "Не удалось считать размер исходного файла", source_file_name);
        return -1;
    }  
    int file_size = statRecord.st_size;
    
    // Запись в архив также глубины файла и размера:
    if(write(archive_descriptor, &depth, sizeof(int)) == -1){
        raiseErrnoBypassPath(func_name, "Не удалось записать в архив глубину файла");
        return -1;
    }    
    
    // Запись в архив размеров файла:
    if(write(archive_descriptor, &file_size, sizeof(int)) == -1){
        raiseErrnoBypassPath(func_name, "Не удалось записать в архив размер файла");
        return -1;
    }

    // Архивация содержимого файла:
    int file_size_copy = file_size; // Оставим копию размера
    char tmp_char;
    while (file_size) {
        // Чтение байта из источника
        if(read(descriptorSourceFile, &tmp_char, sizeof(char)) == -1){
            raiseErrnoBypassPath(func_name, "Ошибка при архивации содержимого файла (чтении)");
            return -1;
        }
        // Запись байта в архив
        if(write(archive_descriptor, &tmp_char, sizeof(char)) == -1){
            raiseErrnoBypassPath(func_name, "Ошибка при архивации содержимого файла (записи)");
            return -1;
        }
        --file_size;
    }

    // Запись во временный файл, в котором формируется заголовок итогового архива,
    // информации о данном архивируемом файле, включающей его размер и имя.
    //
    // При записи в заголовок игнорируются временные файлы.
    //
    // Также в этом блоке кода увеличивается общий счетчик заархивированных файлов.
    if (strcmp(source_file_name, TMP_FILE_NAME)!=0){

        *total_archived_count += 1; // Общий счетчик заархивированных файлов

        // Запись во временный файл, в котором формируется заголовок основного архива
        //
        // 1) Запись размера:
        char str_dir_size[NUMS_LEN]; 
        sprintf(str_dir_size, "%d", file_size_copy);
        if(write(header_descriptor, str_dir_size, NUMS_LEN) == -1){
            raiseErrnoBypassPath(func_name, "Не удалось записать в заголовок размер файла");
            return -1;
        }
        // 2) Запись имени:
        char *clean_folder_name = malloc(MAX_PATH_LENGTH); // Фиксированное число байт для имени
        strncpy(clean_folder_name, source_file_name, strlen(source_file_name)); 
        if(write(header_descriptor, clean_folder_name, MAX_PATH_LENGTH) == -1){
            raiseErrnoBypassPath(func_name, "Не удалось записать в заголовок имя файла");
            return -1;
        }
        free(clean_folder_name);
        // 3) Запись отступа:
        if(write(header_descriptor, "\n", 1) == -1){
            raiseErrnoBypassPath(func_name, "Не удалось сделать отступ при записи в заголовок");
            return -1;
        }

    } // if Запись имен

    if(close(descriptorSourceFile) == -1){
        raiseErrnoBypassPath(func_name, "Не удалось закрыть файл");
        return -1;
    }

    return 0;
}

// PackFolder() архивирует папку по пути folder_path в архив по пути tar_path.
// Для рекурсии передается также глубина папки в иерархии подкаталогов - depth.
// header_descriptor нужен для записи во временный файл заголовка данных о данной папке (имени и размера).
// count ведет подсчет всех заархивированных файлов.
int packFolder(const char *folder_path, const char *tar_path, int depth, int header_descriptor, int *total_archived_count){

    const char *func_name = "PackFolder"; // Для логов ошибок

    // Вывод информационного сообщения в консоль об архивируемой папке.
    for(int i=0;i<depth-1;i++){ // Вывод отступа
            printf("\t");
    }
    printf("%s%s\n", " → Вход в папку ", folder_path);  

    // Создание файла-архива:
    int archive_descriptor = open(tar_path, APPEND_MODE, ALLOW);
    if (archive_descriptor == -1) {
        raiseErrno(func_name, "Не удалось создать архив", tar_path);
        return -1;
    }

    // Целевая папка, которую надо заархивировать
    // 
    // 1) Открытие целевой папки:
    DIR *target_folder = opendir(folder_path);
    if (target_folder == NULL){
        raiseErrno(func_name, "Не удалось создать исходную папку", folder_path);
        return -1;
    }
    // 2) Перемещение в целевую папку:
    if (chdir(folder_path) == -1) {
        raiseErrno(func_name, "Не удалось переместиться в исходную папку", folder_path);
        return -1;
    }

    // Архивация
    struct stat file_info;  // Структура с информацией о файле
    struct dirent *entry = readdir(target_folder); // Структура записей каталога
    while (entry != NULL) {
        
        // Просмотр записей каталога кроме ".", "..":
        if (strcmp(entry->d_name, ".") != 0 &&
             strcmp(entry->d_name, "..") != 0) {

            // Получение информации о файле, соответствующем записи в каталоге:
            if (stat(entry->d_name, &file_info) == -1) {
                raiseErrno(func_name, "Не удалось получить информацию о файле", entry->d_name);
                return -1;
            }

            // Архивация записей
            if (S_ISDIR(file_info.st_mode)) { // Для подкаталогов

                // Создаем из подкаталога архив:
                packFolder(entry->d_name, TMP_FILE_NAME, depth + 1, header_descriptor, total_archived_count);

                // Добавляем новосозданный архив подкаталога в общий архив:
                packFile(TMP_FILE_NAME, entry->d_name, archive_descriptor, depth + 1, header_descriptor, total_archived_count);

                // Удаляем временные файлы:
                remove(TMP_FILE_NAME);
                
            } else { // Для всех файлов кроме каталогов

                // Добавляем файл в архив:
                packFile(entry->d_name, "", archive_descriptor, depth, header_descriptor, total_archived_count);
                
            }

        }

        // Переход к следующему файлу:
        entry = readdir(target_folder);
    }

    // Возвращение в исходную папку:
    if(chdir("..") == -1){
        raiseErrno(func_name, "Не удалось вернуться в исходную папку", "..");
        return -1;
    }

    // Закрытие целевой папки:
    if(closedir(target_folder) == -1){
        raiseErrnoBypassPath(func_name, "Не удалось закрыть целевую папку");
        return -1;
    }

    // Закрытие архива:
    if(close(archive_descriptor) == -1){
        raiseErrnoBypassPath(func_name, "Не удалось закрыть созданный архив");
        return -1;
    }

    *total_archived_count += 1; // Общий счетчик заархивированных файлов

    // Отступ в консоли соответствует глубине подкаталога:
    for(int i=0;i<depth-1;i++){
        printf("\t");
    }

    // Вывод имени папки и размеров:
    int int_dir_size = PrintDirSize(folder_path);
    if (int_dir_size == -1){
        raiseErr(func_name, "Не удалось получить и выывести размер файла");
        return -1;
    }

    // Запись во временный файл, в котором формируется заголовок основного архива
    //
    // 1) Запись размера:
    char str_dir_size[NUMS_LEN]; // Фиксированное число байт для размера
    sprintf(str_dir_size, "%d", int_dir_size);  
    if(write(header_descriptor, str_dir_size, NUMS_LEN) == -1){
        raiseErrnoBypassPath(func_name, "Не удалось записать в заголовок размер файла");
        return -1;
    }
    // 2) Запись имени:
    char *clean_folder_name = malloc(MAX_PATH_LENGTH); // Фиксированное число байт для имени
    strncpy(clean_folder_name, folder_path, strlen(folder_path)); 
    if(write(header_descriptor, clean_folder_name, MAX_PATH_LENGTH) == -1){
        raiseErrnoBypassPath(func_name, "Не удалось записать в заголовок имя файла");
        return -1;
    }
    free(clean_folder_name);
    // 3) Запись отступа:
    if(write(header_descriptor, "\n", 1) == -1){
        raiseErrnoBypassPath(func_name, "Не удалось сделать отступ при записи в заголовок");
        return -1;
    }
   
    return 0;
}