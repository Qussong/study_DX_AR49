

## 장치 초기화

### 1. Device, DeviceContext
- `D3D_FEATURE_LEVEL` 타입 변수 선언
- `D3D11CreateDevice()` 함수  
    - D3D11_CREATE_DEVICE_DEBUG : dll 이기에 문제가 있을시 알림 받음(메모리 릭)
    - D3D11_SDK_VERSION : 메크로, d3d11 의 버전
    - 이중포인터 (device, context)  
    _(d3d11 장치 초기화 들어가는 인자들을 이해하려고 할 필요는 없다.)_
<br><br>

### 2. SwapChain
- CreateSwapChain() 함수 생성 : 내부에 SwapChain 초기화 코드를 구현해준다.
- SwapChain 생성을 위해서 `DXGI_SWAP_CHAIN_DESC` 타입의 변수를 채워야한다.
- QueryInterface() 로 IDXGIDevice 얻어온다.
- IDXGIDevice->GetParent() 로 IDXGIAdatper 얻어온다.
- IDXGIAdatper->GetParent() 로 IDXGIFactory 얻어온다.
- IDXGIFactory->CreateSwapChain() 으로 SwapChain 생성한다.
- 즉, Factory에 접근하여 SwapChain 생성한다.

#### DXGI_SWAP_CHAIN_DESC 
```cpp
// SwapChain 이 관리하는 Buffer 의 구성 정보
DXGI_SWAP_CHAIN_DESC tDesc = {};
```
- Buffer 정보
    - BufferCount
    - BufferUsage
    - BufferDesc 
        - width, height
        - format : 픽셀 포멧
        - refreshrate : Denominator(분자) ,Numerator(분모)
    - Scalin
    - ScanlineOrdering
- Flags
- SampleDesc
    - Count
    - Quality
- Windowed : 창모드
- OutputWindow : 출력 윈도우 지정
<br><br>

### 3. RenderTargetView
- 렌터 타겟 텍스처를 SwapChain 으로부터 얻어온다.
- SwapChain 을 생성하면 "렌더 타겟 텍스처(= 버퍼)"가 하나 생성된다.
- 렌더 타겟 텍스처 = 스왑 체인이 관리하는 버퍼
- 렌더 타겟 텍스처는 GPU의 RAM 을 사용한다.

- SwapChain->GetBuffer() : 스왑 체인이 관리하는 버퍼를 얻을 수 있다.
    이를 통해 SwapChain 의 참조 카운트가 늘어난다.

#### ID3D11Resource
- ID3D11Buffer
- ID3D11Texture2D


## 스마트 포인터

```cpp
// globla.h

#include <wrl.h>
using namespace Micsoft::WL;
```

- 위의 헤더를 추가해줌으로써 ComPtr<type> 사용가능  
- 지정한 타입으로 포인터 변수 생성  
- 해당 타입 변수를 하나 가지고 있는 클래스이다.  
- 클래스 객체이다 보니 소멸하면서 자신이 가지고 있는 멤버 변수 타입의 소멸자를 호출함으로써 자동으로 해당 객체를 메모리 해제한다.
- Comptr로 변수를 선언하면 스마트 포인터로 인해 생성자가 호출되기에 별도로 초기화 할 필요 없다.
- Get() : 주소값 반환
- GetAddressOf() : 더블 포인터 주소값 반환
<br><br>

## View?
- View의 종류 : RenderTargetView, DepthStencilView, ShaderResourceView, UnorderedView, etc ...
- 리소스 객체를 통해서 View가 생성되고 리소스가 필요한 곳에서는 리소스가 아니라 View 를 요청한다.  
- 즉, 다이렉트로 리소스를 요구하지 않는 구조이다.
- 이유 : texture 는 다양한 용도가 있다.
- texture 는 만들어지는 순간부터 용도가 정해진다. (용도를 지정하는 flag값이 있다.)
- 뷰는 중간 전달자의 역할을 한다. 
- 뷰는 해당 리소스가 어떤 용도인지 보장해주는? 증명해주는? ID와도 같은것
- 하나의 리소스에서 여러개의 뷰가 나올 수 있다.

## DepthStencilView
- RenderTarget 용 texture 는 SwapChain으로부터 받아올 수 있었지만, DepthStencil 용 texture 는 받아올 곳이 없기 떄문에 만들어줘야한다.
- OMSetRenderTarets() 함수에서 rendertargetview 와 depthstencilview 를 요구한다.
- OM = Output Merge State

```cpp
순서:

1. depthstencil texture 생성 : 
    D3D11_TEXUTURE_DESC desc;   //D3D11_TEXUTURE_DESC 타입 변수 선언
    desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // 픽셀 포멧

    // depthstencistate 텍스처의 해상도는 반드시 RederTarget 해상도와 동일해야한다.
    desc.Width = m_vRenderResolution.x;     
    desc.Height = m_vRenderResolution.y;

    desc.BindFlag = ;// 용도 설정

    desc.CPUAccessFlags = 0;    // 통신하는게 없다는 의미
    desc.Usage = D3D11_USAGE_DEFAULT;

    // 샘플링
    MipLevel = 1; // 1이면 저 퀄리티의 샘플이 없다는 의미 

```
뭔가 만들고자 할 때는 Device 를 사용한다. (Context 아님)
<br><br>


## 그래픽스 파이프라인
렌더링 하는 과정