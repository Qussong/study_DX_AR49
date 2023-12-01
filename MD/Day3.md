23.11.30
- CDevice.h/cpp

## CDevice 클래스

Device, Device Context, SwapChain 객체 생성 및 초기화<br>
```
	ComPtr<ID3D11Device>			m_Device;
	ComPtr<ID3D11DeviceContext>		m_Context;
	ComPtr<IDXGISwapChain>			m_SwapChain;
```

### ID3D11Device
GPU 메모리 관리 및 객체 생성 담당<br>
▼ 생성 및 초기화 코드
```cpp
// 장치 초기화
D3D_FEATURE_LEVEL eLevel = D3D_FEATURE_LEVEL_11_0;

// Device 생성
HRESULT hr = D3D11CreateDevice(
    nullptr
    , D3D_DRIVER_TYPE_HARDWARE
    , nullptr
    , D3D11_CREATE_DEVICE_DEBUG
    , nullptr, 0
    , D3D11_SDK_VERSION
    , m_Device.GetAddressOf()
    , &eLevel
    , m_Context.GetAddressOf());
```
**[ D3D_FEATURE_LEVEL ]**<br>
"기능수준"<br>
DX11에서부터 도입된 기능으로 GPU가 지원하는 기능의 수준을 나타내는 지표로 사용된다.<br>
D3D_FEATURE_LEVEL 이라는 enum 을 사용한다.<br>
```cpp
typedef enum D3D_FEATURE_LEVEL {
    D3D_FEATURE_LEVEL_1_0_CORE,
    D3D_FEATURE_LEVEL_9_1,
    D3D_FEATURE_LEVEL_9_2,
    D3D_FEATURE_LEVEL_9_3,
    D3D_FEATURE_LEVEL_10_0,
    D3D_FEATURE_LEVEL_10_1,
<<<<<<< HEAD
    D3D_FEATURE_LEVEL_11_0, // choice
=======
    D3D_FEATURE_LEVEL_11_0, // 채택
>>>>>>> 27c77e8bba5d4d98597d607b24ac6e6643a25c5a
    D3D_FEATURE_LEVEL_11_1,
    D3D_FEATURE_LEVEL_12_0,
    D3D_FEATURE_LEVEL_12_1,
    D3D_FEATURE_LEVEL_12_2
}
```
`"해당 프로그램을 사용하기 위해서는 최소한 이정도의 기능을 가지고 있어야 한다"`를 알려준다.<br>
높은 수준에서 낮은 수준으로 level 을 점검하며, 지정해준 최소 level 까지 점검하고 해당 level 까지 지원하지 않는다면 프로그램을 위한 최소사양을 충족하지 않는것으로 간주하고 장치 초기화를 실패하게된다.<br>

**[ D3D11CreateDevice ]**<br>
디스플레이 어댑터를 가리키는 디바이스를 생성한다.<br>
디스플레이 어댑터? : GPU, 시스템에서 디스플레이와 상호작용하여 그래픽을 출력하는 장치
```cpp
HRESULT D3D11CreateDevice(
  [in, optional]  IDXGIAdapter            *pAdapter,
                  D3D_DRIVER_TYPE         DriverType,
                  HMODULE                 Software,
                  UINT                    Flags,
  [in, optional]  const D3D_FEATURE_LEVEL *pFeatureLevels,
                  UINT                    FeatureLevels,
                  UINT                    SDKVersion,
  [out, optional] ID3D11Device            **ppDevice,
  [out, optional] D3D_FEATURE_LEVEL       *pFeatureLevel,
  [out, optional] ID3D11DeviceContext     **ppImmediateContext
);
```
- *pAdapter : 그래픽 어댑터(GPU)를 가리키는 포인터, nullpt인 경우 기본 그래픽 어댑터 사용
- DriverType : 그래픽 드라이버 유형, "~_HARDWARE"는 하드웨어 가속을 의미한다.
- Software : 소프트웨어 드라이버, 일반적으로 nullptr
- Flags : 디버그 모드 또는 다른 설정 옵션을 지정할 수 있다.
- pFeatureLevels : 사용하고자 하는 "DX 기능 수준" 배열에 대한 포인터, nullptr인경우 아래의 기능 수준 배열을 사용한다.
    ```cpp
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };
    ```
- FeatureLevels : aFeatureLevels의 요소 수
- SDKversion : SDK 버전 (D3D11_SDK_VERSION)
- ppDevice: 생성된 Device 를 가리키는 ID3D11Device 개체에 대한 포인터를 반환
- pFeatureLevels : 생성된 DX 장치가 지원하는 기능 수준을 반환
- ppImmediateContext : 생성된 Device Context 를 가리키는 ID3D11DeviceContext 개체에 대한 포인터를 반환
<br><br>

### ID3D11DeviceContext
GPU 렌더링 명령 담당<br>
<<<<<<< HEAD
=======

>>>>>>> 27c77e8bba5d4d98597d607b24ac6e6643a25c5a
<br><br>

### IDXGISwapChain
출력 버퍼 지정 담당<br>
<<<<<<< HEAD
=======

>>>>>>> 27c77e8bba5d4d98597d607b24ac6e6643a25c5a
<br><br>

