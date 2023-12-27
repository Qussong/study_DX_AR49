23.12.27<br>

[1교시]

## TIP
UV 좌표계와 투영좌표계(NDC) 구분 잘하기<br>
보간(=내삽,interpolation) : 알려진 데이터 지점의 고립점 내에서 새로운 데이터 지점을 구성하는 방식<br>

## 객체 삭제

물체를 삭제할 때, 다음 프레임에 삭제하지 않으면 문제가 발생할 수 있다.<br>
**_로직_** : <br>
이번 프레임에선 Dead 상태로 만들어주고, 다음 프레임에서 해당 객체를 가리키던 객체들이 더이상 가리키지 않도록 해준후 해당 프레임에서 Dead 상태인 객체를 GC로 보내 삭제해준다.<br>

객체의 Dead Flag :<br>
```cpp
// CGameObject
public:
    m_bDead;    // 초기값 false
                // 생성된 객체가 바로 dead 상태이면 안된다.
```

Dead 상태인 객체가 삭제되는 곳 :<br>
FinalTick 호출시 iterator 로 루프시켜준다.
```cpp
// CLayer
void FinalTick();
```

### Garbage Collector
삭제 오브젝트를 모아만 둔다.<br>
조건은 직접 구현하기<br>
프레임이 괜찮다면 없애도 된다.<br>
```cpp
class CGC : 
    public CSingleton<CGC>
{
    SINGLE(CGC);
private:
    vector<CEntity*>    m_vecEntity;

public:
    void tick();
    void Add(CEntity* _entity);
}
```

## Sampler

두가지 종류의 샘플러 생성함<br>
1.확대시 뿌옇다(보간o)<br>
2.확대시 뿌옇지 않다(보간x)<br>
```cpp
// CDevice
```

---
[2교시]

## RenderComponent

순서 : 매쉬선택 -> 쉐이더선택<br>
"재질"이라는 개념을 추가로 도입한다.<br>
Material의 역할 : 필요한 파라미터를 중간에서 전달해주는 역할을 한다.<br>
```
                ┌ Mesh 선택
GameObject  --- |
                └ Materail 선택 -> Shader 선택
```

### 구조변경
RenderComponent 는 Shader 대신에 Material 을 참조하게된다.<br>
Material 이 Shader 를 참조한다.<br>


## CMaterial class
클래스 생성
```cpp
class CMaterial :
    public CAsset
{
public:
    CMaterial();
    ~CMaterial();

private:
    tMtrConst           m_const;
    CTexture*           m_arrTex[TEX_PARAM::END];   // 10개

    CGraphicShader*     m_pShader;

public:
    void SetShader(CGraphicShader* _shader);
    CGraphicShader* GetShader();
}
```

### tMtrConst 
Material 대응 구조체 추가
```cpp
// struct
struct tMtrConst
{
    int     iArr[4];
    float   fArr[4];
    Vec2    v2Arr[4]
    Vec4    v4Arr[4]
    Matrix  matArr[4]
}
```

### ScalarParam
Material 을 통해서 전달할 수 있는 파라미터의 타입 정의
```cpp
enum SCALAR_PARAM
{
    INT_0,
    INT_1,
    INT_2,
    INT_3,

    FLOAT_0,
    FLOAT_0,
    FLOAT_0,
    FLOAT_0,

    VEC2_0,
    VEC2_1,
    VEC2_2,
    VEC2_3,

    VEC4_0,
    VEC4_1,
    VEC4_2,
    VEC4_3,

    MAT_0,
    MAT_1,
    MAT_2,
    MAT_3,
}
```

```cpp
// 전달 할 수 있는 이미지 개수 임의로 설정한것(10개)
enum TEX_PARAM
{
    TEX_0,
    TEX_1,
    TEX_2,
    TEX_3,
    TEX_4,
    TEX_5,
    TEX_6,

    TEXCUBE_0,
    TEXCUBE_1,

    TEXARR_0,
    TEXARR_1,

    END,
}
```

## ConstBuffer

### UpdateData()
본인의 타입이 레지스터의 번호이기에 함수 형태 변경 & 타입값을 가지고 있는 멤버변수 추가, 생성자에서 본인의 타입을 인자로 받도록 변경
```cpp
CB_TYPE::TRANSFORM
CB_TYPE:::MATERIAL_TYPE
```

### 쉐이더 코드
쉐이더 코드는 최소 단위가 16Byte 이다.<br>
이 때문에 배열을 사용하기가 힘들다
```cpp
// std2d.fx

cbuffer MATERIAL_CONST : register(b1)
{
    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;

    // ...
}
```