23.11.29

## 동적 라이브러리 런타임 링킹
refer : https://blog.naver.com/sorkelf/40133647878<br>

### DLL 사용 방식
1) 묵시적 링킹 :<br>
    실행 파일 자체에 사용하고자 하는 DLL과 함수에 대한 정보를 포함시키고, OS가 프로그램 실행 시 해당 함수들을 초기화한 후 이용하는 방식<br>
    ```cpp
    #pragma comment(lib, "library 이름");
    ```
2) 명시적 링킹 :<br>
    프로그램 실행 중 API를 이용하여 DLL 파일의 유무를 검사하고, 동적으로 원하는 함수만 불러와 사용하는 방식<br>
    ```cpp
    LoadLibrary();
    GetProcAddress();
    FreeLibrary();
    ```

### 예제 코드
```cpp
#include <iostream>
#include <Windows.h>	// HMODULE

// 동적 라이브러리
#include <math_dynamic.h>

int main()
{
	int data = 0;

	// 실행파일(작업디렉터리) 경로에 있는 dll을 로드해서 링크한다.
	HMODULE hDllHandle = LoadLibrary(L"DynamicLib.dll");

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
```

### LoadLibrary(LPCWSTR)
```cpp
// libloaderapi.h
HMODULE LoadLibraryW(LPCWSTR lpLibFileName);
#ifdef UNICODE
#define LoadLibrary  LoadLibraryW
```
지정한 DLL을 현재 프로세스의 주소 공간으로 mapping 하여 사용할 수 있도록 해준다.<br>
지정된 DLL을 찾을 수 없으면 nullptr 을 반환<br>

### GetProcAddress()
```cpp
// libloaderapi.h
FARPROC GetProcAddress(HMODULE hModule, LPCSTR lpProcName);
```
DLL 에서 export 한 함수의 주소를 찾아서 함수의 포인터를 반환해준다.<br>
지정된 함수를 찾지 못하면 nullptr을 반환한다.<br>

### FreeLibrary()
```cpp
// libloaderapi.h
BOOL FreeLibrary(HMODULE hLibModule);
```
지정된 DLL의 참조 수를 -1 시키고, 참조 수가 0 이 되면 호출 프로세스의 메모리 공간에서 언로드된다.<br>
힘수가 성공하면 반환값이 0이 아니다.<br>

### etc
위의 예제 코드에서 보듯, DLL 을 명시적 링킹으로 사용하기엔 복잡한감이 없잖아 있다.<br>
때문에 DLL 을 사용하더라도 런타임 링킹에 초점을 두기보단, 필수적이거나 간단한 기능의 경우 암시적으로 링킹을 통해 DLL의 기능을 사용한다.<br><br>

## DirectX를 위한 VS 초기설정