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
<br><br>

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

extern tTransform g_Transform;
```