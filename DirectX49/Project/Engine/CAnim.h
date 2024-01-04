#pragma once
#include "CEntity.h"

#include "CTexture.h"

class CAnimator2D;

struct tAnimFrm
{
    Vec2    vLeftTop;
    Vec2    vSlice;
    Vec2    vOffset;
    float   duration;
};

class CAnim :
    public CEntity
{
    using Super = CEntity;

public:
    CAnim();
    ~CAnim();

private:
    CAnimator2D*        m_animator;
    vector<tAnimFrm>    m_vecFrm;
    int                 m_curFrmIdx;
    bool                m_bFinish;
    Ptr<CTexture>       m_atlasTex;
    float               m_accTime;

public:
    void FinalTick();
    void UpdateData();
    static void Clear();

    void Create(CAnimator2D* _animator, Ptr<CTexture> _atlas, Vec2 _vLeftTop
        , Vec2 _vSliceSize, Vec2 _vOffset, int _frmCount, float _FPS);
};

