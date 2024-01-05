#include "pch.h"
#include "CAnimator2D.h"

#include "CAnim.h"

CAnimator2D::CAnimator2D()
	: Super(COMPONENT_TYPE::ANIMATOR2D)
{
}

CAnimator2D::~CAnimator2D()
{
	Delete_Map(m_mapAnim);
}

void CAnimator2D::FinalTick()
{
	m_curAnim->FinalTick();
}

void CAnimator2D::UpdateData()
{
	m_curAnim->UpdateData();
}

void CAnimator2D::Clear()
{
	CAnim::Clear();
}

void CAnimator2D::Create(const wstring& _strKey, Ptr<CTexture> _altasTex, Vec2 _leftTop
						, Vec2 _vSliceSize, Vec2 _offsetSize, Vec2 _background, int _frmCount, float _FPS)
{
	CAnim* pAnim = FindAnim(_strKey);
	assert(!pAnim);

	pAnim = new CAnim;
	pAnim->Create(this, _altasTex, _leftTop, _vSliceSize, _offsetSize, _background, _frmCount, _FPS);
	m_mapAnim.insert(make_pair(_strKey, pAnim));
}

CAnim* CAnimator2D::FindAnim(const wstring& _strAnimName)
{
	map<wstring, CAnim*>::iterator iter = m_mapAnim.find(_strAnimName);

	if (iter == m_mapAnim.end())
		return nullptr;

	return iter->second;
}

void CAnimator2D::Play(const wstring& _strAnimName)
{
	CAnim* pAnim = FindAnim(_strAnimName);
	if (nullptr == pAnim)
		return;

	m_curAnim = pAnim;
}
