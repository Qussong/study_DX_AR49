23.12.20<br>

---
[1교시]

### <span style="color:violet">현재...
현재 물체의 뒷면이 화면에 출력이 안되는데 그 이유는 아직 파이프라인을 전부 사용하고 있지 않기때문이다.
<br><br>

### <span style="color:violet">렌더링 파이프라인
- IA : VB,IB(mesh),Topology(shader),InputLayout
- VS
- HS(x)
- DomainState(x)
- GS(x)
- Rasterizer : 알맞은 PS 호출
- PS
- OM : DepthStencilState, BS
<br><br>

## Rasterizer State

### <span style="color:violet">define.h
RS의 종류에 대한 enum class 정의
```cpp
enum class RS_TYPE
{
    CULL_BACK,  // default <- 굳이 만들필요가 없다, nullptr
    CULL_FRONT,
    CULL_NONE,  // 우리가 사용할것
    WIRE_FRAME
}
```
<br>

### <span style="color:violet">CDevice.h
RS 객체 멤버변수 선언<br>
RS 의 종류가 여러개 이기에 행렬로 선언<br>
<br>

### <span style="color:violet">Culling Mode
Culling Mode 설정 때문에 물체의 뒷면이 보이지 않는다.<br>
<br>

## DepthStencil State
default 값 : less<br>
less 이기에 깊이값이 작을수록 앞에 있는것으로 판정되어 다른것들보다 앞에 그려진다.<br>
grater 옵션을 사용할경우 깊이판정이 반대로 되어버린다.<br>
절두체에서의 위치값이 합쳐져서 출력이 되는데 이때 깊이에대한 정보가 여기에 저장된다<br>
★ 반투명한 애의 경우 겹치는 애와 색이 섞여야한다. 때문에 깊이버퍼에 깊이를 남기지 않아야한다.<br>
<br>

### define.h
DS의 종류에대한 enum class 를 정의한다.
```cpp
enum class DS_TYPE
{
    LESS,           // 작은게 통과              깊이기록 o, nullptr
    LESS_EQUAL,     // 작거나 같으면 통과       깊이기록 o
    GRATER,         // 더 멀면 통과             깊이기록 o
    GRATER_EQUAL,   // 더 멀거나 같으면 통과    깊이기록 o

    NO_TEST,        // 깊이 테스트 x            깊이기록 o
    NO_WRITE,       // 깊이 테스트 o            깊이기록 x

    NO_TEST_NO_WRITE// 깊이 테스트 x            깊이 기록 o
}
```

## Shader
생성한 DS 와 RS로 Shader에 culling, depth 판정을 어떻게 할 것인지 설정해준다.

---
[2교시]

## DepthStencilState

### <span style="color:violet">CreateDepthStencilState()
DepthWriteMastk = 깊이정보를 남기는 경우 DeD11_DEPTh_WRITE_MASK_ALL, 마스킹?<br>
<br>

### <span style="color:violet">Stencil?
구글에 검색해보면 구멍 뚫린 종이들이 나온다.<br>
미술의 Stencil 기법과 유사하다.<br>
가둬두고 색을 채우는 기법<br>
"early z" 사용시 렌더링 기법 활용용으로 사용<br>
<br>

## BlendState

### define.h
BS의 종류를 enum class 로 정의한다.
```cpp
enum class BS_TYPE
{
    default,    // nullptr
    alpha_blend,
    one_one,
    end,
}
```