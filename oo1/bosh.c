/* bosh.c : BOSC shell */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/utsname.h> // For the uname() function
#include <readline/readline.h>
#include <readline/history.h>
#include "parser.h"
#include "print.h"
#include "commands.h"

/* --- symbolic constants --- */
#define HOSTNAMEMAX 100

/* --- use the /proc filesystem to obtain the hostname --- */
char *gethostname(char *hostname, size_t size)
{
	FILE *hostnamefile;
	char hname[HOSTNAMEMAX];
	char line[HOSTNAMEMAX];
	
	hostnamefile = fopen ("/proc/sys/kernel/hostname", "r");

	while (fgets(line, HOSTNAMEMAX, hostnamefile))
	{
		if(sscanf(line, "%s", hname))
		snprintf(hostname, size, "%s@%s", getenv("USER"), hname);
	}
  
	return hostname;
}

/* --- execute a shell command --- */
int executeshellcmd (Shellcmd *shellcmd)
{
	printshellcmd(shellcmd);
	pid_t pid;
	int *ret_status;
	char *commands[100];
	
	if (!strcmp(args[0], "exit" )) exit(0);
	Cmd *cmdlist = shellcmd->the_cmds;
	
	pid = fork();
	if (pid)
	{
        printf("Waiting for child (%d)\n", pid);
        pid = wait(ret_status);
        printf("Child (%d) finished\n", pid);
	}
	else
	{
		while (cmdlist != NULL)
		{
			char **cmd = cmdlist->cmd;
			cmdlist = cmdlist->next;
			commands++ = cmd;
		}
		
		commands++ = NULL;
		
		if(execvp(command, commands))
		{
			
		}
	}
	
	return 0;
}

/* --- main loop of the simple shell --- */
int main(int argc, char* argv[]) {

	/* initialize the shell */
	char *cmdline;
	char hostname[HOSTNAMEMAX];
	int terminate = 0;
	Shellcmd shellcmd;

	if (gethostname(hostname, sizeof(hostname))) 
	{

    /* parse commands until exit or ctrl-c */
		while (!terminate) 
		{
			printf("%s", hostname);
			if (cmdline = readline(":# ")) 
			{
				if(*cmdline) 
				{
					add_history(cmdline);
					if (parsecommand(cmdline, &shellcmd)) 
					{
						terminate = executeshellcmd(&shellcmd);
					}
				}
			
			free(cmdline);
			} 
			else terminate = 1;
		}
		
		printf("Exiting bosh.\n");
	}    
    
	return EXIT_SUCCESS;
}
