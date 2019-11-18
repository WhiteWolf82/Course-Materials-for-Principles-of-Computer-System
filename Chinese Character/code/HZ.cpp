#include<stdio.h>
#include<stdlib.h>
#include<string.h>


void find_py(char pinyin[], unsigned char buf[], FILE *py)	//find this pinyin in the file
{
	char line[100];
	char *hz;
	char data[50][5];
	int count = 0;
	int i, j;
	int choice;
	int flag = 0;

	while (fgets(line, 100, py))	//search each line to find it
	{
		if (strstr(line, pinyin) && 
		    line[0] == pinyin[0] && 
			line[strlen(pinyin) - 1] == pinyin[strlen(pinyin) - 1])	//found it
		{
			flag = 1;
			hz = strrchr(line, ':');
			hz++;
			break;
		}
	}

	if (!flag)
	{
		printf("No such pinyin!\n");
		system("pause");
		exit(0);
	}

	for (j = 1; ; j++)	//copy every character to data
	{
		if (*hz != '\n')
		{
			data[j][0] = *hz;
			hz++;
			data[j][1] = *hz;
			hz++;
			data[j][2] = '\0';

			count++;
		}
		else
			break;
	}

	for (i = 1; i <= count; i++)
	{
		printf("%d.%s ", i, data[i]);
		if (i % 10 == 0)
			printf("\n");
	}

	printf("\n");
	printf("Enter your choice.\n");
	scanf("%d", &choice);

	for (i = 0; i < 3; i++)		//copy this chosen character back to buf
		buf[i] = data[choice][i];

	fseek(py, 0L, 0);		//return original position
}

void norm_display(char tmp[][2])	//normal display
{
	int i, j, k;

	for (i = 0; i < 16; i++)
	{
		for (j = 0; j < 2; j++)
		{
			for (k = 0; k < 8; k++)
			{
				if (tmp[i][j] & (0x80 >> k))
					printf("¡ñ");
				else
					printf("¡ð");
			}
		}
		printf("\n");
	}

	printf("\n");
}

void large_display(char tmp[][2])	//enlargement display
{
	int i, j, k, m, n;
	int times;

	printf("Input enlarging times(integer).\n");
	scanf("%d", &times);

	for (i = 0; i < 16; i++)
	{
		for (m = 0; m < times; m++)
		{
			for (j = 0; j < 2; j++)
			{
				for (k = 0; k < 8; k++)
				{
					for (n = 0; n < times; n++)
					{
						if (tmp[i][j] & (0x80 >> k))
							printf("¡ñ");
						else
							printf("¡ð");
					}
				}
			}
			printf("\n");
		}
	}

	printf("\n");
}

void updown_display(char tmp[][2])	//upside-down display
{
	int i, j, k;

	for (i = 0; i < 16; i++)
	{
		for (j = 0; j < 2; j++)
		{
			for (k = 0; k < 8; k++)
			{
				if (tmp[15 - i][1 - j] & (0x80 >> (7 - k)))
					printf("¡ñ");
				else
					printf("¡ð");
			}
		}
		printf("\n");
	}

	printf("\n");
}

void incline_display(char tmp[][2])		//incline display
{
	int i, j, k, m;
	int degree;

	printf("Input inclining degree(integer).\n");
	scanf("%d", &degree);

	for (i = 0; i < 16; i++)
	{
		for (m = 0; m < (16 - i) * degree; m++)
			printf(" ");
		for (j = 0; j < 2; j++)
		{
			for (k = 0; k < 8; k++)
			{
				if (tmp[i][j] & (0x80 >> k))
					printf("**");
				else
					printf("  ");
			}
		}
		printf("\n");
	}
}

int main()
{
	FILE *fp1 = fopen("HZK16", "rb");
	FILE *fp2 = fopen("mhzk16", "rb");
	FILE *fp3 = fopen("py.txt", "r");

	int op, op1, op2;
	unsigned char buf1[10], buf2[10];
	char tmp[16][2];
	char pinyin[10];
	unsigned int order, position;

	printf("1. Chinese character	2. English character	0. Exit\n");
	scanf("%d", &op2);

	if (op2 == 0)
		exit(0);

	printf("Select the operation you want to take:\n");
	printf("1. Normal display.\n");
	printf("2. Enlargement display.\n");
	printf("3. Upside-down display.\n");
	printf("4. Incline display.\n");
	scanf("%d", &op);

	if (op >= 1 && op <= 4)		//valid operation
	{
		if (op2 == 1)	//Chinese character
		{
			printf("Select the code you want to use.\n");
			printf("1. GB code	2. ZB code\n");
			scanf("%d", &op1);

			if (op1 != 1 && op1 != 2)
			{
				printf("Invalid input!\n");
				system("pause");
				exit(0);
			}

			printf("Enter the pinyin of a Chinese character.\n");
			scanf("%s", pinyin);

			find_py(pinyin, buf1, fp3);	//find this pinyin in the file

			if (op1 == 1)	//GB
			{
				order = 94 * (buf1[0] - 0XA1) + (buf1[1] - 0XA1);		//find the order of this character
				position = order * 32;		//a Chinese character has 32 bytes
				fseek(fp1, position, 0);	//find this position in the file
				fread(tmp, 32, 1, fp1);	//read the information of this character
				fseek(fp1, 0L, 0);		//return original position
			}
			else if (op1 == 2)	//ZB
			{
				order = 94 * (buf1[0] - 0XA1) + (buf1[1] - 0XA1) + 256;	//find the order of this character
				position = order * 32;		//a Chinese character has 32 bytes
				fseek(fp2, position, 0);	//find this position in the file
				fread(tmp, 32, 1, fp2);	//read the information of this character
				fseek(fp2, 0L, 0);		//return original position
			}
		}
		else if (op2 == 2)	//English character
		{
			printf("Enter an English character:\n");
			scanf("%s", buf2);

			order = buf2[0];	//find the order of this character
			position = order * 32;		//a Chinese character has 32 bytes
			fseek(fp2, position, 0);	//find this position in the file
			fread(tmp, 32, 1, fp2);	//read the information of this character
			fseek(fp2, 0L, 0);		//return original position
		}
	}

	while (1)
	{
		switch (op)
		{
		case 1: norm_display(tmp);
			break;
		case 2: large_display(tmp);
			break;
		case 3:	updown_display(tmp);
			break;
		case 4: incline_display(tmp);
			break;
		default: printf("Invalid input!\n");
			break;
		}

		printf("1. Chinese character	2. English character	0. Exit\n");
		scanf("%d", &op2);

		if (op2 == 0)
			break;

		printf("Select the operation you want to take:\n");
		printf("1. Normal display.\n");
		printf("2. Enlargement display.\n");
		printf("3. Upside-down display.\n");
		printf("4. Incline display.\n");
		scanf("%d", &op);

		if (op >= 1 && op <= 4)		//valid operation
		{
			if (op2 == 1)	//Chinese character
			{
				printf("Select the code you want to use.\n");
				printf("1. GB code	2. ZB code\n");
				scanf("%d", &op1);

				if (op1 != 1 && op1 != 2)
				{
					printf("Invalid input!\n");
					system("pause");
					exit(0);
				}

				printf("Enter the pinyin of a Chinese character.\n");
				scanf("%s", pinyin);

				find_py(pinyin, buf1, fp3);	//find this pinyin in the file

				if (op1 == 1)	//GB
				{
					order = 94 * (buf1[0] - 0XA1) + (buf1[1] - 0XA1);		//find the order of this character
					position = order * 32;		//a Chinese character has 32 bytes
					fseek(fp1, position, 0);	//find this position in the file
					fread(tmp, 32, 1, fp1);	//read the information of this character
					fseek(fp1, 0L, 0);		//return original position
				}
				else if (op1 == 2)	//ZB
				{
					order = 94 * (buf1[0] - 0XA1) + (buf1[1] - 0XA1) + 256;	//find the order of this character
					position = order * 32;		//a Chinese character has 32 bytes
					fseek(fp2, position, 0);	//find this position in the file
					fread(tmp, 32, 1, fp2);	//read the information of this character
					fseek(fp2, 0L, 0);		//return original position
				}
			}
			else if (op2 == 2)	//English character
			{
				printf("Enter an English character:\n");
				scanf("%s", buf2);

				order = buf2[0];	//find the order of this character
				position = order * 32;		//a Chinese character has 32 bytes
				fseek(fp2, position, 0);	//find this position in the file
				fread(tmp, 32, 1, fp2);	//read the information of this character
				fseek(fp2, 0L, 0);		//return original position
			}
		}
	}

	fclose(fp1);
	fclose(fp2);
	fclose(fp3);

	system("pause");
	return 0;
}
