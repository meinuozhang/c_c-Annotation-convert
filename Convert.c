#include<stdio.h>
#include"Convert.h"
ConvertState  Convert(FILE *input, FILE *output)
{

	assert(input);
	assert(output);
	char first, second;
	State tag = C_END;//��־��ʾ��û����cע����
	ConvertState  convert = CONVERT_SUCCESS;
	//ѭ����ȡ����
	do
	{
		first = fgetc(input);
		switch (first)
		{
		case '/':
			//1.��/*ת��Ϊ//
			second = fgetc(input);
			if (second == '*')
			{
				if (tag == C_END)//2.ע��ƥ������,���磺/*/* */
				{
					fputc('/', output);
					fputc('/', output);
					tag = C_BEGIN;//����־��Ϊcע�͵Ŀ�ʼ
				}
				else
				{
					fputc('/', output);
					fputc('*', output);
				}
			}
			//7.c++ע��Ƕ��cע��
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
				//3.ע��ĩβ�����ַ����ߴ��ڻ��е����
				char next = fgetc(input);
				if (next == '/')
				{
					fputc('\n', output);
					//4.����ע�͵����/**//**/
					fseek(input, -1, SEEK_CUR);//-1��ʾ�������
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
			if (tag == C_BEGIN)//5.����ע�����
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
	if (tag != C_END)//���Cע�͵ı�־û�б��ûس�ʼֵ����ô��ʾע�Ͳ�ƥ��
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
		printf("���е�ע����ת���ɹ�\n");
	}
	if (ret == NO_MATCH)
	{
		printf("ע�͸�ʽ��ƥ��,ת��ʧ��\n");
	}
	getchar();
	return 0;
}