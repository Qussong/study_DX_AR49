23.12.11<br>
- 필터 추가
    ```
    06. Asset
    07. GameObject
    08. Component
    ```
- CAsset class 추가
- define.h 요소 추가
    ```cpp
    enum class ASSET_TYPE { }
    enum class COMPONENET_TYPE { }
    ```
- CMesh class 추가<br>
    역할 : VertexBuffer 생성, IndexBuffer 생성, 파이프라인에 VB,IB 세팅
- CGraphicsShader class 추가<br>
    역할 : VertexShader 생성, PidexShader 생성, Layout 생성, Topology 설정, 파이프라인에 Layout, Topology, VS/HS/DS/GS/PS 세팅
- Test 클래스 구조 변경 및 Circle Mesh 출력 코드 추가
<br><br>

### <span style="color:violet">D3D11_BUFFER_DESC
버퍼 리소스에 대해서 설명한다.<br>
생성하고자하는 버퍼에 대해서 서술한다.<br>

```cpp
// d3d11.h
typedef struct D3D11_BUFFER_DESC
    {
    UINT ByteWidth;
    D3D11_USAGE Usage;
    UINT BindFlags;
    UINT CPUAccessFlags;
    UINT MiscFlags;
    UINT StructureByteStride;
    } 	D3D11_BUFFER_DESC;
```
- CPUAccessFlags :<br>
    CPU가 버퍼에 접근하는 방식을 식별한다.<br>
    CPU가 버퍼를 사용하지 않는다면 0을 지정한다.
<br><br>

### <span style="color:violet">원본 정점/인덱스 정보 동적할당하여 저장
```cpp
// CMesh.cpp

m_VtxSysMem = new Vtx[m_VtxCount];
m_IdxSysMem = new UINT[m_IdxCount];

memcpy(m_VtxSysMem, _Vtx, sizeof(Vtx) * m_VtxCount);
memcpy(m_IdxSysMem, _Idx, sizeof(UINT) * m_IdxCount);
```
- **memcpy()** :<br>
    ```cpp
    void *memcpy(
        void        *dest,   // 새로운 버퍼
        const void  *src,    // 원본 버퍼
        size_t      count    // 복사할 문자의 수
    );
    ```

### <span style="color:violet">FAILED(hr)
```cpp
// intsafe.h
#define SUCCEEDED(hr)   (((HRESULT)(hr)) >= 0)
#define FAILED(hr)      (((HRESULT)(hr)) < 0)
```
인자값(hr)으로 들어온 값이 0보다 작으면 True<br>
즉, 특정 조건이 실패처리하고자 한다면 해당 매크로의 인자값으로 0 이하의 값을 반환하도록 해주면 된다.<br>
반대로 성공시엔 0 이상의 값을 반환하도록 하면된다.
<br><br>

### <span style="color:violet">E_FAIL
- COM 오류 코드
- COM 기반 API에서 사용하는 오류코드 목록중 하나이다.
```
E_FAIL          : 지정되지 않은 오류
E_UNEXPECTED    : 치명적인 오류
```
<br><br>

### <span style="color:violet">IASetVertexBuffers() / IASetIndexBuffer()
```cpp
CONTEXT->IASetVertexBuffers(0, 1, mVB.GetAddressOf(), &iStride, &iOffset);
CONTEXT->IASetIndexBuffer(mIB.Get(), DXGI_FORMAT_R32_UINT, 0);
```
**_IASetVertexBuffers_**<br>
```cpp
void IASetVertexBuffers(
    [in]           UINT         StartSlot,
    [in]           UINT         NumBuffers,
    [in, optional] ID3D11Buffer * const *ppVertexBuffers,
    [in, optional] const UINT   *pStrides,
    [in, optional] const UINT   *pOffsets
);
```
- StartSlot : 바인딩에 대한 첫 번째 입력 슬롯
- NumBuffers : 배열의 꼭짓점 버퍼의 개수
- ppVertexBuffers : 꼭짓점 버퍼 배열에 대한 포인터
- pStrides : 꼭짓점 버퍼에서 사용할 요소의 크기(Byte)
- pOffsets : 꼭짓점 버퍼의 첫 번째 요소와 사용될 첫 번째 요소 사이의 바이트 수
**_IASetIndexBuffer_**<br>
<br><br>

