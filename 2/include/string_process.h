#include <string.h>

char *trim(char *str);

int split_input_string(char* str, char** separated_commands);

int get_command_name_and_args(char* command, char* command_name, char** command_arguments, int max_list);