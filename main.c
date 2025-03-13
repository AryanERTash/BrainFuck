#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ARRAY_LENGTH 30000

char ARRAY[ARRAY_LENGTH]; // Memory array
int ARRAY_POINTER = 0;	  // Memory pointer
int INS_PTR = 0;		  // Instruction pointer

void evaluate_bf(char *expression);
char *read_file(const char *filename);

void evaluate_bf(char *expression)
{
	char current;
	int loop;

	while ((current = expression[INS_PTR]) != 0)
	{
		switch (current)
		{
		case '>':
			ARRAY_POINTER++;
			break;
		case '<':
			ARRAY_POINTER--;
			break;
		case '+':
			ARRAY[ARRAY_POINTER]++;
			break;
		case '-':
			ARRAY[ARRAY_POINTER]--;
			break;
		case '.':
			putchar(ARRAY[ARRAY_POINTER]);
			fflush(stdout);
			break;
		case ',':
			ARRAY[ARRAY_POINTER] = (char)getchar();
			break;
		case '[':
			if (ARRAY[ARRAY_POINTER] == 0)
			{
				loop = 1;
				while (loop > 0)
				{
					INS_PTR++;
					if (expression[INS_PTR] == '[')
						loop++;
					else if (expression[INS_PTR] == ']')
						loop--;
				}
			}
			break;
		case ']':
			if (ARRAY[ARRAY_POINTER] != 0)
			{
				loop = 1;
				while (loop > 0)
				{
					INS_PTR--;
					if (expression[INS_PTR] == '[')
						loop--;
					else if (expression[INS_PTR] == ']')
						loop++;
				}
				INS_PTR--;
			}
			break;
		}
		INS_PTR++;
	}
}

char *read_file(const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (!file)
	{
		fprintf(stderr, "Error: Could not open file %s\n", filename);
		exit(EXIT_FAILURE);
	}

	fseek(file, 0, SEEK_END);
	long length = ftell(file);
	rewind(file);

	char *buffer = (char *)malloc(length + 1);

	fread(buffer, 1, length, file); // read all at once

	buffer[length] = '\0';
	fclose(file);

	return buffer;
}

int main(int argc, char const *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <brainfuck_file.bf>\n", argv[0]);
		return EXIT_FAILURE;
	}

	memset(ARRAY, 0, ARRAY_LENGTH);

	char *code = read_file(argv[1]);

	evaluate_bf(code);
	putchar('\n');
	free(code);

	return EXIT_SUCCESS;
}
