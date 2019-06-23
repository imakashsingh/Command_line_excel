#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
using namespace std;
#include "base.h"

int main()
{
	char input_from_user[200];
	cell **user_matrix = (cell **)malloc(10 * sizeof(cell*));
	for (int i = 0; i < 10; i++)
	{
		user_matrix[i] = (cell *)malloc(10 * sizeof(cell));
	}
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			user_matrix[i][j].data = 0;
			user_matrix[i][j].form = false;
			user_matrix[i][j].dependency = false;
			user_matrix[i][j].formulae = NULL;
		}
	}
	do
	{
		printf(">");
		fgets(input_from_user, 200, stdin);
		if (strlen(input_from_user) == 0)
		{
			continue;
		}
		input_from_user[strlen(input_from_user)-1] = '\0';
		char *parsed_output = parse_input(_strlwr(input_from_user));
		bool is_valid_command = validate_command_and_process(parsed_output,user_matrix);
		if (!is_valid_command)
			printf("\nInvalid command");
	} while (1);
	system("pause");
	return 0;
}
