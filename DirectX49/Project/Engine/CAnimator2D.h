#pragma once
#include "CComponent.h"

#include "CTexture.h"

class CAnim;

class CAnimator2D :
    public CComponent
{
    using Super = CComponent;

public:
    CAnimator2D();
    ~CAnimator2D();

private:
    map<wstring, CAnim*>    m_mapAnim;
    CAnim*                  m_curAnim;
    bool                    m_bRepeat;

public:
    virtual void FinalTick() override;

    void UpdateData();
    static void Clear();

    // _LeftTop, _SliceSize, _Offset : Pixel Unit
    void Create(const wstring& _strKey, Ptr<CTexture> _altasTex, Vec2 _leftTop
                , Vec2 _vSliceSize, Vec2 _offsetSize, Vec2 _background, int _frmCount, float _FPS);

    CAnim* FindAnim(const wstring& _strAnimName);
    void Play(const wstring& _strAnimName);

};

