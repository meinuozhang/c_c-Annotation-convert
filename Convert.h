#pragma once
#include<errno.h>
#include<assert.h>
//����ת��״̬
typedef enum ConvertState
{
	CONVERT_SUCCESS,
	NO_MATCH,
}ConvertState;
//����һ��ö����Ϊcע�͵ı�־
typedef enum State
{
	C_BEGIN,
	C_END,
	
}State;
ConvertState  Convert(FILE *input, FILE *output);
ConvertState  AnoConvert(char const* in, char const *out);
	