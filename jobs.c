#include "shell.h"

static pid_t arr[1001];
static int sz = 0;

int print(int in, pid_t pid)
{
	char buf[500], path[100];

	sprintf(path, "/proc/%d/stat", (int)pid);
	FILE *fp = fopen(path, "r");
	if(fp == NULL)
		return 1;
	else
	{

		char dummy[500], fname[500], state[5];

		fscanf(fp, "%s %s %s", dummy, fname, state);
		if(state[0] == 'Z')
			return 1;
		fname[strlen(fname)-1] = '\0';
		printf(" [%d] %s[%d]: %s\n", in, fname+1, (int)pid, state);
		fclose(fp);

		return 0;
	}
}

int push_job(pid_t pid)
{
	arr[sz++] = pid;
	return 0;
}

int print_all_jobs()
{
	for(int i = 0, j;i<sz;i++)
		if(print(i+1, arr[i]))
			for(j = i--, sz--;j<sz;j++)
				arr[j] = arr[j+1];
}

