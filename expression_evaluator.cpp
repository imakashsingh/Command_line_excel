#define _CRT_SECURE_NO_WARNINGS
#include "exp_eval.h"

#include "base.h"

int titleToNumber(char* s)
{
	int result = 0;
	for (int i = 0; i<strlen(s); i++)
	{
		result *= 26;
		result += s[i] - 'a' + 1;
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
	strrev(str);
	return str;
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

void store_exp_val(char *input_command, map <char*, cell> &mp)
{
	char first_part[10], second_part[20];
	int fp_len = 0, sp_len = 0;
	while ((input_command[fp_len] != ' ' && input_command[fp_len] != '=') && (fp_len < strlen(input_command)))
	{
		first_part[fp_len] = input_command[fp_len];
		fp_len++;
	}
	first_part[fp_len++] = '\0';
	//fp_len++;
	if (is_string_same(first_part, "exit"))
		exit(0);
	while (fp_len < strlen(input_command))
	{
		if (input_command[fp_len] != '=' && input_command[fp_len] != ' ')
		{
			second_part[sp_len++] = input_command[fp_len];
		}
		fp_len++;
	}
	second_part[sp_len] = '\0';

	//cout << first_part << " " << second_part << " \n";

	if (is_string_same(first_part, "view"))
	{
		cout << "Loading stored data....\n";
		view_data(mp);
		return;
	}

	bool only_digit = true;
	int str_itr = 0;
	while (second_part[str_itr] != '\0')
	{
		if (!isdigit(second_part[str_itr]))
		{
			only_digit = false;
			break;
		}
		str_itr++;
	}

	if (only_digit)
	{
		int z = get_digit(second_part);
		//cout << z;
		mp[first_part].data = z;
		mp[first_part].form = false;
	}
	else
	{
		mp[first_part].form = true;
		mp[first_part].formulae = second_part;
	}
}