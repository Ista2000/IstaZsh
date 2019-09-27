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

int processInRedirection(char *input)
{
	char *command = strsep(&input, "<");
	int fd = -1, temp_fd = -1;
	if(input != NULL)
	{
		while(*input == ' ')
			input++;
		int end = 0;
		for(int i = strlen(input)-1;i >= 0 && input[i] == ' ';i--)
			end = i;
		input[end] = '\0';
		fd = open(input, O_RDONLY);
		temp_fd = dup(0);
		dup2(fd, 0);

		processCommandInput(command);

		dup2(temp_fd, 0);

		close(fd);
		return 0;
	}
	processCommandInput(command);
	return 0;
}

int processOutRedirection(char *input)
{
	char *command = strsep(&input, ">");
	int fd = -1, temp_fd = -1;
	if(input != NULL)
	{
		int flag = 0;
		if(input[0] == '>')
		{
			strsep(&input, ">");
			flag = 1;
		}
		while(*input == ' ')
			input++;
		int end = 0;
		for(int i = strlen(input)-1;i >= 0 && input[i] == ' ';i--)
			end = i;
		input[end] = '\0';

		if(flag)
			fd = open(input, O_WRONLY | O_APPEND);
		else
		{
			fd = open(input, O_WRONLY);
		}
		temp_fd = dup(1);
		dup2(fd, 1);

		processInRedirection(command);

		dup2(temp_fd, 1);

		close(fd);
		return 0;
	}
	processInRedirection(command);
}

int processPipe(char *input)
{
	char *command = strsep(&input, "|");
	if(input == NULL)
		return processOutRedirection(command);

	int fd[2];
	if(pipe(fd) == -1)
	{
		printf("Something went wrong with pipe.\n");
		return 1;
	}


	pid_t pid = fork();
	if(pid < 0)
	{
		printf("Something went wrong with forking the process.\n");
		return 1;
	}
	else if(pid > 0)
	{
		waitpid(pid, NULL, 0);

		int temp_in = dup(0);
		dup2(fd[0], 0);
		processPipe(input);
		dup2(temp_in, 0);
		close(fd[0]);
	}
	else
	{	
		int temp_out = dup(1);
		dup2(fd[1], 1);
		processOutRedirection(command);
		dup2(temp_out, 1);
		close(fd[1]);
		exit(0);
	}
	return 0;
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
		processPipe(command);
	}
}
