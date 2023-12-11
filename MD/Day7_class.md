23.12.07  
<span style="color:yellowgreen"> YellowGreen </span>
<span style="color:violet"> Violet </span>

## 1교시
TODO : 구조변경<br>
Modeling Space, Local Space : 원본 모델링 객체가 위치하고 있는 위치<br>
World Space : 게임이 플레이되고 있는 환경<br>

### <span style="color:violet">Asset</span>
**"class CAsset: public CEntity"** 클래스 추가<br>
<br><br>

### <span style="color:violet">enum class ASSET_TYPE 
define.h 에 추가<br>
CAsset 객체 추가시 생성자 인자값으로 들어간다.<br>
<br><br>

### <span style="color:violet">Mesh
**"class CMesh : public CAsset"** 클래스 추가<br>
Asset 에서 파생된 객체<br>
역할 : 모델링 정보 저장<br>
가지고 있는 정보 : **_VB, IB_**
```cpp
// 멤버변수
ComPtr<ID3D11Buffer>    m_VB        // 
ComPtr<ID3D11Buffer>    m_IB        //

D3D11_BUFFER_DESC       m_VBDesc    //
D3D11_BUFFER_DESC       m_IBDesc    //

// 원본 정점정보 및 인덱스 정보를 저장해준다.
// !!! 직접 할당한 메모리이기에 직접 지워줘야한다 !!!
void*                   m_VtxSysMem //
void*                   m_IdxSysMem //

// 멤버함수
int Create(void*, UINT, void*, UINT);
void UpdateData();
void redner();
```
Test.cpp 에 Mesh 클래스를 활용해서 사각형 출력하는 코드로 수정<br>
Test.cpp 에 IB,VB 가 멤버로 들어갈 필요가 없어진다.
모든 Mesh는 프로그램 종료시 메모리 해제해줘야한다.
<br><br>

### <span style="color:violet">Circle Mesh
vector 사용<br>
삼각형 수가 많을수록 더욱 동그란 원이된다.<br>
VB에 중점을 제일먼저 넣은 후 원둘레 좌표를 넣어준다.<br>
IB에 들어가는 정보 유심히보자<br>
NDC 좌표기준이라 화면의 해상도비율(종횡비 = 세로/가로)에의해 타원형으로 보인다.<br>
```cpp
vector<Vtx>     vecVtx;
vector<UINT>    vecIdx;

// 중심점 설정
{
    Vtx v;
    v.vPos;
    v.vColor;
    v.cUv;
}
UINT            islice;
```
<br><br>

### <span style="color:violet">Shader
**_class CShader : public CAsset_** 생성<br>
**<span style="color:yellowgreen">ComputerShader** 와 **<span style="color:yellowgreen">GraphicsShader**로 나뉜다.<br>
현재 배우는 내용은 GraphicsShader 이다. (ComputerShader는 나중에 배운다.)<br>
Com/Gra Shader의 공통 멤버로는 ErrBlob이 있다. 그 각자 따로 가지고 있다.


### GraphicsShader
역할 : 파이프라인 단계에서 어떻게 적용할지에대한 각 단계에 대한 세팅값<br>

```cpp
// [Blob]
m_VSBlob
// m_HSBlob
// m_DSBlob
// m_GSBlob
m_PSBlob

// [Shader]
m_VS
// m_HS
// m_DS
// m_GS
m_PS

// [Rasterizer State]

// [OuputMerge]

// [DepthStnecil State]

// [Blend State]

// 함수
int CreateVertexShader(const string& _strFilePath, const string& _strHLSLVer);
// int CreateHullShader();
// int CreateDomainShader();
// int CreateGeometryShader();
int CreatePixelShader();

void UpdateData();
```

## 2교시
쉐이더 코드 정리 : Test.cpp 의 코드를 GraphicsShader로 옮김<br>
Test.cpp 에 GraphicsShader 객체 만들어서 사각형 출력하도록 코드 변경<br>

CONTEXT = 관리자<br>
CONTEXT가 받게되는 Shader 의 세팅을 온전히 받기위해서 항상 새로운 Shader 를 받기전에 미리 모든 란을 nullptr 로 밀어줘야한다.<br>
위의 이유때문에 GraphicsShader.DataUpdate() 시 모든 shader를 nullptr 로 밀어준다.<br>

VS의 가장 중요한 역할은 IA 입력된 VB의 정점정보를 정점으로 받아준다.<br>

Layout 과 Topology 도 Shader 안으로 들어간다.<br>


### <span style="color:violet">Object
Unity식으로 **GameObject** 라고 한다.<br>
**"class CGameObject : public CEntiy"** 추가<br>
Component 기반으로 진행할 예정이며 api 때는 vector로 관리했지만 dx 떄는 배열로 관리한다.<br>

**_▼ Component 의 종류_**
```cpp
// define.h

enum class COMPONENT_TYPE
{
    TRANSFORM,  // 오브젝트의 위치, 크기, 회전

    COLLIDER2D, //  2차원 충돌체
    COLLIDER3D, // 

    ANIMATOR2D, // 스프라이트 Animation
    ANIMATOR3D, // Bone Skinning Anumation

    LIGHT2D,    // 2차원 광원
    LIGHT3D,    // 3차원 광원

    CAMERA,     // 카메라 기능

    // Render Component
    MESHRENDER,
    TILEMAP,
    PARTICLESYSTEM,
    SKYBOX,
}
```