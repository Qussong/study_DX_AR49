#include <iostream>

// ���� ���̺귯��
#include <math_static.h>
#pragma comment(lib, "StaticLib")

// ���� ���̺귯��
#include <math_dynamic.h>
#pragma comment(lib, "DynamicLib.lib") // �Ͻ��� ��ŷ

int main()
{
	int data = 0;

	data = Add(100, 200);
	data = Div(100, 200);

	data = Pow(4, 3);

	return 0;
}