23.12.14<br>

## **ConstBuffer class** 추가
- Create(UINT _size, UINT _cnt);<br>
    → 상수버퍼 생성
- SetData(void* _src, UINT_cnt = 0);<br>
    → 상수버퍼에 데이터 세팅 후 CPU -> GPU 데이터 넘김
- UpdateData(UINT _registerNum);<br>
    → VSSetConstantBuffers 를 통해 VS 파이프라인 단계에서 사용하는 상수 버퍼를 설정해준다.

## **CComponent class** 수정
- 함수추가
    ```cpp
    public:
        CComponent(COMPONENT_TYPE _type);
        ~CComponent();

    private:
        const COMPONENT_TYPE m_type;
        CGameObject* m_owner;

    public:
        virtual void Begin() {};
        virtual void Tick() {};
        virtual void FinalTick() = 0;
        virtual void UpdateData() {};

    public:
        COMPONENT_TYPE GetType() { return m_type; }
        CGameObject* GetOwner() { return m_owner; }
        
        friend class GameObject;
    ```

## **CRenderComponent class** 추가
- CComponent 클래스 상속
- 생성자의 인자값을 통해서 컴포넌트의 종류를 전달한다.
    ```cpp
    CRenderComponent(COMPONENT_TYPE _type)
        : CComponent(_type)
    { };
    ```
- 멤버변수 추가
    ```cpp
	CMesh* m_Mesh;
	CGraphicsShader* m_Shader;
    ```
- 멤버함수 추가
    ```cpp
    public:
        void SetMesh(CMesh* _mesh) { m_mesh = _mesh; }
        void SetShader(CGraphicsShader* _shader) { m_shader = _shader; }

        CMesh* GetMesh() { return m_mesh; }
        CGraphicsShader* GetShader() { return m_shader; }

    public:
        virtual void FinalTick() {};
        virtual void UpdateData() = 0;
        virtual void Render() = 0;
    ```
## **CMeshRender class** 추가
- RenderComponent 클래스 상속, COMPONENT_TYPE::MESHRENDER
    ```cpp
    // CMeshRender.h
    class CMeshRender :
	public CRenderComponent
    { }

    // CMeshRender.cpp
    CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)
    { }
    ```
- 멤버함수 추가
    ```cpp
    virtual void UpdateData() override;
    virtual void Render() override;
    ```

## **define.h** 내용 추가
- 방향
    ```cpp
    enum class DIR_TYPE
    {
        RIGHT,
        UP,
        FRONT
    };
    ```
- 상수버퍼의 종류
    ```cpp
    enum class CB_TYPE
    {
        TRANSFORM,
        MATERIAL_CONST,
        GLOBAL_DATA,
        ANIMATION,

        END,
    };
    ```
## **CDevice class** 수정
- 멤버변수 추가<br>
    ```cpp
    // 다양한 상수버퍼들을 저장하고있는 배열
    CConstBuffer* m_arrCB[(UINT)CB_TYPE::END];
    ```
    - 상수버퍼의 종류 : TRANSFORM, MATERIAL_CONST, GLOBAL_DATA, ANIMATION
- 멤버함수 추가
    ```cpp
    // 원하는 종류의 상수버퍼 반환
    CConstBuffer* GetConstBuffer(CB_TYPE _type) { return m_arrCB[(UINT)_type]; }

    // Transform 타입 상수버퍼 생성
    int CreateConstBuffer()
    {
        m_arrCB[(UINT)CB_TYPE::TRANSFORM] = new CConstBuffer;
        m_arrCB[(UINT)CB_TYPE::TRANSFORM]->Create(sizeof(tTransform), 1);

        return S_OK;
    }

    ```

## **func.h** 추가
```cpp
// 포인터 배열의 각 인자를 메모리해제시켜주는 함수
template<typename T, UINT SIZE>
void Delete_Array(T* (&Arr)[SIZE])
{
	for (int i = i = 0; i < SIZE; ++i)
	{
		if (nullptr != Arr[i])
		{
			delete Arr[i];
			Arr[i] = nullptr;
		}
	}
}
```
```cpp
// 벡터가 가지고 있는 모든 포인터 타입 인자들을 메모리해제시켜주는 함수
template<typename T>
void Delete_Vec(vector<T*>& _vec)
{
	for (size_t i = 0; i < _vec.size(); ++i)
	{
		if (nullptr != _vec[i])
			delete _vec[i];
	}

	_vec.clear();
}
```

## **Transform class** 수정
- 멤버변수 추가
    ```cpp
    Vec3    m_vRelativePos;
	Vec3    m_vRelativeScale;
	Vec3    m_vRealtiveRotation;
	Vec3    m_arrDir[3];    // Right, Up, Front
	Matrix  m_matWorld;
    ```
- 멤버함수 추가
    ```cpp
    // UpdateData() 함수 가지고 있음
	virtual void FinalTick() override;
    // 상수버퍼에 transform의 값을 넣고 GPU 에 넘겨줌
	virtual void UpdateData() override;

	void SetRelativePos(Vec3 _Pos) { m_vRelativePos = _Pos; }
	Vec3 GetRelativePos() { return m_vRelativePos; }

	void SetRelativeScale(Vec3 _Scale) { m_vRelativeScale = _Scale; }
	Vec3 GetRelativeScale() { return m_vRelativeScale; }

	void SetRelativeRotation(Vec3 _Rotation) { m_vRealtiveRotation = _Rotation; }
	Vec3 GetRelativeRotation() { return m_vRealtiveRotation; }

	const Matrix& GetWorldMat() { return m_matWorld; }

	Vec3 GetDir(DIR_TYPE _type) { return m_arrDir[(UINT)_type]; }
    ```

## **GameObject class** 수정
- 멤버함수 추가
    ```cpp
    // GameObject 객체가 가지고 있는 모든 Component의 Begin() 호출
    void Begin();
    // GameObject 객체가 가지고 있는 모든 Component의 Tick() 호출
	void Tick();
    // GameObject 객체가 가지고 있는 모든 Component의 FinalTick() 호출
	void FinalTick();
    // nothing
	void Render();
    // GameObject 객체에 Component 를 추가하고 해당 Component의 소유자가 어떤 GameObject인지 알려준다.
	void AddComponent(CComponent* _comp);
    ```
- 매크로 추가
    ```cpp
    #define GET_COMPONENT(Type, TYPE)\
            class C##Type* Type()\
            {\
                return (C##Type*)m_arrCom[(UINT)COMPONENT_TYPE::##TYPE];\
            }
    ```
- 멤버변수 추가
    ```cpp
    CRenderComponent*	m_renderCom;
    ```

- 멤버함수 추가2
    ```cpp
    CComponent* GetComponent(COMPONENT_TYPE _Type) { return m_arrCom[(UINT)_Type]; }

	GET_COMPONENT(Transform, TRANSFORM);
	GET_COMPONENT(MeshRender, MESHRENDER);
    ```