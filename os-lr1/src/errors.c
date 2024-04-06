#include "../include/libs.h"
#include "../include/errors.h"

void raiseErr(const char * func, const char *errMsg){
    printf("Ошибка в %s(): %s\n", func, errMsg);
}

void raiseErrAndSource(const char * func, const char *errMsg, const char *errSource){
    printf("Ошибка в %s(): %s - %s\n", func, errMsg, errSource);
}

void raiseErrno(const char * func, const char *errMsg, const char *errnoPath){
    printf("Ошибка в %s(): %s\n", func, errMsg);
    perror(errnoPath);
}

void raiseErrnoBypassPath(const char * func, const char *errMsg){
    printf("Ошибка в %s(): %s\n", func, errMsg);
    perror(strerror(errno));
}