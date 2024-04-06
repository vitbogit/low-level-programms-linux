#include "../include/libs.h"

#include "../include/signal_handlers.h"
#include "../include/string_process.h"
#include "../include/user_io.h"
#include "../include/my_exec.h"

// Основная программа
int main() { 

	// -----------------------------------------
	// ---------- ОСНОВНЫЕ ПЕРЕМЕННЫЕ ----------
	// -----------------------------------------


	// Строка ввода пользователя из консоли:
	char *input_string = malloc(MAX_COMMAND_LENGTH * sizeof(char)); 

	// Выделенные из строки пользователя команды (разделены '|'):
	char **commands = malloc(MAX_COMMANDS_COUNT * sizeof(char*));

	// Число выделенных из строки пользователя команд:
	int commands_count;


	// -----------------------------------------
	// --- ПОДКЛЮЧЕНИЕ ОБРАБОТЧИКОВ СИГНАЛОВ ---
	// -----------------------------------------


	// Обработчик сигнала прерывания (Сntrl+С)
	if (signal(SIGINT, interrupt_signal_handler) == SIG_ERR){
        printf("Ошибка. Не удалось подключить обработчик сигнала SIGINT!\n");
		return -1;
	}

	// Обработчик сигнала выхода (Сntrl+D)
	if (signal(SIGQUIT, quit_signal_handler) == SIG_ERR){
        printf("Ошибка. Не удалось подключить обработчик сигнала SIGQUIT!\n");
		return -1;
	}


	// -----------------------------------------
	// ------- ПРИВЕТСТВИЕ ПОЛЬЗОВАТЕЛЯ --------
	// ----------------------------------------- 
	

	// Вывод приветствия:
	draw_beautiful_greetings(); 


	// -----------------------------------------
	// ---- ОСНОВНОЙ ЦИКЛ РАБОТЫ ТЕРМИНАЛА -----
	// -----------------------------------------


	// Цикл терминала:
	while (1) { 

		// 1. Печать текущей директории в начале строки:
		print_current_dir(); 

		// 2. Считывание строки пользовательского ввода:
		if (get_user_input(input_string) == STATUS_CODE_EMPTY_USER_INPUT) 
			continue; // Если ввод пустой, далее не обрабатываем

		// 3. Разделяем строку пользователя на комманды 
		commands_count = split_input_string(input_string, commands);  

		if(commands_count > MAX_COMMANDS_COUNT){
			printf("%s...\n");
			continue;
		}

		// for(int i=0;i<commands_count;i++){
		// 	printf("\"%s\"\n", commands[i]);
		// }
		// continue;

		// 4. Выполнение введенных комманд:
		execute_comands(commands_count, commands);
		
	} // Цикл терминала

	free(input_string);
	free(commands);

	return 0;

} // main ()
