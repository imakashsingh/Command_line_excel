#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
using namespace std;

typedef struct cell
{
	int data;
	bool form;
	char *formulae;
}cell;

char* parse_input(char *input_command);

bool is_string_same(char* first, char *second);

void store_exp_val(char *input_command, map <char*, cell> &mp);

void view_data(map <char*, cell> &mp);

int titleToNumber(char* s)