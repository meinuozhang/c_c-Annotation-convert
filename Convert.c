#include<stdio.h>
#include"Convert.h"
ConvertState  Convert(FILE *input, FILE *output)
{

	assert(input);
	assert(output);
	char first, second;
	State tag = C_END;//标志表示还没有在c注释中
	ConvertState  convert = CONVERT_SUCCESS;
	//循环读取数据
	do
	{
		first = fgetc(input);
		switch (first)
		{
		case '/':
			//1.将/*转换为//
			second = fgetc(input);
			if (second == '*')
			{
				if (tag == C_END)//2.注释匹配的情况,形如：/*/* */
				{
					fputc('/', output);
					fputc('/', output);
					tag = C_BEGIN;//将标志置为c注释的开始
				}
				else
				{
					fputc('/', output);
					fputc('*', output);
				}
			}
			//7.c++注释嵌套c注释
			else if (second == '/')
			{
				char next;
				fputc('/', output);
				fputc('/', output);
				do
				{
					next = fgetc(input);
					fputc(next, output);
					if (next == EOF)
					{
						return convert;
					}
				} while (next != '\n');
			}
			else
			{
				fputc(first, output);
				fputc(second, output);
			}
			break;
		case '*':
			second = fgetc(input);
			if (second == '/')
			{
				//3.注释末尾还有字符或者存在换行的情况
				char next = fgetc(input);
				if (next == '/')
				{
					fputc('\n', output);
					//4.连续注释的情况/**//**/
					fseek(input, -1, SEEK_CUR);//-1表示负向回退
				}
				else if (next != '\n'&& next != EOF)
				{
					fputc('\n', output);
					fputc(next, output);
				}
				else
				{
					fputc('\n', output);
				}
				tag = C_END;
			}
			else if (second == '*')
			{
				fputc(first, output);
				fseek(input, -1, SEEK_CUR);
			}
			else
			{
				fputc(first, output);
				fputc(second, output);
			}
			break;
		case '\n':
			fputc('\n', output);
			if (tag == C_BEGIN)//5.多行注释情况
			{
				fputc('/', output);
				fputc('/', output);
			}
			break;
		default:
			fputc(first, output);
			break;
		}
	} while (first != EOF);
	if (tag != C_END)//如果C注释的标志没有被置回初始值，那么表示注释不匹配
	{
		convert = NO_MATCH;
	}
	return convert;
}
ConvertState  AnoConvert(char const* in, char const *out)
{
	FILE *FIN, *FOUT;

	FIN = fopen("input.c", "r");
	if (FIN == NULL)
	{
		printf("open in failed \n");
		return;
	}
	FOUT = fopen("output.c", "w");
	if (FOUT == NULL)
	{
		fclose(FIN);
		printf("open out failed errno");
		return;
	}
	ConvertState  convert;
	convert = Convert(FIN, FOUT);
	fclose(FIN);
	fclose(FOUT);
	return convert;
}

int main()
{
	ConvertState  ret;
	ret=AnoConvert("input.c","output.c");
	if (ret == SUCCESS)
	{
		printf("所有的注释已转换成功\n");
	}
	if (ret == NO_MATCH)
	{
		printf("注释格式不匹配,转化失败\n");
	}
	getchar();
	return 0;
}