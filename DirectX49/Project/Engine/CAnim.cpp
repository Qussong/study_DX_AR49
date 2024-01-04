#include "pch.h"
#include "CAnim.h"

#include "CTimeMgr.h"
#include "CDevice.h"
#include "CConstBuffer.h"

CAnim::CAnim()
	: m_animator(nullptr)
	, m_curFrmIdx(0)
	, m_bFinish(false)
	, m_accTime(0.f)
{
}

CAnim::~CAnim()
{
}

void CAnim::FinalTick()
{
	m_accTime += DT;

	if (m_vecFrm[m_curFrmIdx].duration < m_accTime)
	{
		++m_curFrmIdx;
		if (m_vecFrm.size() <= m_curFrmIdx)
		{
			m_curFrmIdx = 0;
		}
		m_accTime = 0.f;
	}
}

void CAnim::UpdateData()
{
	// 현재 프레임 정보를 상수버퍼로 옮기고 b2 레지스터로 바인딩
	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::ANIM2D_DATA);
	tAnimData2D data = {};
	data.useAnim2D = 1;
	data.vLeftTop = m_vecFrm[m_curFrmIdx].vLeftTop;
	data.vOffset = m_vecFrm[m_curFrmIdx].vOffset;
	data.vSlizeSize = m_vecFrm[m_curFrmIdx].vSlice;

	pCB->SetData(&data);
	pCB->UpdateData();

	// 아틀라스 텍스쳐 t10 에 바인딩
	m_atlasTex->UpdateData(10);
}

void CAnim::Clear()
{
	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::ANIM2D_DATA);
	tAnimData2D data = {};
	data.useAnim2D = 0;

	pCB->SetData(&data);
	pCB->UpdateData();
}

void CAnim::Create(CAnimator2D* _animator, Ptr<CTexture> _atlas, Vec2 _vLeftTop, Vec2 _vSliceSize, Vec2 _vOffset, int _frmCount, float _FPS)
{
	m_animator = _animator;
	m_atlasTex = _atlas;

	for (int i = 0; i < _frmCount; ++i)
	{
		tAnimFrm frm = {};

		frm.vSlice = Vec2(_vSliceSize.x / (float)_atlas->GetWidth(), _vSliceSize.y / (float)_atlas->GetHeight());

		frm.vLeftTop = Vec2(_vLeftTop.x / (float)_atlas->GetWidth() + frm.vSlice.x * i, _vLeftTop.y / (float)_atlas->GetHeight());

		frm.vOffset = Vec2(_vOffset.x / (float)_atlas->GetWidth(), _vOffset.y / (float)_atlas->GetHeight());
		frm.duration = 1.f / _FPS;

		m_vecFrm.push_back(frm);
	}
}
