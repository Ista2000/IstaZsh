#include "shell.h"
#include "input_handling.c"


int main()
{
	char input[100], command[100][100];

	signal(SIGINT, sigint_handler);
	signal(SIGTSTP, sigint_handler);

	init_shell();
	using_history();
	while(1)
	{
		if(takeInput(input))
			continue;
		processInput(input);
	}

	return 0;
}