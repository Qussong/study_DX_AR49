23.12.22<br>

## BlendState

### One-One
OneOne 설정 Blend State 추가 생성<br>
```cpp
// CDevice.cpp
int CDevice::CreateBlendState()
{
    // ...

	// ONE_ONE
	{
		tDesc.AlphaToCoverageEnable = false;
		tDesc.IndependentBlendEnable = false;
		tDesc.RenderTarget[0].BlendEnable = true;
		tDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		tDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		tDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		tDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		tDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		tDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		tDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	HRESULT hr = DEVICE->CreateBlendState(&tDesc, m_arrBS[(UINT)BS_TYPE::ONE_ONE].GetAddressOf());
	CHECK(hr);

    // ...
}
```

## Transform

### 멤버 추가
월드좌표와 지역좌표의 구분으로인해 방향벡터 행렬 수정<br>
```cpp
// [CTransform.h]
// before
Vec3    m_arrDir[3];	// Right, Up, Front
// after
Vec3    m_arrLocalDir[3];	// local
Vec3    m_arrWorldDir[3];	// world
```

### 월드좌표, 로컬좌표
월드와 로컬 개념이 추가됨으로인해 기존에 로컬좌표만 설정해주던 코드에서 월드 좌표도 설정해주는 코드로 변경함<br>
로컬좌표로 먼저 m_arrLocalDir,m_arrWorldDir을 설정한 다음, 만약 부모객체가 존재한다면 부모객체의 월드좌표를 불러와 m_arrWorldDir을 다시 설정해준다.<br>
부모객체를 다시 설정해주는 경우 부모객체에서 Scale 변환이 발생했다면 이를 상쇄시켜주는 작업을 추가해줘야한다.(flag 값 : m_bAbsolute)<br>
```cpp
void CTransform::FinalTick()
{

    // ...

	for (int i = 0; i < 3; ++i)
	{
		// m_matWorld 행렬에 크기정보가 있을 수 있기 때문에 다시 길이를 1로 정규화 시킨다.
		m_arrLocalDir[i] = XMVector3TransformNormal(m_arrLocalDir[i], m_matWorld);
		m_arrWorldDir[i] = m_arrLocalDir[i].Normalize();
	}

	// 부모 오브젝트가 있다면
	if (GetOwner()->GetParent())
	{
		// 부모 객체의 World Matrix
		const Matrix& matParentWorld = GetOwner()->GetParent()->Transform()->GetWorldMat();

		if (m_bAbsolute)
		{
			Vec3 vParentScale = GetOwner()->GetParent()->Transform()->GetRelativeScale();
			// 역행렬과 동일함
			Matrix matParentScaleInv = XMMatrixScaling(1.f / vParentScale.x, 1.f / vParentScale.y, 1.f / vParentScale.z);

			m_matWorld = m_matWorld * matParentScaleInv * matParentWorld;
		}
		else
		{
			m_matWorld *= matParentWorld;
		}

		for (int i = 0; i < 3; ++i)
		{
			// m_matWorld 행렬에 크기정보가 있을 수 있기 때문에 다시 길이를 1로 정규화 시킨다.
			m_arrWorldDir[i] = XMVector3TransformNormal(m_arrWorldDir[i], m_matWorld);
			m_arrWorldDir[i].Normalize();
		}
	}
}
```

## CGameObject

### 멤버 추가
부모객체와 부모객체에 포함된 자식객체들을 인식하기위해 멤버변수 추가<br>
부모객체가 있는지 확인하는 함수, 부모와의 연결을 끊는 함수, 새로운 자식 객체추가 함수 추가<br>
```cpp
// CGameObject.h
// 멤버 변수
vector<CGameObject*>	m_vecChild;
CGameObject*			m_parent;
// 함수
CGameObject* GetParent() { return m_parent; }
void DisconnectWithParent();
void AddChild(CGameObject* _Child);
```

### 부모-자식 Update 순서
자식 객체는 부모객체에 영향을 받을것이기에 자식객체를 업데이트하거나 렌더링 하기전에 부모객체를 먼저 업데이트해준다.<br>
ex)<br>
```cpp
// CGameObject.cpp
void CGameObject::Begin()
{
	// 부모객체
	for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			m_arrCom[i]->Begin();
		}
	}
	// 자식객체
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Begin();
	}
}
```

### DisconnectWithParent()
자식객체를 추가해줄때 만약 이미 부모객체가 존재하는 자식객체라면 기존의 부모객체와의 연결을 끊어준 후 새로운 부모객체와 연결해줘야한다.<br>
```cpp
void CGameObject::DisconnectWithParent()
{
	vector<CGameObject*>::iterator iter = m_parent->m_vecChild.begin();
	for (; iter != m_parent->m_vecChild.end(); ++iter)
	{
		if (*iter == this)
		{
			m_parent->m_vecChild.erase(iter);
			m_parent = nullptr;
			return;
		}
	}

	// 부모가 없는 오브젝트에 DisconnectWithParent 함수를 호출 했거나
	// 부모는 자식을 가리키기지 않고 있는데, 자식은 부모를 가리키고 있는 경우
	assert(nullptr);
}
```

### AddChild()
부모 객체에 자식객체 추가<br>
```cpp
void CGameObject::AddChild(CGameObject* _Child)
{
	if (_Child->m_parent)
	{
		// 이전 부모 오브젝트랑 연결 해제
		_Child->DisconnectWithParent();
	}

	// 부모 자식 연결
	_Child->m_parent = this;
	m_vecChild.push_back(_Child);
}

```