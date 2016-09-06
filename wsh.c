/****************************************************************
Author: Woytenko W.
Date: 06.09.2016
Comment: In this code was many errors. 
The global error is - isn't allocated memory correct. 
For this rewrite required subprogram, who allocate memory.
****************************************************************/


#include "wsh.h"


void init_buff(char ***argv_com)
{
	int i;
	printf("Allocating memory for %d lines %d bytes\n", argc_com, argv_com_len);
	//allocate memory	
	*argv_com = NULL;
	i = 0;
	*argv_com = (char**) malloc((argc_com) * sizeof(char *));
        printf("1L pointer: %p\n", *argv_com);
	for(i = 0; i < argc_com-1; i++) {
       		*(*argv_com + i) = (char *) malloc((argv_com_len + 1) * sizeof(char));
                printf("2L pointer: %p\n", *(*argv_com + i));
		*(*(*argv_com)) = '\0';
 	}
	*(*argv_com + argc_com) = NULL;
	printf("Null 2L pointer %p\n", *(*argv_com + argc_com));
}

void print_inv(void)
{
	printf("?USERNAME?:?DIR?$");
}

void read_command_line(char *command)
{
	int i;
	i = 0;
	while(i < MAX_BUFF && (command[i] = getchar()) != '\n') i++;
	command[i] = '\0';
	#ifdef TEST
		printf("Input line: %s\n", command);
	#endif
}

void calc_len(char *command)
{
	int i, start, stop;
	start = 0; stop = 0;
	argc_com = 0; argv_com_len = 0;
	for(i = 0; i < strlen(command); i++)
	{
		if (command[i] != ' ' && (i-1 < 0 || command[i - 1] == ' ') )
			start = i;		
		if(command[i] != ' ' && ( i+1 == strlen(command) || command[i + 1] == ' ') )
		{
			argc_com++;
			stop = i;
			if (argv_com_len < stop - start + 1)
				argv_com_len = stop - start +1;
		}
	}
	argc_com++; //This inc performed for NULL pointer
	printf("argc = %d Max length = %d\n", argc_com, argv_com_len);
}


void analysis_command(char *command, char *** argv_com)
{
	int i, start, stop, line;
	i = 0;
	start = -1;
	stop = -1;
	line = 0;
	for(i = 0; i < 32; i++)
	{
		*argv_com[i][0] = '\0';
	}	
	i = 0;
	while(command[i] != '\0')
	{
		if(start < 0)
			if(command[i] != ' ')
				start = i;
		if(start >= 0 && stop < 0)
			if(command[i+1] == ' ' || command[i+1] == '\0')
				stop = i;			
		if(start >= 0 && stop >= 0)
		{
			//Copy bytes
			strncpy(*argv_com[line], command + start, stop - start + 1);
			*argv_com[line][stop - start + 2] = '\0';
			start = -1;
			stop = -1;
			line++;
		}
		i++;
	}

}

void analysis_command_v2(char *command, char ** argv_com)
{
//Complete subprogram
	int i, start, stop, line;
	int max_len;
	i = 0;
	start = -1;
	stop = -1;
	max_len = 0;
	printf("debug print 1:\n");
	for(i = 0; i < argc_com - 1; i++)
	{
		printf("First Pointer %p\n", argv_com[i]);
	}

	i = 0;
	
	while(command[i] != '\0')
	{
		if(start < 0)
			if(command[i] != ' ')
				start = i;
		if(start >= 0 && stop < 0)
			if(command[i+1] == ' ' || command[i+1] == '\0')
				stop = i;			
		if(start >= 0 && stop >= 0)
		{
			//Copy bytes
			printf("Copy string: %s\n", argv_com[line]);			
			strncpy(argv_com[line], command + start, stop - start + 1);
                        argv_com[line][stop - start + 2] = '\0'; //Здесь почему-то уходит за рамки памяти
			printf("Copy string: %s\n", argv_com[line]);
			start = -1;
			stop = -1;
			line++;
		}
		i++;
	}
	printf("Debig print:\n");
	//Debug printf
	for(i = 0; i < argc_com-1; i++)
	{
		printf("Line[%d]: %s\n", i, argv_com[i]);
	}
	printf("Also complete\n");
}

void run_command(char **argv_com)
{
	int status;
	if(fork() != 0) {
		//parent process 
		printf("PPID: %d\n", getpid());
		waitpid(-1, &status, 0);
		printf("Waiting child process 1 sec\n");
		sleep(1);
		printf("Parent process success\n");
	}
	else {
		printf("CPID: %d\n", getpid());
		printf("Command: %s\n", *argv_com);
		//if(execve(*argv_com , argv_com, 0) == -1) printf("Execute error\n");	
		if(execve(argv_com[0], argv_com, 0) == -1) printf("Execute error\n");	
		perror("Execve: ");
		printf("Test delay 3 sec\n");
		sleep(3);
		printf("Child process susccess\n");
	}	
}
