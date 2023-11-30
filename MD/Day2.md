23.11.29

## 동적 라이브러리 런타임 링킹
( refer : https://blog.naver.com/sorkelf/40133647878 )<br>

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
때문에 DLL 을 사용하더라도 런타임 링킹에 초점을 두기보단, 필수적이거나 간단한 기능의 경우 암시적으로 링킹을 통해 DLL의 기능을 사용한다.
<br><br><br>


## DirectX 프로젝트 구성 & Visual Studio 초기설정

### 프로젝트 생성
- "Windows 데스크톱 애플리케이션" 프로젝트 생성
- 프로젝트에 "정적 라이브러리 프로젝트" 추가
- 필터 목록 정리

	```
	[Client]
		- Default
			- main.h/cpp
	[Engine]
		- 01.Header
			- define.h
			- global.h
			- singleton.h
			- struct.h
		- 02.Engine
		- 03.Device
		- Default
			- pch.h/cpp
	```
	▼ 솔루션 구성<br>
	<img src='./img/2_프로젝트 필터 정리.png' width=300><br>
<br><br>


### 솔루션 폴더 구성 변경
프로젝트 소스 코드들은 "프로젝트 명"의 폴더에 저장되고, 빌드 결과물은 "x64" 폴더에 저장되던 기본구조를 변경한다.<br>

▼ 기본 폴더 구성<br>
<img src='./img/2_솔루션 폴더 구성 기본.png' width=600><br>

▼ 변경된 폴더 구성<br>
<img src='./img/2_솔루션 폴더 구성 변경.png' width=1000><br>

```
External : 외부 라이브러리 저장 폴더
	- Include : *.h
	- Library : *.lib

OuputFile : 빌드 결과물(실행파일) 및 리소스 저장 폴더
	- bin : *.exe (작업 디렉터리)
	- content : 리소스

Project : 소스코드 저장 폴더
	- Client : 기본 
	- Engine : 게임 기능 구현
```
<br><br>


### Engine 프로젝트 설정

#### 1. 출력 디렉터리 경로 변경
```
설정위치 : 구성 속성 > 일반 > 일반속성/출력 디렉터리

기본 경로 : $(SolutionDir)$(Platform)\$(Configuration)\

수정 경로 : $(SolutionDir)External\Library\Engine\
```

#### 2. 출력 대상 이름 변경
```
구성 : Debug

설정위치 : 구성 속성 > 일반 > 대상 이름

기본 값 : $(ProjectName)

수정 값 : $(ProjectName)_d
```

#### 3. 작업 디렉터리 경로 변경
```
설정위치 : 구성 속성 > 일반 > 디버깅 > 작업 디렉터리

기본 경로 : $(ProjectDir)

수정 경로 : $(SolutionDir)OutputFile\bin\
```

#### 4. 라이브러리 탐색 경로 추가
```
설정위치 : 구성 속성 > 일반 > VC++ 디렉터리 > 외부 include 디렉터리, 라이브러리 디렉터리

추가 경로 : 
	- 외부 include 디렉터리 : $(SolutionDir)External\Include\
	- 라이브러리 디렉터리 	: $(SolutionDir)External\Library\
```

#### 5. 빌드 이벤트 추가
```
설정위치 : 구성 속성 > 빌드 이벤트 > 빌드 전 이벤트 > 명령줄

추가 이벤트 : 	cd $(SolutionDir)
				call EngineCopy.bat
```
<br><br>

### Client 프로젝트 설정

#### 1. 출력 디렉터리 변경
```
설정위치 : 구성 속성 > 일반 > 일반속성/출력 디렉터리

기본 경로 : $(SolutionDir)$(Platform)\$(Configuration)\

수정 경로 : $(SolutionDir)External\Library\bin\
```

#### 2. 출력 대상 이름 변경
```
구성 : Debug

설정위치 : 구성 속성 > 일반 > 대상 이름

기본 값 : $(ProjectName)

수정 값 : $(ProjectName)_d
```

#### 3. 라이브러리 탐색 경로 추가
```
설정위치 : 구성 속성 > 일반 > VC++ 디렉터리 > 외부 include 디렉터리, 라이브러리 디렉터리

추가 경로 : 
	- 외부 include 디렉터리 : $(SolutionDir)External\Include\
	- 
```

#### 4. 프로젝트 빌드 종속성 설정
```
설정위치 : 프로젝트 우클릭 > 빌드 종속성 > 프로젝트 종속성 > 종속성 > 다음에 종속

설정 값 : "다음에 종속"에서 "Engine" 선택

```
<br><br>


## 작업 디렉터리? 출력 디렉터리?

### 작업 디렉터리 (Working Directory)
- 현재 프로그램이 실행되고 있는 프로세스가 기준이 되는 디렉터리
- 일반적으로 파일을 읽거나 쓸 때, 상대 경로의 기준이 되는 디렉터리
- 일반적으로 작업 디렉터리는 소스 코드가 위치한 곳일 수 있다.

### 출력 디렉터리 (Output Directory)
- 빌드된 프로젝트의 실행 파일이나 라이브러리 등이 생성되는 디렉터리
- 개발 환경에서 코드를 컴파일하고 빌드할 때, 결과물들이 위치하는 디렉터리
- 빌드 시에 설정한 디렉터리에 실행 파일, 라이브러리 파일 등이 생성된다.

### Test
```
출력 디렉터리 경로 : $(SolutionDir)External\bin\
작업 디렉터리 경로 : $(ProjectDir)
```

```cpp
// 작업 디렉터리 경로 출력 코드

wchar_t dirPath[255] = {};
GetCurrentDirectory(255, dirPath);
```
▼ 작업 디렉터리 경로<br>
<img src='./img/2_작업 디렉터리 경로.png' width=700><br>

▼ 작업 디렉터리 폴더<br>
일반적으로 작업 디렉터리는 *.sln 또는 *.vcxproj 파일이 위치한 곳이다.
<img src='./img/2_작업 디렉터리 폴더.png' width=1000><br>


### GetCurrentDirectory( )
Windows 운영체제의 경우 해당 함수를 통해 현재 작업 디렉터리의 경로를 가져올 수 있다.
```cpp
DWORD GetCurrentDirectory(DWORD  nBufferLength, LPTSTR lpBuffer);

// nBufferLength: 가져올 디렉터리 경로의 최대 길이를 나타내는 매개변수입니다.
// lpBuffer		: 현재 작업 디렉터리의 경로를 저장할 문자열 버퍼입니다.
```