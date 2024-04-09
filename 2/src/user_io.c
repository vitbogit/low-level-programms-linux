#include "../include/libs.h"
#include "../include/user_io.h"

// Отображение красивого приветствия:
void draw_beautiful_greetings() 
{ 
	clear(); // Очистка экрана

	printf("\n\n\n\n******************"
		"************************"); 
	printf("\n\n\n\t--- ТЕРМИНАЛ vitbogit ---"); 
	printf("\n\n\n\n*******************"
		"***********************"); 
	printf("\n"); 

	// Делаем паузу, чтобы пользователь нагляделся:
	sleep(BEAUTIFUL_GREETINGS_DURATION); 
	
	clear(); // Очистка экрана

} // draw_beautiful_greetings(). Отображение красивого приветствия.


// Печать текущего каталога:
void print_current_dir() 
{ 
	
	char cwd[MAXPATH]; // Current Working Directory

	getcwd(cwd, sizeof(cwd)); // Получение текущей директории

	printf("\r() Каталог: %s.", cwd); // Вывод cwd без переноса строки

} // print_current_dir(). Печать текущего каталога.


// Получаем ввод пользователя
int get_user_input(char* str) 
{ 
	// Буфер для временного хранения ввода пользователя:
	char* buf; 

	// Приглашаем пользователя ввести команду и считываем ее:
	buf = readline(" Ваша команда: "); 

	// Возвращаем определенный код, если ввод пустой:
	if (!buf || strlen(buf) == 0) {

		free(buf);
		return STATUS_CODE_EMPTY_USER_INPUT; 

	}

	add_history(buf); // История команд (нажимайте на стрелки вниз-вверх)
	strcpy(str, buf); // Запись считанной команды в нужную переменную
	
	free(buf);
	return 0; 

} // get_user_input(). Получаем ввод пользователя.


// // function for finding pipe 
// int parsePipe(char* str, char** strpiped) 
// { 
// 	int i; 
// 	for (i = 0; i < 2; i++) { 
// 		strpiped[i] = strsep(&str, "|"); 
// 		if (strpiped[i] == NULL) 
// 			break; 
// 	} 

// 	if (strpiped[1] == NULL) 
// 		return 0; // returns zero if no pipe is found. 
// 	else { 
// 		return 1; 
// 	} 
// } 

// // function for parsing command words 
// void parseSpace(char* str, char** parsed) 
// { 
// 	int i; 

// 	for (i = 0; i < MAX_COMMANDS_COUNT; i++) { 
// 		parsed[i] = strsep(&str, " "); 

// 		if (parsed[i] == NULL) 
// 			break; 
// 		if (strlen(parsed[i]) == 0) 
// 			i--; 
// 	} 
// } 
