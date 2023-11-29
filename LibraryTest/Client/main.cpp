#include <iostream>

// 정적 라이브러리
#include <math_static.h>
#pragma comment(lib, "StaticLib")

// 동적 라이브러리
#include <math_dynamic.h>
#pragma comment(lib, "DynamicLib.lib") // 암시적 링킹

int main()
{
	int data = 0;

	data = Add(100, 200);
	data = Div(100, 200);

	data = Pow(4, 3);

	return 0;
}