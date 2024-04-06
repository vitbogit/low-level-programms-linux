#include "../include/libs.h"
#include "../include/string_process.h"

char *trim(char *str)
{
    size_t len = 0;
    char *frontp = str;
    char *endp = NULL;

    if( str == NULL ) { return NULL; }
    if( str[0] == '\0' ) { return str; }

    len = strlen(str);
    endp = str + len;

    /* Move the front and back pointers to address the first non-whitespace
     * characters from each end.
     */
    while( isspace((unsigned char) *frontp) ) { ++frontp; }
    if( endp != frontp )
    {
        while( isspace((unsigned char) *(--endp)) && endp != frontp ) {}
    }

    if( frontp != str && endp == frontp )
            *str = '\0';
    else if( str + len - 1 != endp )
            *(endp + 1) = '\0';

    /* Shift the string so that it starts at str so that if it's dynamically
     * allocated, we can still free it on the returned pointer.  Note the reuse
     * of endp to mean the front of the string buffer now.
     */
    endp = str;
    if( frontp != str )
    {
            while( *frontp ) { *endp++ = *frontp++; }
            *endp = '\0';
    }

    return str;
}

int split_input_string(char* str, char** separated_commands) 
{ 
	// Подсчитываем число команд (количество символов |):
	int number_of_commands = 1;
	for (int i = 0; str[i]; i++)
  		number_of_commands += (str[i] == '|');

	// Разделяем исходную строку на строки команд:
	for (int i = 0; i < number_of_commands; i++) { 
		separated_commands[i] = trim(strsep(&str, "|")); 
	} 

	return number_of_commands;

} 

int get_command_name_and_args(char* command, char* command_name, char** command_arguments, int max_list){
	//printf("in func :'%s'\n", command_name);
	while(1){
		char *arr = strsep(&command, " ");
		// printf("just got result of separaing:\n");
		// printf("%s\n", arr);
		//printf("now copying to command_name...\n");
		strcpy(command_name, arr);
		//printf("copied!\n");
		//printf("in func command_name:'%s'\n", command_name);
		if(command_name == NULL){
			command_name = command;
			return 0;
		}
		if(strlen(command_name)==0) continue;
		if(strcmp(command_name, " ")==0) continue;
		break;
	}
	
//	printf("debug left:'%s'\n", command_name);
//	printf("debug cmd left:'%s'\n", command);

	for (int i = 0; i < max_list; i++) { 
		command_arguments[i] = strsep(&command, " ");

		if (command_arguments[i] == NULL) {
			//printf("debug return name:'%s'\n", command_name);
			//printf("debug return arg0:'%s'\n", command_arguments[0]);
			return i;
		}
		if (strlen(command_arguments[i]) == 0){
			i--;
		}
	} 

	return -1;
}


