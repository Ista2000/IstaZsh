#include "shell.h"
#include "input_handling.c"

int main()
{
	char input[100], command[100][100];

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