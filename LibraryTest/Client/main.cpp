#include <iostream>
#include <Windows.h>	// HMODULE

// ���� ���̺귯��
#include <math_static.h>
#pragma comment(lib, "StaticLib")

// ���� ���̺귯��
#include <math_dynamic.h>
//#pragma comment(lib, "DynamicLib.lib") // �Ͻ��� ��ŷ

int main()
{
	int data = 0;

	data = Add(100, 200);
	data = Div(100, 200);

	//data = Pow(4, 3);	// �Ͻ��� ��ŷ

	// ��������(�۾����͸�) ��ο� �ִ� dll�� �ε��ؼ� ��ũ�Ѵ�.
	HMODULE hDllHandle = LoadLibrary(L"DynamicLib.dll");	// ����
	//HMODULE hDllHandle = LoadLibrary(L"DynamicLib1.dll");	// ����


	// ���� ���̺귯�� �ε� �����ϸ� ���α׷� ����
	if (nullptr == hDllHandle)
		return 0;

	// ��ȯŸ���� int, ���ڰ� int 2���� �Լ� ������ Ÿ���� �����Ѵ�.
	typedef int(*FUNC_TYPE)(int, int);
	
	// ��ȯŸ���� int, ���ڰ� int 2���� �Լ� ������Ÿ�� ������ ����
	FUNC_TYPE pFunc = nullptr;

	pFunc = (FUNC_TYPE)GetProcAddress(hDllHandle, "Pow");

	if (nullptr != pFunc)
	{
		// Pow �Լ��� ȣ��
		data = pFunc(4, 3);
	}

	if (nullptr != hDllHandle)
	{
		FreeLibrary(hDllHandle);
		hDllHandle = nullptr;
	}


	return 0;
}