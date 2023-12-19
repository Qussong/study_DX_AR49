23.12.19

## CTransform

### FinalTick()
```cpp
// before
m_matWorld._11 = m_vRelativeScale.x;
m_matWorld._22 = m_vRelativeScale.y;
m_matWorld._33 = m_vRelativeScale.z;

m_matWorld._41 = m_vRelativePos.x;
m_matWorld._42 = m_vRelativePos.y;
m_matWorld._43 = m_vRelativePos.z;
```
```cpp
// after
Matrix matScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);

Matrix matRotX = XMMatrixRotationX(m_vRealtiveRotation.x);
Matrix matRotY = XMMatrixRotationY(m_vRealtiveRotation.y);
Matrix matRotZ = XMMatrixRotationZ(m_vRealtiveRotation.z);

Matrix matTranslation = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);

m_matWorld = matScale * matRotX * matRotY * matRotZ * matTranslation;

m_arrDir[(UINT)DIR_TYPE::RIGHT] = Vec3(1.f, 0.f, 0.f);
m_arrDir[(UINT)DIR_TYPE::UP]    = Vec3(0.f, 1.f, 0.f);
m_arrDir[(UINT)DIR_TYPE::FRONT] = Vec3(0.f, 0.f, 1.f);

for (int i = 0; i < 3; ++i)
{
	m_arrDir[i] = XMVector3TransformNormal(m_arrDir[i], m_matWorld);
	m_arrDir[i].Normalize();
}
```
<br>

### UpdateData()
```cpp
// before
tTransform transform = {};
transform.matWorld = m_matWorld;

CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
pCB->SetData(&transform);
pCB->UpdateData(0);
```
```cpp
// after
g_transform.matWorld = m_matWorld;

CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
pCB->SetData(&g_transform);
pCB->UpdateData(0);
```
<br>

## struct.h

### 상수버퍼 대응 구조체
```cpp
// before
struct tTransform
{
	Matrix	matWorld;
};
```
```cpp
// after
struct tTransform
{
	Matrix	matWorld;
	Matrix	matView;
	Matrix	matProj;
};
extern tTransform g_transform;
```
g_transform 은 extern.cpp 에서 구현
```cpp
// extern.cpp
#include "pch.h"
tTransform g_transform = {};
```
<br>

## CCamera class

### FinalTick()
```cpp
Vec3 vCamPos = Transform()->GetRelativePos();

m_matView = XMMatrixIdentity();

m_matView._41 = -vCamPos.x;
m_matView._42 = -vCamPos.y;
m_matView._43 = -vCamPos.z;

m_matProj = XMMatrixIdentity();

g_transform.matView = m_matView;
g_transform.matProj = m_matProj;
```
<br>

## CameraMoveScript class
```cpp
// 클래스만 추가
class CCameraMoveScript :
	public CScript
{
public:
	CCameraMoveScript();
	~CCameraMoveScript();
	
public:
	virtual void Tick();
};
```
<br>

## PlayerScript.cpp
### Tick()
```cpp
// 회전객체 추가
Vec3 vRot = Transform()->GetRelativeRotation();

// 회전이동 추가
if (KEY_PRESSED(KEY::X))
{
	vRot.x += DT * XM_PI;
}

if (KEY_PRESSED(KEY::Y))
{
	vRot.y += DT * XM_PI;
}

if (KEY_PRESSED(KEY::Z))
{
	vRot.z += DT * XM_PI;
}
Transform()->SetRelativeRotation(vRot);
```
<br>

## CLevelMgr.cpp
CameraMoveScript 헤더 추가<br>
Camera 객체 생성 및 초기화<br>

### Init()
```cpp
// Camera Object 생성
CGameObject* pCamObj = new CGameObject;
pCamObj->AddComponent(new CTransform);
pCamObj->AddComponent(new CCamera);
pCamObj->AddComponent(new CCameraMoveScript);

pCamObj->Transform()->SetRelativePos(Vec3(0.5f, 0.f, 0.f));
pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

m_curLevel->AddObject(pCamObj, 0);
```

## 쉐이더코드 수정

### cbuffer TRANSFORM
```cpp
// before
cbuffer TRANSFORM : register(b0)
{
    row_major float4x4 g_matWorld;
}
```
```cpp
// after
cbuffer TRANSFORM : register(b0)
{
    row_major float4x4 g_matWorld;
    row_major matrix g_matView;
    row_major matrix g_matProj;
}
```
**"row_major"** 키워드는 GPU가 행렬을 읽을때 열우선순위로 읽기때문에 행우선순위로 읽어라는 의미로 설정해둔 키워드이다.
<br>

### VS_Std2D()
```cpp
// before
VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWorld);
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    return output;
}
```
```cpp
// after
VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    // 로컬(모델) 좌표를 -> 월드 -> 뷰 -> 투영 좌표계로 순차적으로 변환
    float4 vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);  // W
    float4 vViewPos = mul(vWorldPos, g_matView);                // V
    float4 vProjPos = mul(vWorldPos, g_matProj);                // P
    
    output.vPosition = vProjPos;;
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
    return output;
}
```