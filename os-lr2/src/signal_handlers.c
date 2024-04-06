#include "../include/libs.h"
#include "../include/signal_handlers.h"

void quit_signal_handler(int sig)
{
    signal(sig, SIG_IGN); // SIG_IGN - игнорируем сигнал

	char c;
	printf("\nВы точно хотите выйти? [y/n]\n");
	c = getchar();
	if (c == 'y')
		exit(0);
	
	printf("%s\n", "Нажмите Enter для возвращения...");
	c = getchar();

    signal(SIGQUIT, quit_signal_handler);
}

void interrupt_signal_handler(int sig)
{
	signal(sig, SIG_IGN); // SIG_IGN - игнорируем сигнал

    printf("\nПрервано.\n");

    signal(SIGINT, interrupt_signal_handler);

    // // Посылаем процессу сигнал для прерывания:
	// if(raise(sig) != 0){
    //     printf("hmmm\n");
    // }
    //rl_free_line_state;
    // printf("\n");
    // rl_on_new_line(); // Regenerate the prompt on a newline
    // rl_replace_line("", 0); // Clear the previous text
    //rl_redisplay();
}