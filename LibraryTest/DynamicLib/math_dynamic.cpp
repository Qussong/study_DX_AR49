#include "pch.h"
#include "math_dynamic.h"

int Pow(int a, int b)
{
	int iReturn = 1;

	for (int i = 0; i < b; ++i)
	{
		iReturn *= a;
	}

	return iReturn;
}