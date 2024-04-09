#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <readline/readline.h>

void quit_signal_handler(int sig);
void interrupt_signal_handler(int sig);