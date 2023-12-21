23.12.21<br>

---
[2교시]
내용 : 텍스처 마무리하고 블랜드 스테이트 설정도 마무리한다.

## CTexture

### Load
```cpp
int CTexture::Load(const wstring& _texturePath)
{
    wchar_t szExt[20] = {};
    _wsplitpatj_s(strFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 20);

    HRESULT hr = S_OK;

    if(!wscmp(szExt, L"dds") || !wscmp(szExt, L"DDS"))
    {
        hr = LoadFromDDSFile();
    }
    else if(!wscmp(szExt, L"tga") || !wscmp(szExt, L"TGA")
    {
        hr = LoadFromTGAFile();
    }
    else
    {
        // Window Image Component(WIC)
        // Window 애서 잘 아는 확장자들을 통칭한다
        // png, bmp, jpg, jpeg
        hr = LaoadFromWICFile();
    }

    // Load 실패 확인
    CHECK(hr);

    // Direct Tex 의 역할은 아래의 결과물을 불러오는 순간 끝이다.
    m_Image.GetMatadata().width();
    m_Image.GetMatadata().height();
    m_Image.GetMatadata().format();


}
```
_splitpath_s()/_wsplitpath_s() : 들어온 문자를 쪼개준다.<br>
wscmp(szExt, L"png") : 일치할경우 0 을 반환

추가적인 작업 : 
1. Direct 2D Texture 생성(ID3D11Texture2D)
2. ScratchImage 에 있는 데이터를 ID3D11Texture2D 객체로 전달
3. ID3D11Texture2D 를 이용해 Shader Resource View 생성
위의 3단계를 한번에 처리해주는 함수가 존재한다. -> CreateShaderResourceView()

### CreateShaderResourceView()
```cpp
CreateShaderResourceView(device, m_Image.GetImage(), m_Image.GetMetadata(), **m_SRV);
// SRV 얻어오기
m_SRV->GetResource( (ID3D11Resource**)m_Tex2D.GetAddressOf() );
// Tex2D Desc 얻어오기
m_Tex2D->GetDesc(&m_Desc);
```
결과 값으론 ShaderResourceView 만 나온다. 
m_Tex2D 는 결과값으로 나오지 않는다. 
하지만 해당 과정을 통해서 메모리상에 만들어지긴 했다.

## AlphaBlending

```cpp
// CTexuter.cpp

CONTEXT->VSSetShaderResources(_registerNum, 1, m_SRV.GetAddressOf());
CONTEXT->HSSetShaderResources(_registerNum, 1, m_SRV.GetAddressOf());
CONTEXT->DSSetShaderResources(_registerNum, 1, m_SRV.GetAddressOf());
CONTEXT->GSSetShaderResources(_registerNum, 1, m_SRV.GetAddressOf());
CONTEXT->PSSetShaderResources(_registerNum, 1, m_SRV.GetAddressOf());
```

### 쉐이더코드에서 PS의 알파블랜딩 조정함