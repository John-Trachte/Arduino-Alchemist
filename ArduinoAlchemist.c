#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

void setHigh(int arduinoPin);

void setLow(int arduinoPin);

void parseSetup(FILE* raw, FILE* baremetal);

void parseLoop();

void lex(FILE* raw, FILE* baremetal);


int main(int argc, char const *argv[])
{
	FILE* raw = fopen("Arduino.ino", "r");
	FILE* baremetal = fopen("Baremetal.ino", "a");

	lex(raw, baremetal);

	fclose(raw);
	fclose(baremetal);

	return 0;
}

void setHigh(int arduinoPin)
{
	// write 1 to the register bit corresponding to arduinoPin
}

void setLow(int arduinoPin)
{
	// write 1 to the register bit corresponding to arduinoPin
}

void parseSetup(FILE* raw, FILE* baremetal)
{
	fgetc(raw); // closing parenthesis
	fgetc(raw); // newline character
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
			char port[3];
			port[0] = fgetc(raw);
			char secondDigit = fgetc(raw);
			if(secondDigit != ',')
			{
				port[1] = secondDigit;
			}

			int portNum = atoi(port);
			fgets(command, 100, raw);

			if(portNum < 8) // only allows setting port as input
			{
				fputs("\tvolatile uint8_t* pin", baremetal);
				fputs(port, baremetal);

				if(strstr(command, "OUTPUT") != NULL)
				{
					fputs(" = 0x2A|(1<<", baremetal);
				}
				else if(strstr(command, "INPUT") != NULL)
				{
					fputs(" = 0x2A&~(1<<", baremetal);
				}

				fputs(port, baremetal);
				fputs(");\n", baremetal);
			}
			else if(portNum < 14)
			{
				fputs("\tvolatile uint8_t* pin", baremetal);
				fputs(port, baremetal);

				if(strstr(command, "OUTPUT") != NULL)
				{
					fputs(" = 0x24|(1<<", baremetal);
				}
				else if(strstr(command, "INPUT") != NULL)
				{
					fputs(" = 0x24&~(1<<", baremetal);
				}

				sprintf(port, "%d", portNum - 8);
				fputs(port, baremetal);
				fputs(");\n", baremetal);
			}
			else if(portNum < 20)
			{
				fputs("\tvolatile uint8_t* pin", baremetal);
				fputs(port, baremetal);

				if(strstr(command, "OUTPUT") != NULL)
				{
					fputs(" = 0x27|(1<<", baremetal);
				}
				else if(strstr(command, "INPUT") != NULL)
				{
					fputs(" = 0x27&~(1<<", baremetal);
				}

				sprintf(port, "%d", portNum - 13);
				fputs(port, baremetal);
				fputs(");\n", baremetal);
			}

			memset(command, '\n', sizeof(command));
		}

		curr = fgetc(raw);
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
			if(strstr(command, "setup") != NULL)
			{
				fputs("int main() {\n", baremetal);
				parseSetup(raw, baremetal);
			}
			else if(strstr(command, "loop") != NULL)
			{
				fputs("\n\twhile(1) {", baremetal);
				parseLoop(raw, baremetal);
				fputs("\n\t}\n\treturn 0;\n}", baremetal);
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