#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void lex(FILE* raw, FILE* baremetal)
{
	/**
	read characters, add to a char* command
	if(char == " or { or ( or ' or ,? or ;?)
		check if char* command is an arduino command
			if it is, translate it (call a parse fn)
			otherwise push it to ouptut file
	**/

	char command[100];
	int len = 0;
	char curr = fgetc(raw);
	while(curr != EOF)
	{
		if((curr == '(')) // function is complete, parameters follow
		{
			// analyze command
			if(strcmp(command, "setup") == 0)
			{

			}
			else if(strcmp(command, "loop") == 0)
			{

			}
			else if(0)
			{
				// follow previous conditions
			}

			memset(command, '\n', sizeof(command));
			len = 0;
		}

		if((curr == '"')) // parameter is a string, can be any length
		{
			// fgets to avoid 100 char limit
		}

		len++;
		if(len != 100)
		{
			command[len] = curr; // would like to use strcat here, causes stack smashing error
		}
		else
		{
			printf("\n\nLine over length limit\n\n");
			exit(1);
		}
		
		curr = fgetc(raw);
	}
}

void parseSetup()
{

}

void parseLoop()
{
	
}

int main(int argc, char const *argv[])
{
	FILE* raw = fopen("Arduino.ino", "r");
	FILE* baremetal = fopen("Baremetal.ino", "r");

	lex(raw, baremetal);

	fclose(raw);
	fclose(baremetal);

	return 0;
}