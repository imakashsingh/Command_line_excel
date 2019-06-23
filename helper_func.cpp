#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stack>
using namespace std;

#include "base.h"

int reverse_number(int num)
{
	int new_num = 0;
	while (num > 0)
	{
		new_num = (new_num * 10) + (num % 10);
		num /= 10;
	}
	return new_num;
}

void clear_dependency_flags(cell **matrix)
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
			matrix[i][j].dependency = false;
	}
}

bool cycle_detection(char *exp,cell **excel_data)
{
	bool result = false;
	for (int itr = 0; itr < strlen(exp); itr++)
	{
			int i = 0, col, row,flag = 0;
			char buff[20];
			strcpy(buff, "");
			while (isalpha(exp[itr]) && i < strlen(exp))
			{
				buff[i] = exp[itr];
				i++;
				itr++;
			}
			if (strlen(buff) != 0)
			{
				buff[i] = '\0';
				col = titleToNumber(buff);
			}
			else
				col = -1;
			int value = 0;
			while (itr < strlen(exp) && isdigit(exp[itr]))
			{
				buff[i] = exp[itr];
				value = value * 10 + (exp[itr] - '0');
				itr++;
				i++;
			}
			if (flag == 1)
				itr--;
			buff[i] = '\0';
			row = value - 1;
			

			if (col != -1 && excel_data[row][col].form == true)
			{
				if (excel_data[row][col].dependency == true)
					return true;
				excel_data[row][col].dependency = true;
				result = cycle_detection(excel_data[row][col].formulae,excel_data);
			}
		}
	if (result)
		return true;
	return false;
}

int titleToNumber(char* s)
{
	int result = 0;
	for (int i = 0; i<strlen(s); i++)
	{
		result *= 26;
		result += s[i] - 'a';
	}

	return result;
}

char* index_to_colname(int n)
{
	char str[10];
	int i = 0;
	while (n>0)
	{

		int rem = n % 26;
		if (rem == 0)
		{
			str[i++] = 'z';
			n = (n / 26) - 1;
		}
		else
		{
			str[i++] = (rem - 1) + 'a';
			n = n / 26;
		}
	}
	str[i] = '\0';
	_strrev(str);
	return str;
}

int operator_precedence(char operatorr)
{
	if (operatorr == '+' || operatorr == '-')
		return 1;
	else if (operatorr == '/' || operatorr == '*')
		return 2;
	else if (operatorr == '^')
		return 3;
	else
		return 0;
}

int eval_operands(int op1,int op2,char operatorr)
{
	switch (operatorr)
	{
	case '^' : return op1 ^ op2;
	case '*' : return op1 * op2;
	case '/' : return op1 / op2;
	case '+' : return op1 + op2;
	case '-' : return op1 - op2;
	}
}

int get_digit(char *str)
{
	int i = 0;
	int num = 0;
	while (str[i] != '\0')
	{
		num = (num * 10) + (str[i] - '0');
		i++;
	}
	return num;
}

bool is_string_same(char* first, char *second)
{
	if (strlen(first) != strlen(second))
		return false;
	int i = 0;
	while (first[i] == second[i] && first[i] != '\0')
		i++;
	if (i == strlen(first))
		return true;
	else
		return false;
}


char* parse_input(char *input_command)
{
	bool space_chk = false;
	char *parsed_str = (char*)malloc(100 * sizeof(char));
	int parse_str_ptr = 0;
	int i = 0;
	while (input_command[i] == ' ')
	{
		i++;
	}
	for (; i < strlen(input_command); i++)
	{
		if ((input_command[i] == ' ') && (space_chk == false))
		{
			parsed_str[parse_str_ptr++] = input_command[i];
			space_chk = true;
		}
		else if ((input_command[i] == ' ') && (space_chk))
			continue;
		else
		{
			parsed_str[parse_str_ptr++] = input_command[i];
		}
	}
	parsed_str[parse_str_ptr] = '\0';
	return parsed_str;
}


bool validate_command_and_process(char *input_command,cell **matrix)
{
	char first_part[50], second_part[50];
	int fp_len = 0, sp_len = 0;
	while ((input_command[fp_len] != ' ' && input_command[fp_len] != '=') && (fp_len < strlen(input_command)))
	{
		first_part[fp_len] = input_command[fp_len];
		fp_len++;
	}
	first_part[fp_len++] = '\0';
	while (fp_len < strlen(input_command))
	{
		if (input_command[fp_len] != ' ')
		{
			second_part[sp_len++] = input_command[fp_len];
		}
		fp_len++;
	}
	second_part[sp_len] = '\0';

	if (strcmp(first_part, "set") == 0)
	{
		set_wrapper(second_part, matrix);
		return true;
	}
	else if (strcmp(first_part, "get") == 0)
	{
		get_func(second_part, matrix,false);
		return true;
	}
	else if (strcmp(first_part, "print") == 0)
	{
		print_func(matrix,true);
		return true;
	}
	else if (strcmp(first_part, "export") == 0)
	{
		export_func(second_part,matrix);
		return true;
	}
	else if (strcmp(first_part, "import") == 0)
	{
		import_func(second_part, matrix);
		return true;
	}
	else if (strcmp(first_part, "exit") == 0)
	{
		exit(0);
	}
	else if (strcmp(first_part, "save") == 0)
	{
		char buff[15];
		print_func(matrix, false);
		printf("Enter filename");
		scanf("%s", buff);
		export_func(buff, matrix);
		return true;
	}
	else
	{
		return false;
	}

}

int expression_evaluation(char* exp,cell **matrix,int x_pos,int y_pos)
{
	stack <int> operands;
	stack <char> operatorr;
	for (int itr = 0; itr < strlen(exp); itr++)
	{
		if (exp[itr] == ' ')
			continue;
 
		else if (exp[itr] == '(')
		{
			operatorr.push(exp[itr]);
		}

		else if (exp[itr] == ')')
		{
			while (!operatorr.empty() && operatorr.top() != '(')
			{
				int value2 = operands.top();
				operands.pop();

				int value1 = operands.top();
				operands.pop();

				char op = operatorr.top();
				operatorr.pop();

				operands.push(eval_operands(value1, value2, op));
			}

			operatorr.pop();
		}

		else if (exp[itr] == '+' || exp[itr] == '-' || exp[itr] == '*'  || exp[itr] == '^' || exp[itr] == '/')
		{
			while (!operatorr.empty() && operator_precedence(operatorr.top())>= operator_precedence(exp[itr])){
				int value2 = operands.top();
				operands.pop();

				int value1 = operands.top();
				operands.pop();

				char op = operatorr.top();
				operatorr.pop();

				operands.push(eval_operands(value1, value2, op));
			}

			operatorr.push(exp[itr]);
		}

		else
		{
			int i = 0,col,row;
			char buff[20];
			strcpy(buff, "");
			while (isalpha(exp[itr]) && i < strlen(exp))
			{
				buff[i] = exp[itr];
				i++;
				itr++;
			}
			buff[i] = '\0';
			if (strlen(buff) != 0)
				 col = titleToNumber(buff);
			else
				 col = -1;
			i = 0;
			int value = 0;
			while (itr < strlen(exp) &&
				isdigit(exp[itr]))
			{
				value = value * 10 + (exp[itr] - '0');
				itr++;
			}
			itr--;
			row = value - 1;
				
			if (matrix[row][col].form == false)
				operands.push(matrix[row][col].data);
				if (matrix[row][col].form == true)
				{
					int val = expression_evaluation(matrix[row][col].formulae, matrix, row, col);
					operands.push(val);
				}

				if (col == -1)
					operands.push(value);
		}
	}
	while (!operatorr.empty()){
		int value2 = operands.top();
		operands.pop();

		int value1 = operands.top();
		operands.pop();

		char op = operatorr.top();
		operatorr.pop();

		operands.push(eval_operands(value1, value2, op));
	}

	return operands.top();
}


void set_func(char* exp,cell **excel_data)
{
	int left_row_value = 0;
	int left_col_value = 0;

	int i = 0 , cell_name_len = 0;
	char buff[50],cell_name[50];
	while (isalpha(exp[i]) && exp[i] != '\0')
	{
		cell_name[cell_name_len] = exp[i];
		buff[i] = exp[i];
		i++;
		cell_name_len++;
	}
	
	buff[i] = '\0';
	
	left_col_value = titleToNumber(buff);
	int itr = 0;
	while (isdigit(exp[i]) && exp[i] != '\0')
	{
		cell_name[cell_name_len++] = exp[i];
		buff[itr++] = exp[i++];
	}
	buff[itr] = '\0';
	cell_name[cell_name_len] = '\0';
	left_row_value = (get_digit(buff))-1;
	itr = 0;
	
	i++;
	
	while (isdigit(exp[i]) && exp[i] != '\0')
	{
		buff[itr++] = exp[i++];
	}
	buff[itr] = '\0';
	if (i == strlen(exp))
	{
		excel_data[left_row_value][left_col_value].form = false;
		excel_data[left_row_value][left_col_value].data = get_digit(buff);
	}
	else
	{
		itr = 0,i=0;
		while (exp[itr] != '=')
		{
			itr++;
		}
		itr++;
		while (exp[itr] != '\0')
		{
			buff[i] = exp[itr];
			itr++;
			i++;
		}


		buff[i] = '\0';

		excel_data[left_row_value][left_col_value].dependency = true;
		excel_data[left_row_value][left_col_value].form = true;
		bool is_there_cycle = cycle_detection(buff,excel_data);

		clear_dependency_flags(excel_data);

		if (is_there_cycle)
		{
			cout << "Cycle detected . Expression not valid.\n";
			return;
		}
		excel_data[left_row_value][left_col_value].form = true;
		excel_data[left_row_value][left_col_value].formulae = (char *)malloc(50 * sizeof(char));
		strcpy(excel_data[left_row_value][left_col_value].formulae, buff);
	}
}

void set_wrapper(char *rem_part, cell **matrix)
{
	if (rem_part[0] == '\0')
	{
		cout << "\nNo arguments passed";
		return;
	}

	set_func(rem_part , matrix);

}

int get_func(char *rem_part, cell **matrix,bool call_from_print)
{
	if (rem_part[0] == '\0')
	{
		cout << "\nNo arguments passed";
		return -1;
	}

	int left_row_value = 0;
	int left_col_value = 0;

	int i = 0;
	char buff[50];
	while (isalpha(rem_part[i]) && rem_part[i] != '\0')
	{
		buff[i] = rem_part[i];
		i++;
	}

	buff[i] = '\0';

	left_col_value = titleToNumber(buff);
	int itr = 0;
	while (isdigit(rem_part[i]) && rem_part[i] != '\0')
	{
		buff[itr++] = rem_part[i++];
	}
	buff[itr] = '\0';
	left_row_value = (get_digit(buff))-1;

	if (matrix[left_row_value][left_col_value].form == false)
	{
		if (call_from_print)
			return matrix[left_row_value][left_col_value].data;
		cout << matrix[left_row_value][left_col_value].data<<"\n";
	}
	else
	{

		int result = expression_evaluation(matrix[left_row_value][left_col_value].formulae,matrix,left_row_value,left_col_value);

		if (call_from_print)
			return result;

		cout << result << "\n";

		//cout << matrix[left_row_value][left_col_value].formulae << "\n";
	}
	return matrix[left_row_value][left_col_value].data;
}

void print_func(cell **matrix,bool call_print)
{
	if (call_print)
		printf("   ");
	for (int i = 0; i < 10; i++)
	{
		if (call_print)
			printf("%c   ", (97+i));
	}
	if (call_print)
		printf("\n");
	for (int i = 0; i < 10; i++)
	{
		if (call_print)
			printf("%d   ", i + 1);
		for (int j = 0; j < 10; j++)
		{
			if (matrix[i][j].form == true)
			{
				int itr = 0;
				char cell_name[10];
				int cell_name_length = 0;
				int row_no = i+1;
				char row_name[4];
				char* col_name = index_to_colname(j+1);
				while (cell_name_length < strlen(col_name))
				{
					cell_name[cell_name_length] = col_name[cell_name_length];
					cell_name_length++;
				}
				_itoa(row_no, row_name, 10);
				while (row_name[itr] != '\0')
				{
					cell_name[cell_name_length++] = row_name[itr++];
				}
				cell_name[cell_name_length] = '\0';
				matrix[i][j].data = get_func(cell_name,matrix,true);
				if (call_print)
					printf("%d   ", matrix[i][j].data);
			}
			else
			{
				if (call_print)
					printf("%d   ", matrix[i][j].data);
			}
		}
		if (call_print)
			printf("\n");
	}
}

void export_func(char *rem_part, cell **matrix)
{
	char *filename = (char *)malloc(15*sizeof(char));
	int i = 0;
	while (rem_part[i] != '\0')
	{
		filename[i] = rem_part[i];
		i++;
	}
	filename[i] = '\0';
	filename = strcat(filename, ".csv");

	FILE *fptr = fopen(filename, "w");

	for (int i = 0; i < 10; i++)
	{
		if (i!=0)
			fprintf(fptr, "\n");
		for (int j = 0; j < 10; j++)
		{
			if (j==0)
				fprintf(fptr, "%d", matrix[i][j].data);
			else
				fprintf(fptr, ",%d", matrix[i][j].data);
		}
	}
	fclose(fptr);
}

void import_func(char *rem_part, cell **matrix)
{
	char *filename = (char *)malloc(15 * sizeof(char));
	int i = 0;
	while (rem_part[i] != '\0')
	{
		filename[i] = rem_part[i];
		i++;
	}
	filename[i] = '\0';
	filename = strcat(filename, ".csv");

	FILE *fptr = fopen(filename,"r");
	if (fptr == NULL)
	{
		printf("Please enter a valid filename\n");
		return;
	}

	char buffer[300];
	int row = 0;
	int col = 0;
	while ((fgets(buffer,300,fptr)!=NULL) && (row < 10) && (col < 10))
	{
		char data[10];
		int i = 0,z=0;
		while ((buffer[i] != '\0') && (row < 10) && (buffer[i] != '\n'))
		{
			if(buffer[i] != ',')
			{
				data[z++] = buffer[i++];
			}
			else if (buffer[i] == ',')
			{
				data[z] = '\0';
				matrix[row][col].data = get_digit(data);
				z = 0;
				col++;
				i++;
			}
		}
		data[z] = '\0';
		matrix[row][col].data = get_digit(data);
		row++;
		col = 0;
	}
	fclose(fptr);
}
