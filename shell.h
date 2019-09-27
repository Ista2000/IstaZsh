#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <dirent.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <pwd.h>
#include <grp.h>
#include <readline/readline.h> 
#include <readline/history.h>
#include <fcntl.h>

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define RESET "\x1b[0m"
#define BOLD "\e[1m"

#define clear() printf("\033[H\033[J")

int inputAvailable();
int checkpath(char*);
void substr(char*, char*, int, int);
void sanitizePath(char*, char*);
void absolutePath(char*, char*);
void prompt();
int takeInput(char*);
int processCommand(char**, int);
int processSystemCommands(char**, int);
int processCommandInput(char*);
int processInput(char*);
int ls(char**, int);
int cd(char**, int);
int pwd(char**, int);
int echo(char**, int);
int pinfo(char**, int);
int history(char**, int);
int nightswatch(char**, int);
