#include <iostream>
#include <stdlib.h>
#include <string.h>


typedef struct cell
{
	int data;
	bool form;
	bool dependency;
	char *formulae;
}cell;


char* parse_input(char *input_command);

bool validate_command_and_process(char *input_command,cell **matrix);

bool check_if_present_in_string(char **arr, int arr_len,char *str);

void set_wrapper(char *rem_part, cell **matrix);

int get_func(char *rem_part, cell **matrix, bool call_from_print);

int reverse_number(int num);

void print_func(cell **matrix,bool call_print);

void export_func(char *rem_part, cell **matrix);

void import_func(char *rem_part, cell **matrix);

int titleToNumber(char* s);

char* index_to_colname(int n);

int get_digit(char *str);

bool is_string_same(char* first, char *second);

void set_func(char* exp, cell **excel_data);

int operator_precedence(char operatorr);

int eval_operands(int op1, int op2, char operatorr);

int expression_evaluation(char* exp, cell **matrix, int x_pos, int y_pos);

bool cycle_detection(char *exp, char **arr, int arr_len, cell **excel_data);

void clear_dependency_flags(cell **matrix);