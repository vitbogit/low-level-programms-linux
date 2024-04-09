#include "../include/tar.h"

#include "../include/libs.h"
#include "../include/errors.h"
#include "../include/additional.h"

#include "../include/constant_tmp_file.h"
#include "../include/constant_tmp_header.h"
#include "../include/constant_extension.h"

// tar() - функция для архивации.
// - target_path - это путь к папке, которую надо заархивировать;
// - tar_path - это путь к папке, в которую следует сохранить архив.
// Функция возвращает 0 в случае успешной архивации, 1 при ошибках.
int tar(const char *target_path, const char *tar_path){

    const char *func_name = "tar";

    // Путь к текущей папке программы.
    // 
    // Программа будет перемещаться по папкам, поэтому необходимо
    // сохранить его значение.
    //
    // Сохраняем текущую папку:
    char programm_path[MAX_PATH_LENGTH]; 
    if(getcwd(programm_path, MAX_PATH_LENGTH) == NULL){
        raiseErrnoBypassPath(func_name, "Не удалось получить путь к папке программы");
        return -1;
    }

    // Временный файл для формирования заголовка архива.
    //
    // Заголовок архива будем последовательно формировать в отдельном временном
    // файле по ходу процесса архивации. В самом конце мы добавим этот заголовок
    // в начало архива.
    // 
    // 1) Формируем путь к временному файлу для заголовка:   
    char header_path[MAX_PATH_LENGTH]; // Путь к файлу заголовка
    strcpy(header_path, TMP_HEADER_FILE_NAME);
    // 2) Создаем временный файл для заголовка:
    int header_descriptor = open(header_path, NEW_FILE_MODE, ALLOW); 
    if (header_descriptor == -1) {
        raiseErrno(func_name, "Не удалось создать временный файл заголовка", header_path);
        return -1;
    }
    
    // Временный файл для архивации подкаталогов
    //
    // Подкатологи исходной папки будем архивировать перед добавлением в общий архив, 
    // для этого будем использовать временный файл.
    //
    // Формируем путь к временному файлу архивации:   
    char tmp_file_path[MAX_PATH_LENGTH]; // Путь к временному файлу архивации
    strcpy(tmp_file_path, TMP_FILE_NAME);

    // Файл архива
    //
    // Путь к создаваемому файлу архива формируется следующим образом:
    //
    //      archive_path = (папка с архивами) + / + (имя архивируемой папки).myarchive
    // 
    // 1) Формируем путь к файлу архиву:
    char archive_path[MAX_PATH_LENGTH];
    strcpy(archive_path, tar_path);
    strcat(archive_path, "/");
    strcat(archive_path, basename((char *)target_path));
    strcat(archive_path, EXTENSION);
    // 2) Создаем файл-архива:
    int archive_descriptor = open(archive_path, NEW_FILE_MODE, ALLOW);
    if (archive_descriptor == -1) {
        raiseErrno(func_name, "Не удалось создать файл для архива", archive_path);
        return -1;
    }

    // Целевая папка, то есть папка, которую надо архивировать.
    // 
    // 1) Открытие целевой папки:
    DIR *target_folder = opendir(target_path);
    if (target_folder == NULL){
        raiseErrno(func_name, "Не удалось открыть целевую папку", target_path);
        return -1;
    }
    // 2) Перемещение в целевую папку:
    if (chdir(target_path) == -1) {
        raiseErrno(func_name, "Не удалось переместиться в целевую папку", target_path);
        return -1;
    }

    // Архивация содержимого целевой папки: 
    struct stat file_info;  // Структура с информацией о файле
    struct dirent *entry = readdir(target_folder); // Структура записей каталога
    int files_count = 0; // Число файлов в заголовке, будем подсчитывать
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

                // Создаем из подкаталога с именем entry->d_name временный архив tmp_file_path,
                // при этом указываем увеличенную глубину, 1, для возможной рекурсии, а также /
                // дескриптор временного файла заголовка и счетчик общего числа архивированных файлов:
                if (packFolder(entry->d_name, tmp_file_path, 1, header_descriptor, &files_count) == -1){
                    return -1;
                }

                // Добавляем новосозданный временный архив tmp_file_path, носящий имя архивируемой папки entry->d_name,
                // в общий архив archive_descriptor:
                if(packFile(tmp_file_path, entry->d_name, archive_descriptor, 1, header_descriptor, &files_count) == -1){
                    return -1;
                }

                // Удаляем временные файлы:
                remove(tmp_file_path);

            } else { // Для НЕ подкаталогов

                // Добавляем файл entry->d_name в архив archive_descriptor. Так как архивируемый файл не является каталогом,
                // то имя каталога игнорируется и указывается "":         
                if(packFile(entry->d_name, "", archive_descriptor, 0, header_descriptor, &files_count) == -1){
                    return -1;
                }

            }

        }

        // Переход к следующему файлу:
        entry = readdir(target_folder);
    }

    chdir(programm_path);

    printf("\nФормируем заголовок архива с %d файлами и их размерами...\n", files_count);
    
    // Переоткрываем архив для чтения
    if(close(archive_descriptor) == -1){
        raiseErrnoBypassPath(func_name, "Не удалось закрыть архив");
        return -1;
    }
    archive_descriptor = open(archive_path, O_RDONLY, ALLOW);
    if (archive_descriptor == -1) {
        raiseErrno(func_name, "Не удалось переоткрыть файл для архива (на чтение)", archive_path);
        return -1;
    }
    // Добавляем в конец временного файла заголовка все содержимое архива:
    char tmp_char = 'f';
    while (read(archive_descriptor, &tmp_char, sizeof(char)) != 0) {
       // printf("%c", tmp_char);
        // Запись в архив
        if(write(header_descriptor, &tmp_char, sizeof(char)) == -1){
            raiseErrnoBypassPath(func_name, "Ошибка при архивации содержимого файла (записи)");
            return -1;
        }
    }
    // TODO: clear archive / reopen archive for write-trunc / remove and create
    close(archive_descriptor);
    close(header_descriptor);
    remove(archive_path);
    archive_descriptor = open(archive_path, NEW_FILE_MODE, ALLOW);
    if (archive_descriptor == -1) {
        raiseErrno(func_name, "Не удалось создать файл для архива", archive_path);
        return -1;
    }
    // TODO: write N to archive file
    char str_files_count [NUMS_LEN];
    sprintf(str_files_count, "%d", files_count);  
    if(write(archive_descriptor, &str_files_count, NUMS_LEN) == -1){
        raiseErrnoBypassPath(func_name, "Не удалось записать в заголовок архива число файлов");
        return -1;
    }
    // TODO: write everything from header file to archive file
    header_descriptor = open(header_path, O_RDONLY, ALLOW); 
    if (header_descriptor == -1) {
        raiseErrno(func_name, "Не удалось создать временный файл заголовка", header_path);
        return -1;
    }
    while (read(header_descriptor, &tmp_char, sizeof(char)) != 0) {
        // Запись в архив
        if(write(archive_descriptor, &tmp_char, sizeof(char)) == -1){
            raiseErrnoBypassPath(func_name, "Ошибка при архивации");
            return -1;
        }
    }

    // Очищение памяти и возвращение в исходную директорию:
    remove(header_path);
    closedir(target_folder);
    close(archive_descriptor);
    close(header_descriptor);

    printf("\n%s\n\n", "Архивация завершена успешно!");
    
    return 0;
}

// untar() - функция для разархивации.
// - target_path - это путь к архиву, который надо разархивировать;
// - untar_path - это путь к папке, в которой должна оказаться папка 
//                с файлами из архива.
// Функция возвращает 0 в случае успешной разархивации, -1 при ошибках.
int untar(const char *target_path, const char *untar_path){

    const char *func_name = "untar"; // Для логов ошибок

    // Сохрание пути к папке программы, чтобы
    // после разархивации вернуться сюда:
    char programm_path[MAX_PATH_LENGTH]; 
    getcwd(programm_path, MAX_PATH_LENGTH);

    // Открытие архива:
    int archive_descriptor = open(target_path, O_RDONLY);
    if (archive_descriptor == -1) {
        raiseErrno(func_name, "Не удалось открыть архив", target_path);
        return -1;
    }

    // Подготовка папки для разархивации
    //
    // 1) Перемещаемся в каталог, в котором создадим новую папку для файлов из архива:
    if (chdir(untar_path) == -1) {
        raiseErrno(func_name, "Не удалось переместиться в каталог результатов разархивации", untar_path);
        return -1;
    }
    // 2) Формируем имя новой папки для содержимого архива:
    char unarchive_path[MAX_PATH_LENGTH];
    strcpy(unarchive_path, basename((char *)target_path));
    removeFileExtensionFromString(unarchive_path);
    // 3) Если необходимо, создаем новую папку:
    struct stat destinationFolderStat;
    if (stat(unarchive_path, &destinationFolderStat) == -1) {
        printf("%s%s\n\n", "Создаем папку для разархивации - ", unarchive_path);
        if (mkdir(unarchive_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IRWXO) == -1){
            raiseErrno(func_name, "Не удалось создать папку для разархивации", unarchive_path);
            return -1;
        }   
    }
    // 4) Перемещаемся в папку разархивации:
    if (chdir(unarchive_path) == -1) {
        raiseErrno(func_name, "Не удалось переместиться в папку для разархивации", unarchive_path);
        return -1;
    }
        
    // Чтение заголовка архива:

    // TODO: read number N from archive`s header
    char str_files_count [NUMS_LEN];
    if(read(archive_descriptor, &str_files_count, NUMS_LEN) == -1){
        raiseErrnoBypassPath(func_name, "Не удалось считать число файлов в архиве");
        return -1;
    }
    // TODO: read N entities (size - name) from archive`s header
    int files_count = atoi(str_files_count);
    while(files_count){
        char file_size [NUMS_LEN];
        if(read(archive_descriptor, &file_size, NUMS_LEN) == -1){
            raiseErrnoBypassPath(func_name, "Не удалось считать размер очередного файла в заголовке архива");
            return -1;
        }
        char file_name[MAX_PATH_LENGTH];
        if(read(archive_descriptor, &file_name, MAX_PATH_LENGTH) == -1){
            raiseErrnoBypassPath(func_name, "Не удалось считать имя очередного файла в заголовке архива");
            return -1;
        }
        char new_line_symbol;
        if(read(archive_descriptor, &new_line_symbol, 1) == -1){
            raiseErrnoBypassPath(func_name, "Не удалось считать перенос в заголовке архива");
            return -1;
        }
        files_count--;
    }
    
    
    // Чтение из архива самих файлов:
    char fileName[MAX_PATH_LENGTH]; // Имя очередного файла
    int fileDepth; // Глубина очередного файла в иерархии каталогов (0 или 1 для корня)
    int fileSize; // Длина очередного файла
    while (read(archive_descriptor, fileName, MAX_PATH_LENGTH)) { // Чтение имени
        read(archive_descriptor, &fileDepth, sizeof(int)); // Чтение глубины
        read(archive_descriptor, &fileSize, sizeof(int)); // Чтение размера

        if (fileDepth == 1){ // Встречен подкаталог корневой папки

            // Читаем во временный файл ровно fileSize байт из архива archive_descriptor,
            // при этом временный файл будет содержать только информацию о файлах из подкаталога:
            if(UnpackFile(TMP_FILE_NAME, fileSize, archive_descriptor) == -1){
                raiseErrAndSource(func_name, "Для папки не удалось создать временный файл", fileName);
                return -1;
            }

            // Создаем папку fileName и заполняем ее файлами, которые будем последовательно 
            // разархировать из временного файла. При этом указываем увеличенную глубину, 1, 
            // для последующей рекурсии вниз по иерархии:
            if(UnpackFolder(TMP_FILE_NAME, fileName, 1) == -1){
                raiseErrAndSource(func_name, "Не удалось разархивировать папка", fileName);
                return -1;
            }

            // Удаляем временный файл:
            if(remove(TMP_FILE_NAME) == -1){
                raiseErrno(func_name, "Не удалось удалить временный файл", TMP_FILE_NAME);
                return -1;
            }

        } else if (fileDepth == 0){ // Файл лежит в корневой папке
        
            // Читаем из архива archive_descriptor ровно fileSize байт
            // в создаваемый в корневой папке файл с именем fileName - он 
            // и будет результатом разархивации:
            if(UnpackFile(fileName, fileSize, archive_descriptor) == -1){
                raiseErrAndSource(func_name, "Не удалось разархивировать файл", fileName);
                return -1;
            }

        } else { // Ошибочная глубина файла

            raiseErr(func_name, "Архив критически неккоректен, неверная глубина файлов");
            return -1;

        } // if-else - анализ файлов архива

    }  // while - чтение архива

    // Закрытие созданного архива:
    if(close(archive_descriptor) == -1){
        raiseErrnoBypassPath(func_name, "Не удалось закрыть созданный архив");
        return -1;
    }

    // Возвращение в исходную папку:
    if(chdir(programm_path) == -1){
        raiseErrno(func_name, "Не удалось вернуться в исходную папку", programm_path);
        return -1;
    }



    printf("\n%s\n\n", "Разархивация завершена успешно!");
    
    return 0;
}   