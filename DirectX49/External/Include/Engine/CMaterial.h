#pragma once
#include "CAsset.h"

#include "CTexture.h"
#include "CGraphicsShader.h"

class CMaterial :
    public CAsset
{
public:
    CMaterial();
    ~CMaterial();

public:
    tMtrlConst				m_const;
    Ptr<CTexture>			m_arrTex[(UINT)TEX_PARAM::END];

    Ptr<CGraphicsShader>    m_pShader;


public:
    void                    SetShader(Ptr<CGraphicsShader> _shader) { m_pShader = _shader; }
    Ptr<CGraphicsShader>    GetShader() { return m_pShader; }

    template<typename T>
    void                    SetScalarParam(SCALAR_PARAM _ParamType, const T& _Value);

    void                    SetTexParam(TEX_PARAM _Param, Ptr<CTexture> _pTex);

    void                    UpdateData();
};

template<typename T>
void CMaterial::SetScalarParam(SCALAR_PARAM _ParamType, const T& _Value)
{
	const T* pValue = &_Value;

	switch (_ParamType)
	{
	case INT_0:
	case INT_1:
	case INT_2:
	case INT_3:
		/*	if constexpr (std::is_same_v<T, int>) {

			}*/
		m_const.iArr[_ParamType] = *((int*)pValue);
		break;

	case FLOAT_0:
	case FLOAT_1:
	case FLOAT_2:
	case FLOAT_3:
		m_const.fArr[_ParamType - FLOAT_0] = *((float*)pValue);
		break;

	case VEC2_0:
	case VEC2_1:
	case VEC2_2:
	case VEC2_3:
		m_const.v2Arr[_ParamType - VEC2_0] = *((Vec2*)pValue);


		break;
	case VEC4_0:
	case VEC4_1:
	case VEC4_2:
	case VEC4_3:
		m_const.v4Arr[_ParamType - VEC4_0] = *((Vec4*)pValue);

		break;
	case MAT_0:
	case MAT_1:
	case MAT_2:
	case MAT_3:
		m_const.matArr[_ParamType - MAT_0] = *((Matrix*)pValue);
		break;
	}
}