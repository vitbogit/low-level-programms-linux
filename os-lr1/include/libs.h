#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>

#define MAX_PATH_LENGTH 400 // Максимальная длина пути к файлу, которая может быть корректно обработана программой
#define NUMS_LEN 10

#define ALLOW 00666 
//0666 is rw-rw-rw- 
// 0777 is rwxrwxrwx

// Режим "NEW_FILE_MODE" является сокращение для объединения: 
//    O_CREAT - при необходимости создает файл
//    O_WRONLY - открытие только для записи
//    O_TRUNC - задает нулевую длину файла, отбрасывая существующее содержимое
#define NEW_FILE_MODE O_CREAT | O_WRONLY | O_TRUNC

// Режим "APPEND_MODE" является сокращение для объединения O_CREAT, O_WRONLY и:
//    O_APPEND - файл открывается в режиме добавления, перед каждой операцией 
//              write файловый указатель будет устанавливаться в конце файла
#define APPEND_MODE O_CREAT | O_WRONLY | O_APPEND