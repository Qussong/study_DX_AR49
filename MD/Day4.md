23.12.01
- 구조 설계하다가 멈춤 (CEntity.h/cpp, CLevel.h/cpp)
- 삼각형 출력 테스트 (Test.h.cpp)
- string 헤더 추가 (global.h)
    ```cpp
    #include <string>
    using std::wstring;
    using std::string;
    ```
- SimpleMath.h/inl 추가
- Viewport 설정 추가 (CDevice.cpp)
- DEVICE, CONTEXT 메크로 추가 (define.h)
<br><br>

## Entity

<br><br>

## SimpleMath
```cpp
// 
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;
typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;
```
<br><br>

## DEVICE, CONTEXT
```cpp
// define.h
#define DEVICE CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetContext()

// CDevice.h
ID3D11Device* GetDevice() { return m_Device.Get(); }
ID3D11DeviceContext* GetContext() { return m_Context.Get(); }
```
<br><br>
