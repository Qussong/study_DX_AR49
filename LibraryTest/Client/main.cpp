#include <iostream>
#include <Windows.h>	// HMODULE

// 정적 라이브러리
#include <math_static.h>
#pragma comment(lib, "StaticLib")

// 동적 라이브러리
#include <math_dynamic.h>
//#pragma comment(lib, "DynamicLib.lib") // 암시적 링킹

int main()
{
	int data = 0;

	data = Add(100, 200);
	data = Div(100, 200);

	//data = Pow(4, 3);	// 암시적 링킹

	// 실행파일(작업디렉터리) 경로에 있는 dll을 로드해서 링크한다.
	HMODULE hDllHandle = LoadLibrary(L"DynamicLib.dll");	// 성공
	//HMODULE hDllHandle = LoadLibrary(L"DynamicLib1.dll");	// 실패


	// 동적 라이브러리 로드 실패하면 프로그램 종료
	if (nullptr == hDllHandle)
		return 0;

	// 반환타입이 int, 인자가 int 2개인 함수 포인터 타입을 정의한다.
	typedef int(*FUNC_TYPE)(int, int);
	
	// 반환타입이 int, 인자가 int 2개인 함수 포인터타입 변수를 선언
	FUNC_TYPE pFunc = nullptr;

	pFunc = (FUNC_TYPE)GetProcAddress(hDllHandle, "Pow");

	if (nullptr != pFunc)
	{
		// Pow 함수를 호출
		data = pFunc(4, 3);
	}

	if (nullptr != hDllHandle)
	{
		FreeLibrary(hDllHandle);
		hDllHandle = nullptr;
	}


	return 0;
}