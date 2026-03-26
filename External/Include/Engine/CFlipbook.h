#pragma once
#include "CAsset.h"
#include "CSprite.h"
class CFlipbook :
    public CAsset
{
private:
    vector<Ptr<CSprite>> m_vecSprite;
    bool m_bLoop;
    Vector2 m_AnimOffset;

public:
    void AddSprite(Ptr<CSprite> _Sprite) { m_vecSprite.push_back(_Sprite); }
    UINT GetMaxSprite() { return (UINT)m_vecSprite.size(); }
    bool GetLoop() { return m_bLoop; }
    void SetLoop(bool _Loop) { m_bLoop = _Loop; }
    Vector2 GetAnimOffset() { return m_AnimOffset; }
    void SetAnimOffset(Vector2 _Offset) { m_AnimOffset = _Offset; }

    Ptr<CSprite> GetSprite(int _Idx) { return _Idx < m_vecSprite.size() ? m_vecSprite[_Idx] : nullptr; }
    vector<Ptr<CSprite>>& GetSprites() { return m_vecSprite; }

private:
    virtual int Load(const wstring& _FilePath) override;

public:
    virtual int Save(const wstring& _strFilePath) override;

public:
    CLONE_DISABLE(CFlipbook);
    CFlipbook(bool _bEngineRes = false);
    ~CFlipbook();
};

