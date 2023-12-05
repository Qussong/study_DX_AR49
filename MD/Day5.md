23.12.05
- 삼각형 그리기 완료
- 매니저 클래스 추가 (Key, Time, Path)
- Shader 객체 생성
- Layout 생성

## Shader

**_Comment :_**<br>
hlsl 코드를 binary 코드로 컴파일해서 Blob에 저장한다.<br>
쉐이더 코드는 런타임 도중에 컴파일되며, 리소스처럼 취급된다.<br>
Blob은 버퍼와 같은 데이터 저장소로 생각하면된다.<br>
<br><br>

### Vertex Shader 생성과정
Path Mgr를 통해 작성된 쉐이더 파일 경로 불러온다.<br>
D3DCompileFromFile()을 통해 컴파일된 "쉐이더 코드(바이너리 코드)"를 Blob에 저장한다.<br>
Device->CreateVertexShader()를 통해 VS 생성<br>
<br><br>

### Pixel Shader 생성과정
<br><br>

## Rasterizer

화면에 그려질 픽셀의 정보를 찾아온다.<br>
Default 값이 있기에 값 설정을 하지않고 넘어가도 문제는 없다.<br>

### 보간
<br><br>

## Topology
<br><br>

## InputLayout
<br><br>

