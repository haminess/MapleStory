#pragma once
#include "CComponent.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CAssetMgr.h"
#include "components.h"


#ifdef _DEBUG
#define SERIALIZABLE(Class) m_ScriptName = #Class; 
#define PROPERTY(Type, Value) m_vecMeta.push_back({#Type, #Value, &Value});
#define PROPERTY_SETNAME(Type, Name, Value) m_vecMeta.push_back({#Type, Name, &Value});
#define PROPERTY_DELEGATE(Type, Value) m_vecMeta.push_back({#Type, Name, &Value});
#define HEADER(Title) m_vecMeta.push_back({"Header", Title, nullptr});
#else
#define SERIALIZABLE(Class) 
#define PROPERTY(Type, Value) 
#define PROPERTY_SETNAME(Type, Name, Value) 
#define PROPERTY_DELEGATE(Type, Value) 
#define HEADER(Title) 
#endif


struct tScriptParam
{
    SCRIPT_PARAM    Param;
    string          Desc;
    void*           pData;
    SCRIPT_DELEGATE Func;
};


class CCollider2D;
class CScript :
    public CComponent
{
#ifdef _DEBUG
protected:
    string m_ScriptName;
    vector<tProperty> m_vecMeta;

public:
    string GetScriptName() { return m_ScriptName; }
    const vector<tProperty>& GetProperties() { return m_vecMeta; }
    const tProperty& GetProperty(int _idx) { return m_vecMeta[_idx]; }
#endif

private:
    // Serialization
    const UINT              m_ScriptType;
    vector<tScriptParam>    m_vecScriptParam;

public:
    UINT GetScriptType() { return m_ScriptType; }
    void AddScriptParam(tScriptParam _Param) { m_vecScriptParam.push_back(_Param); }
    void ClearScriptParam() { m_vecScriptParam.clear(); }
    const vector<tScriptParam>& GetScriptParam() { return m_vecScriptParam; }

public:
    void Instantiate(Ptr<CPrefab> _Pref, Vector3 _WorldPos, int _Layer);
    void Instantiate(Ptr<CPrefab> _Pref);

public:
    virtual void Begin() {}
    virtual void Tick() = 0;
    virtual void FinalTick() final {}

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) {}
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) {}
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) {}

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _FILE) override;

public:
    CScript(UINT _ScriptType);
    CScript(const CScript& _Other);
    ~CScript();
};
