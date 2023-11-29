23.11.28  

## 라이브러리
- 라이브러리는 코드 모음집이다.  
- 라이브러리를 통해 이전에 만들어둔 코드들을 다른 프로그램에 쉽게 적용할 수 있다.  
- 라이브러리화 된 필수 기능들이 있다면 이를 통해 개발을 더 빠르고 효율적으로 할 수 있다. (분리처리)  
- 코드를 공개하지 않고 배포할 수 있다.  
- 장점 : 재사용성 증가 , 유지보수 및 업데이트 용이성  
<br>

## 정적 vs 동적

정적 라이브러리와 동적 라이브러리 둘 다 코드를 `모듈화`하고 재사용하기위해 사용한다.

### 정적 라이브러리 (Static Library)
- 컴파일 시점에 프로그램에 결합되는 라이브러리  
- 컴파일러가 소스 코드를 기계어로 변환할 때 라이브러리의 코드가 실행 파일안에 포함된다.  
- 라이브러리의 코드가 포함됨으로인해 실행 파일의 크기가 커질 수 있다.  
- 프로그램 배포시 라이브러리가 포함되기에 외부 종속성에 대한 걱정이 없으며, 이식성이 높다.

### 동적 라이브러리 (Dynamic Library)
- 프로그램이 실행될 때 필요한 시점에 메모리에 로드되어 사용되는 라이브러리  
- 실행 파일과 별도로 관리되며, 동적으로 로드되어 사용된다.  
- 메모리에 이미 로드되어있다면 추가적인 로딩 과정없이 사용할 수 있다.
- 이를 통해 시스템 자원을 효율적으로 활용하고 중복 메모리 사용을 최소화 할 수 있다.  
<br>

## 실습_정적 라이브러리 생성

### 1. 정적 라이브러리 프로젝트 추가
아래의 방법으로 솔루션에 정적 라이브러리 프로젝트를 추가한다.<br>
<img src="./img/1_정적 라이브러리 프로젝트 추가.png" width=1000><br><br>
솔루션에 프로젝트를 추가하게되면 솔루션 폴더의 구성은 아래와 같아진다.<br>
<img src="./img/1_프로젝트 폴더 구성.png" width=500><br><br>
정적 라이브러리 빌드 진행시 `솔루션 구성(Debug,Release)`에 따라서 아래의 폴더들에 빌드 결과물이 생성된다.<br>
<img src="./img/1_정적 라이브러리 빌드 결과물.png" width=500><br><br>

### 2. 정적 라이브러리 프로젝트에 `배포하려는 기능(사칙연산)` 구현
```cpp
// math_static.h

#pragma once

int Add(int a, int b);
int Sub(int a, int b);
int Mul(int a, int b);
int Div(int a, int b);
```

```cpp
// math_static.cpp

#include "pch.h"
#include "math_static.h"

int Add(int a, int b) {
	return a + b;
}

int Sub(int a, int b) {
	return a - b;
}

int Mul(int a, int b) {
	return a * b;
}

int Div(int a, int b) {
	return a / b;
}
```
위와 같이 코드를 작성한 후 빌드하면 결과물로 *.lib 파일이 생성된다.<br>
<img src="./img/1_lib 파일 생성.png" width=400><br><br>
:warning: **구현부에 pch.h 미포함시 *.lib 파일이 결과물로 나오지 않는다.**<br>
<img src="./img/1_pch.h 미포함시.png" width=1000><br><br>

### 3. 정적 라이브러리 사용하기
만들어낸 정적 라이브러리를 사용하기 위해 Client 프로젝트에 아래 코드를 추가해준다.
```cpp
// main.cpp

#include <math_static.h> // 꺽쇠 괄호로 묶을시 프로젝트내의 경로에서 탐색한다.
#pragma comment(lib, "StaticLib.lib") // lib 파일 명에서 뒤의 확장자는 생략 가능하다.
```
하지만 아래와 같이 라이브러리를 인식하지 않음을 확인할 수 있다.<br>
<img src="./img/1_정적 라이브러리 인식 안함.png" width=400><br><br>

### 4. Visaul Studio 프로젝트 설정
정적 라이브러리를 사용하기 위해선 *.lib, *.h 파일을 참조하는 경로 설정을 해줘야한다.<br>
편의를 위해 외부 라이브러리를 모아둘 `External` 폴더를 솔루션 폴더에 추가한다.<br>
이후 아래의 프로젝트 속성에 아래의 설정을 해준다.<br>
```
설정 위치 : 프로젝트 속성 → VC++ 디렉터리 → 외부 include 디렉터리, 라이브러리 디렉터리

추가 경로 : $(SolutionDir)External\
```
<img src="./img/1_프로젝트 라이브러리 탐색 경로 추가.png" width=800><br>
추가 경로가 성공적으로 설정되면 아래와 같이 입력해준 경로가 추가됐음을 확인 할 수 있다.<br>
<img src="./img/1_라이브러리 경로 추가.png" width=800><br><br>
위의 설정을 마친 후 External 폴더에 사용하고자하는 *.lib, *.h 파일을 옮겨준다.<br>
<img src="./img/1_정적라이브러리 사용 세팅.png" width=400><br><br>

### 5. 종속성
Client 가 StaticLib 의 라이브러리 파일을 가져다 쓰면 Client 가 StaticLib 에 `종속성`을 가진다고 볼 수 있다. (빌드 순서 잡아줘야한다.)  
아래의 설정을 모두 마치면 정적 라이브러리 프로젝트에서 작업한 내용을 다시 빌드하기만 하면 바로 클라이언트에 적용시킬 수 있다.  
```
[ Do List ]
1. 빌드 전에 정적 라이브러리 프로젝트에 생성해준 header 파일을 External 폴더로 옮겨줘야한다.
	이를 위한 *.bat 파일을 생성한다.

2. 생성한 *.bat 파일을 프로젝트 속성 중 하나인 "빌드 전 이벤트"에 설정하여 빌드전 사전작업으로 이루어질 수 있도록 해준다. (정적 라이브러리 프로젝트에서 작업)

3. 정적 라이브러리 프로젝트의 빌드시 출력 디렉터리 설정을 변경해준다. 

4. 정적 라이브러리를 사용하는 클라이언트 프로젝트 속성에서 종속설정을 해준다.
```
<br>

**[ 1. *.bat 파일 생성 ]**<br>
메모장에 아래와 같은 코드를 입력하고 *.bat 확장자로 저장하면 된다.  
`exclude:exclude_list.txt` : "exclude_list.txt" 파일을 생성하여 해당 파일안에 제외하고자하는 파일이 있으면 추가해주면 된다.
```
xcopy /s /y  /exclude:exclude_list.txt ".\StaticLib\*.h" ".\External"
```
<br>

**[ 2. 빌드 전 이벤트 설정 ]**<br>
정적 라이브러리 프로젝트에서 해당 설정을 해준다.  
정적 라이브러리 빌드 전 생성한 *.bat 파일이 실행되면서 *.h 파일을 지정한 경로(External 파일)로 옮겨준다.
```
설정 위치 : 정적 라이브러리 프로젝트 속성 → 빌드 이벤트 → 빌드 전 이벤트 → 명령줄

추가 값 : 	cd $(SolutionDir)
			call Copy.bat
```
<img src="./img/1_정적 라이브러리 프로젝트 빌드 전 이벤트 설정.png" width=800><br><br>

**[ 3. 출력 디렉터리 설정 ]**<br>
정적 라이브러리 프로젝트의 헤더파일이 External 폴더로 옮겨지듯 *.lib 파일도 옮겨져야한다.  
lib 파일은 정적 라이브러리 프로젝트 빌드시 생성되기에 출력 디렉터리 위치 설정을 변경해줌으로써 이동시켜준다.<br>
<img src="./img/1_출력 디렉터리 설정.png" width=800><br><br>

**[ 4. 종속성 설정 ]**<br>
```
설정 위치 : 프로젝트 우클릭 → 빌드 종속성 → 프로젝트 종속성

"종속성 탭"에서 해당 프로젝트가 종속성을 가지는 프로젝트명을 체크해준다.
```
위의 과정을 거치면 아래와 같이 프로젝트 빌드 순서가 변경되었음을 확인 할 수 있다.<br>
<img src="./img/1_프로젝트 빌드 종속성 설정.png" width=1000><br><br>

## 실습_동적 라이브러리 생성

### 1. 동적 라이브러리 프로젝트 추가
아래의 방법으로 솔루션에 정적 라이브러리 프로젝트를 추가한다.<br>
<img src="./img/1_동적 라이브러리 프로젝트 생성.png" width=1000><br><br>

### 2. 동적 라이브러릴 프로젝트에 `배포하려는 기능(pow)`구현
코드를 작성한 후 빌드하면 결과물로 *.lib, *.dll 파일이 생성된다.<br>
```cpp
// math_dynamic.h

#pragma once

#ifdef DYNAMICLIB_EXPORTS
#define MY_DLL extern "C" __declspec(dllexport)
#else
#define MY_DLL extern "C" __declspec(dllimport)
#endif

MY_DLL int Pow(int a, int b);
```
```cpp
// math_dynamic.cpp

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
```
<br>


### 3. 동적 라이브러리 사용하기
```cpp
// 동적 라이브러리
#include <math_dynamic.h>
#pragma comment(lib, "DynamicLib.lib") // 암시적 링킹
```
<br>

### 4. Visual Studio 프로젝트 설정
클라이언트 프로젝트가 동적 라이브러리를 참조할 경로를 프로젝트 속성에서 설정해줘야한다.  
이미 정적 라이브러리를 참조하기위해 External 폴더를 참조하도록 했기에 생략한다. 
```
설정 위치 :
	프로젝트 속성 → VC++ 디렉터리 → 외부 include 디렉터리, 라이브러리 디렉터리
```
<br>

### 5. *.dll 파일
정적 라이브러리와 차이점이 있다면 동적 라이브러리 프로젝트는 빌드시 *.lib, *dll 파일들을 출력한다는 점이다.  
출력 디렉터리를 External 폴더로 해줬기에 *.dll 파일도 여기 있을텐데, *.dll 파일은 프로그램 실행파일 경로에 있어야한다.<br>
<img src="./img/1_동적 라이브러리 빌드 결과물.png" width=1000><br><br>
dll 파일이 위와 같이 External 폴더에 위치해 있으면 프로그램 실행시 오류가 발생한다.<br>
<img src="./img/1_dll 파일을 찾을 수 없음.png" width=1000><br><br>
때문에 dll 파일을 실행파일(exe) 위치로 옮겨줘야한다.<br>
<img src="./img/dll파일 실행파일 위치로 옮김.png" width=500><br><br>