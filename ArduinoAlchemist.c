#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void putHigh(int arduinoPin);

void putLow(int arduinoPin);

void parseSetup(FILE* raw, FILE* baremetal);

void parseLoop();

void lex(FILE* raw, FILE* baremetal);


int main(int argc, char const *argv[])
{
	FILE* raw = fopen("Arduino.ino", "r");
	FILE* baremetal = fopen("Baremetal.ino", "r");

	lex(raw, baremetal);

	fclose(raw);
	fclose(baremetal);

	return 0;
}

void putHigh(int arduinoPin)
{
	// write 1 to the register bit corresponding to arduinoPin
}

void putLow(int arduinoPin)
{
	// write 1 to the register bit corresponding to arduinoPin
}

void parseSetup(FILE* raw, FILE* baremetal)
{
	fgetc(raw);
	if(fgetc(raw) != '{')
	{
		printf("Could not find body of function\nsetup\n");
		exit(1);
	}

	char command[100];
	int len = 0;
	char curr = fgetc(raw);
	while(curr != '}')
	{
		if((curr == '(')) // function is complete, parameters follow
		{
			// pin number might be one or two digits long
			char port[2];
			port[0] = fgetc(raw);
			char secondDigit = fgetc(raw);
			if(secondDigit != ',')
			{
				port[1] = secondDigit;
			}
		}
	}
}

void parseLoop()
{
	
}

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
			// analyze command[]
			if(strcmp(command, "setup") == 0)
			{
				parseSetup(raw, baremetal);
			}
			else if(strcmp(command, "loop") == 0)
			{
				parseLoop(raw, baremetal);
			}
			else if(0) // only functions with no parameters
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