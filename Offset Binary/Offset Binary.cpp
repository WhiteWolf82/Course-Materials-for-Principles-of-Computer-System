/*Refer to the report for proof.*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

#define HALFINT pow(2, 15)
typedef unsigned int INT;


INT atom(char *s)	//change a string to offset binary
{
	INT result = 0;
	int i;
	i = (s[0] == '-') ? 1 : 0;
	while (s[i] != '\0')
	{
		result = result * 10 + s[i] - '0';
		i++;
	}
	result = (s[0] == '-') ? (HALFINT - result) : (HALFINT + result);	//change to offset binary
	return result;
}

char* mtoa(INT code)	//change an offset binary to string
{
	char *str = (char *)malloc(sizeof(char) * 100);
	int i = 0;

	//change back to true code
	if (code & 0x8000)		//code > 0
		code = code - HALFINT;
	else	//code < 0
	{
		code = HALFINT - code;
		str[i++] = '-';
	}

	sprintf(str + i, "%u", code);
	return str;
}

INT madd(INT code1, INT code2)	//add
{
	INT result = (code1 + code2) - HALFINT;
	return result;
}

INT msub(INT code1, INT code2)	//subtract
{
	INT result = (code1 - code2) + HALFINT;
	return result;
}

INT mmul(INT code1, INT code2)	//multiply
{
	INT result = code1 * code2 - 
		         HALFINT * (code1 + code2) +
		         HALFINT * HALFINT + HALFINT;
	return result;
}

INT mdiv(INT code1, INT code2)	//divide
{
	INT result = (code1 - HALFINT) / (code2 - HALFINT) + HALFINT;
	return result;
}

int main()
{
	char *str = (char *)malloc(sizeof(char) * 100);
	char *str1 = (char *)malloc(sizeof(char) * 100);
	char *str2 = (char *)malloc(sizeof(char) * 100);
	int op;
	INT code = 0;
	INT code1 = 0, code2 = 0;
	INT result = 0;

	printf("Choose the operation you want to take:\n");
	printf("1.Change a string to offset binary.\n");
	printf("2.Change an offset binary to string.\n");
	printf("3.Add.\n");
	printf("4.Subtract.\n");
	printf("5.Multiply.\n");
	printf("6.Divide.\n");
	printf("0.Exit.\n");

	scanf("%d", &op);
	while (op != 0)
	{
		switch (op)
		{
		case 1: printf("Enter a string.\n");
			scanf("%s", str);
			code = atom(str);
			printf("%X\n", code);
			break;
		case 2: printf("Enter a code.\n");
			scanf("%X", &code);
			str = mtoa(code);
			printf("%s\n", str);
			break;
		case 3: printf("Enter the first number.\n");
			scanf("%s", str1);
			code1 = atom(str1);
			printf("Enter the second number.\n");
			scanf("%s", str2);
			code2 = atom(str2);
			itoa(atoi(str1) + atoi(str2), str, 10);
			result = madd(code1, code2);
			printf("%s(%X) + %s(%X) = %s(%X)\n", str1, code1, str2, code2, str, result);
			break;
		case 4: printf("Enter the first number.\n");
			scanf("%s", str1);
			code1 = atom(str1);
			printf("Enter the second number.\n");
			scanf("%s", str2);
			code2 = atom(str2);
			itoa(atoi(str1) - atoi(str2), str, 10);
			result = msub(code1, code2);
			printf("%s(%X) - %s(%X) = %s(%X)\n", str1, code1, str2, code2, str, result);
			break;
		case 5: printf("Enter the first number.\n");
			scanf("%s", str1);
			code1 = atom(str1);
			printf("Enter the second number.\n");
			scanf("%s", str2);
			code2 = atom(str2);
			itoa(atoi(str1) * atoi(str2), str, 10);
			result = mmul(code1, code2);
			printf("%s(%X) * %s(%X) = %s(%X)\n", str1, code1, str2, code2, str, result);
			break;
		case 6: printf("Enter the first number.\n");
			scanf("%s", str1);
			code1 = atom(str1);
			printf("Enter the second number.\n");
			scanf("%s", str2);
			code2 = atom(str2);
			itoa(atoi(str1) / atoi(str2), str, 10);
			result = mdiv(code1, code2);
			printf("%s(%X) / %s(%X) = %s(%X)\n", str1, code1, str2, code2, str, result);
			break;
		case 0:
			break;
		default: printf("Invalid input!");
			break;
		}

		printf("Choose the operation you want to take:\n");
		printf("1.Change a string to offset binary.\n");
		printf("2.Change an offset binary to string.\n");
		printf("3.Add.\n");
		printf("4.Subtract.\n");
		printf("5.Multiply.\n");
		printf("6.Divide.\n");
		printf("0.Exit.\n");

		scanf("%d", &op);
	}

	system("pause");
	return 0;
}