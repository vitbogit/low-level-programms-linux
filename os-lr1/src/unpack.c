#include "../include/tar.h"
#include "../include/libs.h"
#include "../include/constant_tmp_file.h"
#include "../include/errors.h"

// UnpackFile() создает файл по пути file_path, в который 
// записывает file_size байт из источника source_descriptor:
int UnpackFile(const char *file_path, int file_size, int source_descriptor){

    const char *func_name = "UnpackFile"; // Для логов ошибок

    // Вывод в логи имени разархивируемого файла,
    // при этом временные файлы, конечно, игнорируются:
    if (strcmp(file_path, TMP_FILE_NAME) != 0){
        printf("%s%s\n","Разархивация файла: ", file_path);
    }
    
    // Создание файла:
    int destination_descriptor = open(file_path, O_CREAT | O_WRONLY, ALLOW);
    if (destination_descriptor == -1){
        raiseErrno(func_name, "Не удалось создать файл - результат разархивации", file_path);
        return -1;
    }

    // Чтение из источника file_size байт: 
    while (file_size--) {
        char tempChar;
        // Чтение байта:
        if(read(source_descriptor, &tempChar, sizeof(char)) == -1){ 
            raiseErrnoBypassPath(func_name, "Неудачное чтение файла источника");
            return -1;
        }
        // Запись байта:
        if(write(destination_descriptor, &tempChar, sizeof(char)) == -1){ 
            raiseErrnoBypassPath(func_name, "Неудачная запись в файл-результат");
            return -1;
        }
    }
    
    // Закрытие созданного файла:
    if(close(destination_descriptor) == -1){
        raiseErrnoBypassPath(func_name, "Не удалось закрыть созданный файл, результат разархивации");
        return -1;
    }

    return 0;
}

int UnpackFolder(const char *archive_path, const char *result_path, int depth) {

    const char *func_name = "UnpackFolder"; // Для логов ошибок

    // Вывод информационного сообщения в консоль об разархивируемом файле-каталоге.
    printf("%s%s\n","Разархивация файла-каталога: ", result_path);

    // Открытие исходного архива:
    int archive_descriptor = open(archive_path, O_RDONLY);
    if (archive_descriptor == -1) {
        raiseErrno(func_name, "Не удалось открыть исходный архив", archive_path);
        return -1;
    }

    // Создание папки для результата разархивации:
    struct stat destinationFolderStat;
    if (stat(result_path, &destinationFolderStat) == -1) {

        if(mkdir(result_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IRWXO) == -1){
            raiseErrno(func_name, "Не удалось создать папку - результат разархивации папки", result_path);
            return -1;
        }

    }

    // Переход в папку, результат разархивации:
    if(chdir(result_path) == -1){
        raiseErrno(func_name, "Не удалось перейти в папку - результат разархивации папки", result_path);
        return -1;
    }

    char fileName[MAX_PATH_LENGTH]; // Имя очередного файла
    int fileDepth;  // Глубина очередного файла в иерархии каталогов - различается
                    // не более чем на единицу в данном каталоге
    int fileSize; // Длина очередного файла
    while (read(archive_descriptor, fileName, MAX_PATH_LENGTH)) {
        read(archive_descriptor, &fileDepth, sizeof(int));
        read(archive_descriptor, &fileSize, sizeof(int));

        if (fileDepth == depth + 1){ // Встречен подкаталог 

            // Читаем во временный файл ровно fileSize байт из архива archive_descriptor,
            // при этом временный файл будет содержать только информацию о файлах из подкаталога:
            if(UnpackFile(TMP_FILE_NAME, fileSize, archive_descriptor) == -1){
                raiseErrAndSource(func_name, "Для папки не удалось создать временный файл", fileName);
                return -1;
            }

            // Создаем папку fileName и заполняем ее файлами, которые будем последовательно 
            // разархировать из временного файла. При этом указываем увеличенную на 1 глубину  
            // для последующей рекурсии вниз по иерархии:
            if(UnpackFolder(TMP_FILE_NAME, fileName, depth + 1) == -1){
                raiseErrAndSource(func_name, "Не удалось разархивировать папка", fileName);
                return -1;
            }

            // Удаляем временный файл:
            if(remove(TMP_FILE_NAME) == -1){
                raiseErrno(func_name, "Не удалось удалить временный файл", TMP_FILE_NAME);
                return -1;
            }

        } else if (fileDepth == depth){ // Файл лежит в данной папке
        
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
    }

    // Закрытие созданного архива:
    if(close(archive_descriptor) == -1){
        raiseErrnoBypassPath(func_name, "Не удалось закрыть архив");
        return -1;
    }

    // Возвращение в папку уровнем выше:
    if(chdir("..") == -1){
        raiseErrno(func_name, "Не удалось вернуться в исходную папку", "..");
        return -1;
    }

    return 0;
}
