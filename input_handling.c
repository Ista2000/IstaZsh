#include "shell.h"
#include "commands.c"


int takeInput(char* str) 
{ 
    char* buf; 
  	prompt();
    buf = readline(" $ "); 
    if (strlen(buf) != 0) { 
        add_history(buf); 
        strcpy(str, buf); 
        return 0; 
    } else { 
        return 1; 
    } 
}

int processCommandInput(char* input)
{
	char* parsed[20];
	int i;
	for(i = 0;i<20;i++)
	{
		parsed[i] = strsep(&input, " ");
		if(parsed[i] == NULL)
			break;
		char temp[] = "";
		if(strcmp(parsed[i], temp) == 0)
			i--;
		if(i == 19)
		{
			printf("Too many arguments!");
			return 1;
		}
	}
	int execFlag = processCommand(parsed, i);
	if(execFlag == 1)
		processSystemCommands(parsed, i);
	return execFlag;
}

int processInput(char *input)
{
	char* command;
	int i = 0;
	for(i = 0;i<30;i++)
	{
		command = strsep(&input, ";");
		if(command == NULL)
			break;
		char temp[] = "";
		if(strcmp(command, temp) == 0)
			i--;
		if(i == 29)
		{
			printf("Too many commands!");
			return 1;
		}
		processCommandInput(command);
	}
}
