#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h> 
#include<readline/readline.h> 
#include<readline/history.h> 

#define MAX_COMMAND_LENGTH 200 // Максимальная длина команды в консоли
#define MAX_COMMANDS_COUNT 10 // Максимальное число команд в команде
#define MAXPATH 200 // Максимальная поддерживая длина пути

#define BEAUTIFUL_GREETINGS_DURATION 2 // Время приветствия
#define STATUS_CODE_EMPTY_USER_INPUT 1 // Код пустого ввода пользователя