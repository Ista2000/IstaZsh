#include "shell.h"
#include "utils.c"
#include "jobs.c"

int fore = -1;

int ls(char* parsed[], int x)
{
	char dir[100];
 	dir[0] = '.';
 	dir[1] = '\0';
	int a = 0, l = 0;
	if(x > 1)
	{
		if(parsed[1][0] != '-')
			strcpy(dir, parsed[1]);
		else
		{
			switch(parsed[1][1])
			{
				case 'a': a = 1; return 0;
				case 'l': l = 1;return 0;
				default: return 1;
			}
			if(parsed[1][2] != '\0')
			{
				switch(parsed[1][2])
				{
					case 'a': a = 1; return 0;
					case 'l': l = 1;return 0;
					default: return 1;
				}
			}
		}
	}
	if(x > 2)
	{
		if(parsed[2][0] != '-')
			strcpy(dir, parsed[2]);
		else
		{
			switch(parsed[2][1])
			{
				case 'a': a = 1; return 0;
				case 'l': l = 1;return 0;
				default: return 1;
			}
			if(parsed[2][2] != '\0')
			{
				switch(parsed[2][2])
				{
					case 'a': a = 1; return 0;
					case 'l': l = 1;return 0;
					default: return 1;
				}
			}
		}
	}
	if(x > 3)
	{
		if(parsed[3][0] != '-')
			strcpy(dir, parsed[3]);
		else
		{
			switch(parsed[3][1])
			{
				case 'a': a = 1; return 0;
				case 'l': l = 1;return 0;
				default: return 1;
			}
		}
	}
	struct dirent **namelist;
	int n;
	int i=0;
	n = scandir(dir, &namelist, NULL, alphasort);
	if (n < 0)
  		perror("scandir");
	else {
  		while (i<n) {
  			if(!a && namelist[i]->d_name[0] == '.')
  			{
  				++i;
  				continue;
  			}
  			if(l)
  			{
  				struct stat buff;
  				stat(namelist[i]->d_name, &buff);
			    printf( (S_ISDIR(buff.st_mode)) ? "d" : "-");
			    printf( (buff.st_mode & S_IRUSR) ? "r" : "-");
			    printf( (buff.st_mode & S_IWUSR) ? "w" : "-");
			    printf( (buff.st_mode & S_IXUSR) ? "x" : "-");
			    printf( (buff.st_mode & S_IRGRP) ? "r" : "-");
			    printf( (buff.st_mode & S_IWGRP) ? "w" : "-");
			    printf( (buff.st_mode & S_IXGRP) ? "x" : "-");
			    printf( (buff.st_mode & S_IROTH) ? "r" : "-");
			    printf( (buff.st_mode & S_IWOTH) ? "w" : "-");
			    printf( (buff.st_mode & S_IXOTH) ? "x" : "-");
			    printf("  %d", (int)buff.st_nlink);
			    struct passwd *pw = getpwuid(buff.st_uid);
				struct group  *gr = getgrgid(buff.st_gid);
				char date[20];
				strftime(date, 20, "%b  %d  %I:%M", gmtime(&(buff.st_ctime)));
			    printf(" %s  %s  %d  %s  %s", pw->pw_name, gr->gr_name, (int)buff.st_size, date, namelist[i]->d_name);
			    printf("\n");
  			}
  			else
  			{
  				struct stat buff;
  				stat(namelist[i]->d_name, &buff);
  				if(i&1)
			    	printf( (S_ISDIR(buff.st_mode)) ? BOLD BLUE "%-30s\n" RESET: "%-30s\n", namelist[i]->d_name);
			    else
			    	printf( (S_ISDIR(buff.st_mode)) ? BOLD BLUE "%-30s\t" RESET: "%-30s\t", namelist[i]->d_name);
     		}
     		free(namelist[i]);
     		++i;
  		}
  		free(namelist);
	}
}
int cd(char* parsed[], int x)
{
	char s[100];
	absolutePath(s, parsed[1]);
	chdir(s);
}
int pwd(char* parsed[], int x)
{
	char d[100];
    getcwd(d, 100);
    printf("%s\n", d);
}
int echo(char* parsed[], int x)
{
	for(int i = 1;i<x;i++)
		printf("%s ", parsed[i]);
}
int pinfo(char* parsed[], int x)
{
	int pid = (int)getpid();
	if(x == 2)
		pid = atoi(parsed[1]);
	else if(x > 2)
	{
		printf("There are unexpected number of arguments\nExpected 2, got %d", x);
		return 0;
	}
	char buf[500], path[100];

	sprintf(path, "/proc/%d/stat", pid);
	FILE *fp = fopen(path, "r");
	if(fp == NULL)
	{
		printf("Some error occured while running the command");
		return 0;
	}
	else
	{

		char dummy[500], fname[500], state[5];
		unsigned long int vsize;

		fscanf(fp, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %lu", dummy, fname, state, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, &vsize);
		printf("PID: %d\nFilename: %s\nProcess status: %s\nMemory: %lu\n", pid, fname, state, vsize);
		fclose(fp);
	}

	sprintf(path, "/proc/%d/exe", pid);
	readlink(path, buf, 500);
	printf("Executable path: %s\n", buf);
}
int history(char* parsed[], int x)
{
	int max_hist = 10;
	if(x == 2)
	{
		int temp = atoi(parsed[1]);
		if(temp <= 20)
			max_hist = temp;
		else
		{
			printf("History only accessible till the previous 20 commands.");
			return 0;
		}
	}
	HISTORY_STATE *hist_state = history_get_history_state ();
	HIST_ENTRY **hist_entries = history_list();
	if(hist_state->length < max_hist)
		max_hist = hist_state->length;
	for (int i = 0; i < max_hist; i++)
        printf ("%s\n", hist_entries[hist_state->length-1-i]->line);
}
int nightswatch(char* parsed[], int x)
{
	int interval = 2;
	char command_provided[20];
	if(x == 4)
	{
		if(parsed[1][1] == 'n')
			interval = atoi(parsed[2]);
		else
		{
			printf("Invalid option provided.");
			return 0;
		}
		strcpy(command_provided, parsed[3]);
	}
	else if(x!=2)
	{
		printf("This is not a valid way to run the command.");
		return 0;
	}
	else
	{
		strcpy(command_provided, parsed[1]);
	}
	char* commands[] = {"interrupt", "dirty"};
	if(strcmp(command_provided, commands[0]) == 0)
	{
		int num_cpu = get_nprocs();
		for(int i = 0;i<num_cpu;i++)
			printf("CPU%d\t", i);

		while(1)
		{
			FILE *interrupt = fopen("/proc/interrupts", "r");
			size_t len = 0;
			char * line = NULL;

			if (interrupt == NULL){
				perror("Error opening interrupt file.");
				return 0;
			}

			int i = 0;

			while(i < 3 && getline(&line, &len, interrupt) != -1) {
				i++;
			}
			long long int cpu[10];
			char garbage[10];

			sscanf(line, "%s", garbage);
			for(int i = 0;i<num_cpu;i++)
				sscanf(line, "%lld", &cpu[i]);
			printf("\n");
			for(int i = 0;i<num_cpu;i++)
				printf("%lld\t", cpu[i]);
			printf("\n");

			fclose(interrupt);
			if(inputAvailable())
			{
				char q = getchar();
				if(q == 'q' || q == 'Q')
					return 0;
			}
			sleep(interval);
		}
	}
	else if(strcmp(command_provided, commands[1]) == 0)
	{
		while(1)
		{
			FILE *meminfo = fopen("/proc/meminfo", "r");
			size_t len = 0;
			char * line = NULL;

			if (meminfo == NULL){
				perror("Error opening meminfo file: ");
				return 0;
			}

			int i = 0;

			while(i < 17 && getline(&line, &len, meminfo) != -1) {
				i++;
			}
			printf("%s", line);

			fclose(meminfo);
			if(inputAvailable())
			{
				char q = getchar();
				if(q == 'q' || q == 'Q')
					return 0;
			}
			sleep(interval);
		}
	}
}

int kjob(char *parsed[], int n)
{
	if(n < 3)
	{
		printf("Invalid number of arguments.\n");
		return 0;
	}
	int id = atoi(parsed[1]), sig = atoi(parsed[2]);
	kill(arr[id-1], sig);
	return 0;
}

int processCommand(char* parsed[], int n)
{
	int numCommands = 16;
	char* commandList[numCommands];
	commandList[0] = "cd";
	commandList[1] = "pwd";
	commandList[2] = "echo";
	commandList[3] = "ls";
	commandList[4] = "pinfo";
	commandList[5] = "clear";
	commandList[6] = "quit";
	commandList[7] = "history";
	commandList[8] = "nightswatch";
	commandList[9] = "jobs";
	commandList[10] = "kjob";
	commandList[11] = "overkill";
	commandList[12] = "fg";
	commandList[13] = "bg";
	commandList[14] = "setenv";
	commandList[15] = "unsetenv";
	int matched = numCommands;
	for(int i = 0;i < numCommands; i++)
		if(strcmp(parsed[0], commandList[i]) == 0)
		{
			matched = i;
			break;
		}
	int x = n;
	switch(matched)
	{
		case 0:
			cd(parsed, x);
			break;
		case 1: ;
		    pwd(parsed, x);
			break;
		case 2:
			echo(parsed, x);
			break;
		case 3: ;
		 	ls(parsed, x);
			break;
		case 4: ;
			pinfo(parsed, x);
			break;
		case 5:
			clear();
			break;
		case 6:
			for(int i = 0;i<sz;i++)
				kill(arr[i], 9);
			exit(0);
		case 7: ;
			history(parsed, x);
			break;
		case 8: ;
			nightswatch(parsed, x);
			break;
		case 9: ;
			print_all_jobs();
			break;
		case 10: ;
			kjob(parsed, x);
			break;
		case 11: ;
			for(int i = 0;i<sz;i++)
				kill(arr[i], 9);
			break;
		case 12: ;
			if(x != 2)
				printf("Unexpected number of arguments\n");
			else
			{
				int id = atoi(parsed[1]);
				kill(arr[id-1], SIGCONT);
				fore = arr[id-1];
				waitpid(arr[id-1], NULL, WUNTRACED);
				fore = -1;
			}
			break;
		case 13: ;
			if(x != 2)
				printf("Unexpected number of arguments\n");
			else
			{
				int id = atoi(parsed[1]);
				kill(arr[id-1], SIGCONT);
			}
			break;
		case 14: ;
			if(x != 3)
				printf("Unexpected number of arguments\n");
			else
				setenv(parsed[1], parsed[2], 1);
			break;
		case 15: ;
			if(x != 2)
				printf("Unexpected number of arguments\n");
			else
				unsetenv(parsed[1]);
			break;
		default:
			return 1;
	}
	return 0;
}

int processSystemCommands(char* parsed[], int n)
{
	int fd[2];
	if(pipe(fd) == -1)
	{
		printf("Some error occured.\n");
		return 1;
	}
	pid_t pid = fork();
	if(pid < 0)
	{
		printf("\nFailed creating a child fork\n");
		return 1;
	}
	else if(pid == 0)
	{
		setpgid(0, 0);
		if(execvp(parsed[0], parsed) < 0)
			printf("There was some error in executing the command\n");
		exit(1);
	}
	else
	{
		push_job(pid);
		if(parsed[n-1][0] == '&' && strlen(parsed[n-1]) == 1)
		{
			printf("\nProcess started with pid: %d\n", (int)pid);
		}
		else
		{
			int status;
			fore = pid;
			waitpid(pid, &status, WUNTRACED);
			if(WIFSTOPPED(status))
				push_job(pid);
			else if(!status)
				printf("\nProcess %s with pid %d ended normally\n", parsed[0], (int)pid);
			fore = -1;
		}
		return 0;
	}
}


void sigint_handler(int sig)
{
	if(fore == -1)
		signal(sig, SIG_IGN);
	else
	{
		kill(fore, sig);
		fore = -1;
	}
}
