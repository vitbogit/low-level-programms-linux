
#define clear() printf("\033[H\033[J") // Очистка терминала

// Отображение красивого приветствия:
void draw_beautiful_greetings();

// Печать текущего каталога:
void print_current_dir();

// Получаем ввод пользователя
int get_user_input(char* str);