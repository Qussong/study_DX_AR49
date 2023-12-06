23.12.06  
<span style='color:red'>red</span>
<span style='color:orange'>orange</span>
<span style='color:blue'>blue</span>

## 1교시

### 사각형
- 사각형 그리기
    - Topology가 삼각형 -> 정점이 6개 필요하다
        ```
        vertex 구성)
        0      3 - 4
        | \      \ |
        3 - 2      5
        ```
    - Vertex 위치 설정(6개)
    - 사각형 크기를 줄이기 위해 NDC 좌표 수정 (1.f -> 0.5f)

- KeyMgr 로 객체 움직이기
    - VB 는 GPU에 있는 메모리
        - bufferdesc.CPUAcessflag = write 로 되어 있기에 런타임 도중에 수정 가능
    - TestProgress() 에 Tick(), Render() 추가
    - Tick() 에서 키보드 입력 받아 전역변수의 값 변경하는 코드 추가 (↑ +)
    - CPU의 데이터를 GPU로 전달하도록 해야한다.
        ```cpp
        D3D11_MAP_SUBRESOURCE tSub = {};
        CONTEXT->Map(g_VB.Get(), 0, D3D11_MAP_WRITE_DESCARD, 0, &tSub);

        // cpu 에 gpu로 데이터를 보낼 수 있는 공간(tSub.pData)을 할당해준다.
        // Unmap 호출시 GPU로 데이터가 넘어간다.
        memcpy(tSub.pData, g_vtx, sizeof(Vtx)*6);

        CONTEXT->Unmap(g_VB.Get(), 0);

        // ☆ 하지만 해당 방식으로 물체를 움직이지 않는다. 문제점이 있다.
        ```
### 해결해야할 문제점들
1. 도형을 그릴때 겹치는 정점이 존재한다.
    - VS의 호출 횟수를 줄이지 않고 VB의 용량을 줄일 수 있는 방법을 모색해야한다.
    - **Index Buffer의 등장**
        - 정점의 개수를 6 -> 4 줄인다.
            ```
            0 -- 1
            | \\ |
            3 -- 2
            ```
        - 인덱스 버퍼를 통해 간접적으로 VS 를 6번 호출하도록 한다.<br>
            이를통해 VB의 용량은 줄어들었지만, VS는 6번 호출된다.<br>
            <span style='color:grey'>**_(이를통해 모델링 정보의 용량을 줄일 수 있다.)_**</span>

    - IndexBuffer 생성
        ```cpp
        // 버텍스 버퍼 생성 구조체
        D3D11_BUFFER_DESC bufferDesc;
        ZeroMemory(&bufferDesc, sizeof(bufferDesc));
        {
            bufferDesc.ByteWidth = sizeof(Vtx) * 3;				// 버퍼의 크기
            bufferDesc.StructureByteStride = sizeof(Vtx);		// 정점 하나의 크기
            bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;	    // 용도설정 = 버텍스 버퍼
            bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            bufferDesc.Usage = Default; // ???
        }
        ```
        Render() 과정에서 IndexBuffer를 알려주는 과정 추가
        도형 그려주는 함수 변경 (Draw -> DrawIndexed)        
<br><br>

## 2교시

- 동일한 리소스(모델링, mesh) 정보를 메모리에 여러개 올릴 이유는 없다.
- VB 를 직접 조작하여 움직이는게 좋지 않은 이유 :<br>
    Mesh 는 VB 와 IB로 이루어져있는데 VB를 직접적으로 수정하여 객체를 움직이면 해당 Mesh 를 사용하는 객체들은 모두 이동의 영향을 받게된다.
- 물체를 움직이는 방법
    - VB는 움직이지 않는다. 즉, Mesh의 원본은 움직이지 않는다.
    - 위취정보(Transform) 만 GPU로 넘겨준다.
    - VS는 모델링(Mesh)좌표에 입력된 위치정보를 더해서 반환해준다. <span style='color:yellow'>**(VS의 역할)**</span>
    - Mesh에 있는 정점 개수만큼의 메모리가 이동할것을 위치정보(x,y,z,w)만 넘겨줌으로써 부하가 적어진다.

### 상수버퍼 (Constant Buffer)
- 상수버퍼 생성
    - 상수버퍼 대응 구조체 생성
        ```cpp
        struct tTransform
        {
            Vec4 vWorldPos;
        }
        // 메모리를 16Byte로 정렬해줘야한다.
        // 만약 16Byte 를 못 맞춰줄것 같으면 Padding 값을 넣어줘야한다.
        ```
    - buffer 생성
        ```cpp
        D3D11_BUFFER_DESC bufferDesc;
        ZeroMemory(&bufferDesc, sizeof(bufferDesc));
        {
            bufferDesc.ByteWidth = sizeof(tTransform) * 3;	        // 버퍼의 크기
            bufferDesc.StructureByteStride = sizeof(tTransform);	// 정점 하나의 크기
            bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	    // 용도설정
            bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        }
        ```
    - MAP 과정 추가 (CPU -> GPU)
    - 쉐이더 파일에 상수버퍼 관려 코드 추가
    - 파이프라인에 상수버퍼 전달 (리소스 바인딩)
        ```cpp
        VSSetConstantBuffer(0, 1, g_CB.GetAddressOf());
        // 0 : 레지스터 0번 
        // 1 : 버퍼 1개
        // VS 동작전에 b0 레지스터에 상수버퍼에 대한 정보가 들어가서 사용할 수 있게된다.
        ```

### Scale
- 상수버퍼 구조체에 "Vec4 Sacle" 멤버 추가
- 쉐이더 파일상 상수버퍼의 구조체도 이에 맞게 수정
- Test.Tick() 에 Scale 관련 코드 추가


### <span style='color:orange'>**☆POINT☆**</span>
1. 사각형 그릴때 IndexBuffer 사용 전/후
2. 객체 이동시 ConstantBuffer 사용 전/후
3. 연산순서 Scale -> Transform