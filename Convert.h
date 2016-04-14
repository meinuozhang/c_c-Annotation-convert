#pragma once
#include<errno.h>
#include<assert.h>
//定义转化状态
typedef enum ConvertState
{
	CONVERT_SUCCESS,
	NO_MATCH,
}ConvertState;
//定义一个枚举作为c注释的标志
typedef enum State
{
	C_BEGIN,
	C_END,
	
}State;
ConvertState  Convert(FILE *input, FILE *output);
ConvertState  AnoConvert(char const* in, char const *out);
	