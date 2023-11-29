
## 공부
- friend 키워드
    ```
    friend 키워드를 사용하면 특정 대상에 대해 모든 멤버를 공개할 수 있다.
    -> friend 를 선언한 대상에게 내 모든 것을 공개한다.

    friend 지정되면 '접근 지정자'에 상관없이 모든 멤버를 읽을 수 있다.
    friend는 전역 함수, 클래스, 멤버 함수 세 가지 수준에서 지정한다.

    1. 외부 전역 함수에 모두 공개
    외부의 전역 함수를 friend로 지정할 때 클래스 선언문 안에 원형을 밝히되 friend 지정자를 붙인다.

    class Some
    {
        friend void sub();
        //...
    }
    Some 클래스의 멤버 변수들은 모두 friend 인 sub()함수가 접근할 수 있다.
    sub()함수가 Some 클래스에 friend 선언되어 있다는 의미지, 실제 sub()함수는 Some 클래스 밖에 존재하고 있다.

    2. 

    ```
- <stdlib.h> atexit()
- MessageBox()
- ComPtr

## 질문
동적 라이브러리 암시적 링크로 사용할 경우 dll 파일을 실행파일 경로로 옮겨줘야하는데 
이에 대한 bat 파일을 "동적 라이브러리 프로젝트"의 "빌드 이벤트/빌드 후 작업"에 넣어줘도 되는가?


## 동적 라이브러리 런타임 도중에 실시간으로 링킹하여 함수 호출하는 과정
```
// HMODULE 타입
HMODULE hDllHandle = LoadLibrary(L"DynamicLib.dll");    // 동적 라이브러리 핸들 받아옴

typedef int(*FUNC_TYPE)(int, int);  // 함수 포인트타입 선언
FUNC_TYPE pFunc = nullptr;

pFunc = (FUNC_TYPE)GetProcAddress(hDllHandle, "Pow");   // 로드된 dll 라이브러리에서 pow 함수를 찾아서 주소를 반환

pFunc(4, 3);    // Pow 함수 호출한것과 동일함

FreeLibrary(hDllHandle);    //동적 라이브러리 링크 해제
```

위의 과정을 보면 사용하기 힘듦을 알 수 있다.  
때문에 dll 을 사용하더라도 실시간에 초점을 두기보단, 필수적이거나 간단한 함수의 경우 암시적으로 링크를 걸어서 사용한다.  

언어관련 기능을 사용할 때, 이를 dll로 관리할 수 있다. (동적)  
모든 언어를 한 프로젝트에 다 넣어버리면 실행파일이 너무 커진다.  
때문에 국가에 따라 필요한 언어를 dll을 통해서 동적할당 하면 깔끔하다.  

## 수업

### 솔루션 생성
"Window 데스크톱 애플리케이션"으로 솔루션 생성  
```
솔루션 명 : DirectX49
프로젝트 명 : Client
```

### 솔루션 정리
- 소스, 헤더 필터 제거
- client.cpp -> main.cpp 로 파일명 변경
- 정적 라이브러리 프로젝트 추가 (프로젝트 명 : "Engine")
- Engine 에 필터(Default) 추가
- Project 폴더에 Client, Engine 폴더 위치 옮기기 (Project 파일 따로 관리하기 위함)
- x64 에 빌드되는 경로 변경한다.
    ```
    [CLinet]
    Platform = x64

    Output 폴더 생성
        - bin 폴더 생성(빌드 )
        - content 폴더 생성

    출력 디렉터리 변경 '모든 구성' (경로 : Output/bin) - 출력물 관리
    디버깅/작업 디렉터리 설정 - Current Directory 설정

    일반 -> "대상이름" 에서 Debug 의 겨우 뒤에 "_d" 를 붙여주는 걸로 변경

    [Engine]
    라이브러리 폴더 생성하여 출력 디렉터리 설정해준다.

    [bat 파일]
    빌드 이벤트에 사용할 *.bat파일 과 exclude_list.txt 파일 추가

    일반 -> "대상이름" 에서 Debug 의 겨우 뒤에 "_d" 를 붙여주는 걸로 변경
    ```

### CEngine 클래스 
- 정적 라이브러리 프로젝트에 Engine 클래스 생성
- CEngine 클래스 SingleTon 으로 만든다. (Template을 상속해서 만들어본다)
    ```cpp
    // singleton.h

    // atexit() : 
    //      메인함수 종료시 호출 될 함수 등록해주는 함수
    //      SingleTon 객체들 메모리 해제할 때 사용

    // 불필요하다고 생각하는 매니저를 런타임 도중에 메모리 해제할 수 있다.

    ```

- init(HWND, Vec2)
    - AdjustWindowRect()
    - SetWindowPos()
- progress() : 매틱마다 호출되는 함수


### struct.h 헤더 생성
- 선언시 windows.h 아래에 위치해야한다. (POINT 타입 인식)
- Vec2 구조체 생성

### CDevice 클래스
DirectX11 기준 GPU 제어  
게임의 최저 사양이 안되는 경우 실패처리  
- ID3D11Device : GPU 메모리 관리, 객체 생성 - 메모리 관련 주 업무를 담당하는 클래스
- ID3D11DeviceContext : GPU 렌더링 명령
- IDXGISwapChain : 출력 버퍼 지정

[OM = OutputMergeState]
- ID3D11Texture2D : 렌더타겟 텍스쳐
- ID3D11RenderTargetView : 렌더 타겟 뷰
- ID3D11Texture2D : 뎊스 스텐실 텍스쳐
- ID3D11DepthStencilView : 뎊스 스텐실 뷰

- ID3D11RasterizerState
- ID3D11BlendState
- ID3D11DepthStencilState
- ID3D11SamplerState


### DirectX 
- DirectX 9 은 라이브러리를 다운받아서 사용햐야했으나, 이제 그럴필요 없다.
```cpp
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

using namespace DirectX;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
```

## To be...
Device 장치 초기화 과정 