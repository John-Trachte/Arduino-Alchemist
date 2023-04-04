#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

void setHigh(int arduinoPin, FILE* baremetal);

void setLow(int arduinoPin, FILE* baremetal);

void parseSetup(FILE* raw, FILE* baremetal);

void parseLoop(FILE* raw, FILE* baremetal);

void parseDW(FILE* raw, FILE* baremetal);

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

void setHigh(int arduinoPin, FILE* baremetal)
{
	// write 1 to the register bit corresponding to arduinoPin, declare if not in states[]
	char temp[3];

	if(arduinoPin < 8) 
	{
		fputs("\n\t*portD |= (1<<", baremetal);
		sprintf(temp, "%d", arduinoPin);
		fputs(temp, baremetal);
		fputs(");", baremetal);
	}
	else if(arduinoPin < 14)
	{
		fputs("\n\t*portB |= (1<<", baremetal);
		sprintf(temp, "%d", arduinoPin - 8);
		fputs(temp, baremetal);
		fputs(");", baremetal);
	}
	else if(arduinoPin < 20)
	{
		fputs("\n\t*portC |= (1<<", baremetal);
		sprintf(temp, "%d", arduinoPin - 14);
		fputs(temp, baremetal);
		fputs(");", baremetal);
	}
}

void setLow(int arduinoPin, FILE* baremetal)
{
	// write 0 to the register bit corresponding to arduinoPin, declare if not in states[]
	char temp[3];

	if(arduinoPin < 8) 
	{
		fputs("\n\t*portD &= ~(1<<", baremetal);
		sprintf(temp, "%d", arduinoPin);
		fputs(temp, baremetal);
		fputs(");", baremetal);
	}
	else if(arduinoPin < 14)
	{
		fputs("\n\t*portB &= ~(1<<", baremetal);
		sprintf(temp, "%d", arduinoPin - 8);
		fputs(temp, baremetal);
		fputs(");", baremetal);
	}
	else if(arduinoPin < 20)
	{
		fputs("\n\t*portC &= ~(1<<", baremetal);
		sprintf(temp, "%d", arduinoPin - 14);
		fputs(temp, baremetal);
		fputs(");", baremetal);
	}
}

void parseSetup(FILE* raw, FILE* baremetal)
{
	int states[3] = {0, 0, 0};

	fputs("int main() {\n", baremetal);
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
			char port[3];
			int index = 0;
			char curr = fgetc(raw);
			while(curr != ',') // pin number might be one or two digits long
			{
				port[index] = curr;
				curr = fgetc(raw);
				index++;
			}

			int portNum = atoi(port);
			fgets(command, 100, raw);

			if(portNum < 8) 
			{
				if(states[0] == 0) // declare ports so that data can be written without redeclaring each loop
				{
					fputs("\n\tvolatile uint8_t* portD = 0x2B;\n", baremetal);
					states[0] = 1;
				}

				fputs("\tvolatile uint8_t* pin", baremetal); // configure Data Direction Registers
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
				if(states[1] == 0)
				{
					fputs("\n\tvolatile uint8_t* portB = 0x25;\n", baremetal);
					states[1] = 1;
				}

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
				if(states[2] == 0)
				{
					fputs("\n\tvolatile uint8_t* portC = 0x28;\n", baremetal);
					states[2] = 1;
				}

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

				sprintf(port, "%d", portNum - 14);
				fputs(port, baremetal);
				fputs(");\n", baremetal);
			}

			memset(command, '\n', sizeof(command));
		}

		curr = fgetc(raw);
	}
}

void parseLoop(FILE* raw, FILE* baremetal)
{
	fputs("\n\twhile(1) {", baremetal);
}

void parseDW(FILE* raw, FILE* baremetal)
{
	char port[3];
	int index = 0;
	char curr = fgetc(raw);
	while(curr != ',') // pin number might be one or two digits long
	{
		port[index] = curr;
		curr = fgetc(raw);
		index++;
	}

	int portNum = atoi(port);

	char value[10]; 
	fgets(value, 10, raw); // get remaining argument, should be within 10 characters

	if(strstr(value, "HIGH"))
	{
		setHigh(portNum, baremetal);
	}
	else // must be low
	{
		setLow(portNum, baremetal);
	}
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
				parseSetup(raw, baremetal);
			}
			else if(strstr(command, "loop") != NULL)
			{
				parseLoop(raw, baremetal);
			}
			else if(strstr(command, "digitalWrite") != NULL)
			{
				parseDW(raw, baremetal);
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
	
	fputs("\n\t}\n\treturn 0;\n}", baremetal);
}