#include "../include/libs.h"

#include "../include/my_exec.h"
#include "../include/string_process.h"

void execute_comands(int number_of_commands, char** commands){

    // printf("commands[i]:\n");
    // for(int i=0; i < number_of_commands; i++){
    //     printf("\"%s\"\n", commands[i]);
    // }

	int forked_processes = 0;

	for(int i=0;i<number_of_commands;i++){

        if(strlen(commands[i])==0){
            continue;
        }

	//	printf("--- i=%d ---\n", i);

      //  printf("Pre func command[%d]: '%s'\n", i, commands[i]);

		char* command_name = malloc(MAX_COMMAND_LENGTH * sizeof(char));
        char* command_arguments[MAX_COMMANDS_COUNT];
		int number_of_arguments = -2;
        
       // printf("%s\n", "going to get in func...");
		number_of_arguments = get_command_name_and_args(commands[i], command_name, command_arguments, MAX_COMMANDS_COUNT);

        char* args[MAX_COMMANDS_COUNT];
        args[0] = command_name;
        for(int j=0; j<number_of_arguments; j++){
			args[j+1] = command_arguments[j];
		}

       // printf("%s\n", "RESULTTTTTTTT:");
        //strcpy(command_name, "test");
		
	
        if(strlen(command_name)==0){
            printf("empry command, skipping...");
            continue;
        }

		forked_processes++;

        if(fork() == 0)
        {
            // printf("Name:'%s'\n", command_name);
            // for(int j=0; j<number_of_arguments+1; j++){
			//     printf("arg[%d]:\"%s\"\n", j, args[j]);
		    // }
            args[number_of_arguments+1]='\0';
            if(execvp(command_name, args)<0){
                printf("Не удалось exec()\n");
            }
        }

        free(command_name);
  
    }

    for(int i = 0; i < forked_processes; i++){
		wait(NULL);
	}
	
}



// // Function where the system command is executed 
// void execArgs(char** parsed) 
// { 
// 	printf("%s%s\n", "работаем с командой:", parsed[0]);
// 	// Forking a child 
// 	pid_t pid = fork(); 

// 	if (pid == -1) { 
// 		printf("\nFailed forking child.."); 
// 		return; 
// 	} else if (pid == 0) { 
// 		if (execvp(parsed[0], parsed) < 0) { 
// 			printf("\nCould not execute command.."); 
// 		} 
// 		exit(0); 
// 	} else { 
// 		// waiting for child to terminate 
// 		wait(NULL); 
// 		return; 
// 	} 
// } 


// // Function where the piped system commands is executed 
// void execArgsPiped(char** parsed, char** parsedpipe) 
// { 
// 	printf("%s%s", "работаем PIPED с командой:", parsed[0]);
// 	// 0 is read end, 1 is write end 
// 	int pipefd[2]; 
// 	pid_t p1, p2; 

// 	if (pipe(pipefd) < 0) { 
// 		printf("\nPipe could not be initialized"); 
// 		return; 
// 	} 
// 	p1 = fork(); 
// 	if (p1 < 0) { 
// 		printf("\nCould not fork"); 
// 		return; 
// 	} 

// 	if (p1 == 0) { 
// 		// Child 1 executing.. 
// 		// It only needs to write at the write end 
// 		close(pipefd[0]); 
// 		dup2(pipefd[1], STDOUT_FILENO); 
// 		close(pipefd[1]); 

// 		if (execvp(parsed[0], parsed) < 0) { 
// 			printf("\nCould not execute command 1.."); 
// 			exit(0); 
// 		} 
// 	} else { 
// 		// Parent executing 
// 		p2 = fork(); 

// 		if (p2 < 0) { 
// 			printf("\nCould not fork"); 
// 			return; 
// 		} 

// 		// Child 2 executing.. 
// 		// It only needs to read at the read end 
// 		if (p2 == 0) { 
// 			close(pipefd[1]); 
// 			dup2(pipefd[0], STDIN_FILENO); 
// 			close(pipefd[0]); 
// 			if (execvp(parsedpipe[0], parsedpipe) < 0) { 
// 				printf("\nCould not execute command 2.."); 
// 				exit(0); 
// 			} 
// 		} else { 
// 			// parent executing, waiting for two children 
// 			wait(NULL); 
// 			wait(NULL); 
// 		} 
// 	} 
// } 

// // Help command builtin 
// void openHelp() 
// { 
// 	puts("\n***ТЕРМИНАЛ БОГОМОЛЬСКОГО: HELP***"
// 		"\nList of Commands supported:"
// 		"\n>cd"
// 		"\n>ls"
// 		"\n>exit"
// 		"\n>all other general commands available in UNIX shell"
// 		"\n>pipe handling"
// 		"\n>improper space handling"); 

// 	return; 
// } 

// // Function to execute builtin commands 
// int ownCmdHandler(char** parsed) 
// { 
// 	int NoOfOwnCmds = 4, i, switchOwnArg = 0; 
// 	char* ListOfOwnCmds[NoOfOwnCmds]; 
// 	char* username; 

// 	ListOfOwnCmds[0] = "exit"; 
// 	ListOfOwnCmds[1] = "cd"; 
// 	ListOfOwnCmds[2] = "help"; 
// 	ListOfOwnCmds[3] = "hello"; 

// 	for (i = 0; i < NoOfOwnCmds; i++) { 
// 		if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) { 
// 			switchOwnArg = i + 1; 
// 			break; 
// 		} 
// 	} 

// 	switch (switchOwnArg) { 
// 	case 1: 
// 		printf("\nGoodbye\n"); 
// 		exit(0); 
// 	case 2: 
// 		chdir(parsed[1]); 
// 		return 1; 
// 	case 3: 
// 		openHelp(); 
// 		return 1; 
// 	case 4: 
// 		username = getenv("USER"); 
// 		printf("\nHello %s.\nMind that this is "
// 			"not a place to play around."
// 			"\nUse help to know more..\n", 
// 			username); 
// 		return 1; 
// 	default: 
// 		break; 
// 	} 

// 	return 0; 
// } 
