#include "shell.h"

char tilde[100];

int inputAvailable()  
{
	struct timeval tv;
	fd_set fds;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	FD_ZERO(&fds);
	FD_SET(STDIN_FILENO, &fds);
	select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
	return (FD_ISSET(0, &fds));
}

int checkpath(char s[]) 
{
	if(strlen(s) < strlen(tilde))
		return 0;
	for(int i = 0;i<strlen(tilde);i++)
		if(tilde[i] != s[i])
			return 0;
	return 1;
}

void substr(char s[], char sub[], int l, int r)
{
	int i = 0;
	while(i+l < r)
		sub[i] = s[i+l], i++;
	sub[i] = '\0';
}

void sanitizePath(char dest[], char s[])
{
	if(checkpath(s))
	{
		dest[0] = '~';
		dest[1] = '\0';
		char subs[100];
		substr(s, subs, strlen(tilde), strlen(s));
		strcat(dest, subs);
	}
	else
		strcpy(dest, s);
}

void absolutePath(char dest[], char s[])
{
	if(s[0] == '~')
	{
		strcpy(dest, tilde);
		char sub[100];
		substr(s, sub, 1, strlen(s));
		strcat(dest, sub);
	}
	else
		strcpy(dest, s);
}

void prompt()
{
	char* username = getenv("USER");
    char dir[100];
    getcwd(dir, 100);

    char showdir[100];
    sanitizePath(showdir, dir);

    char hostname[50];
    gethostname(hostname, 50);
    printf(BOLD GREEN "\n<%s@%s" RESET BOLD ":" CYAN "%s>" RESET, username, hostname, showdir);
    fflush(stdout);
}

void init_shell() 
{ 
    clear(); 
    printf(BOLD RED "\n\n\n\n***********************************************************************"); 
    printf("\n\n\n\t\t****Welcome to Istazsh****"); 
    printf("\n\n\t-Not copied from geeks for geeks-");
    printf("\n\n\n\n***********************************************************************" RESET);
    getcwd(tilde, 100);
}