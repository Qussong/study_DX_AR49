23.12.22<br>

## BlendState

종류 :
- DEFAULT
- ALPHA_BLEND
- ONE_ZERO (<- 오늘 새로 추가)
- ONE_ONE

### Desc
현재 설정은 Alpha Blending 이다.
```cpp
// CreateBlendState
tDesc.AlphaToCoverageEnable = false;
tDesc.IndependentBlendEnable = false;

tDesc.RenderTarget[0].BlendEnable = true;                       // 블랜드 사용여부
tDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;             // 블랜드 옵션 : 현재 더하기
tDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;         // 더해질 색상
tDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;    // 목적지에 이미 그려져 있는 색상
tDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;        // 알파끼리 어떻게 합칠것인지에 대한 설정(눈에 보이는 영역은 아니기에 뭐가 들어가든 상관 없긴하다.  현재설정은 Src Alpha + Dest Alha 값이 들어간다.)
tDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;          // 
tDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
```
- IndependentBlendEnable : 렌더 타겟 설정 8개 까지 가능 false면 0번의 설정이 나머지 7개에도 동일하게 적용된다.
- 공식 : Src(RGB) x A + Dest(RGB) x (1-A)

### ONE_ZERO


### ONE_ONE

## Transform

### World
월드좌표 활용시작(계층구조)<br>
레벨에선 부모객체 GameOject만 관리한다.<br>
자식 객체는 부모객체가 관리한다.<br>
```cpp
// 월드 기준 방향벡터 추가
m_arrWorldDir[3] = {}
```

## GameObject

## 부모객체
자신의 부모객체가 있는지 확인할 수 있는 멤버변수 추가<br>
null 이면 본인이 부모객체임<br>

### 자식객체
자신의 자식객체 멤버변수 추가<br>
자식객체 넣어주는 함수 추가<br>
```cpp
AddChild() 
```
이미 부모가 있는경우(예외처리) : 이미 부모가 있는 경우 해당 부모에서 새로운 부모로 옮겨지는 형태로 함수의 예외처리를 해준다.<br>

---
[2교시]

### 자식객체의 SRT 적용순서

자식객체에 본인의 SRT 적용후 부모의 SRT를 적용시켜준다.<br>

### 크기연산
자식 객체에 부모객체의 크기값이 적용되지 않도록 역행렬을 곱해주는 연산을 넣어준다.