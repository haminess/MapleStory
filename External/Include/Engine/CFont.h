#pragma once
#include "CAsset.h"
#include "CFontMgr.h"

class CFont :
    public CAsset
{
private:
    wstring m_FontName;
    IFW1FontWrapper* m_FontWrapper;

public:
    virtual int Save(const wstring& _FilePath) override;

private:
    virtual int Load(const wstring& _strFilePath) override { return S_OK; }
    
public:
    int Create(IFW1Factory* _Factory, const wstring& _FontKey);

public:
    virtual CFont* Clone() override;
private:
    CFont(const CFont& _Other);
public:
    CFont(bool _EngineRes = false);
    ~CFont();

    friend class CFontMgr;
};

