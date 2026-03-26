#pragma once
#include "CEntity.h"
#include "CGameObject.h"

// C##은 Type에 C를 붙여주겠다는 의미
#define GET_OTHER_COMPONENT(Type) C##Type* Type() { return m_Owner->Type(); }


class CComponent :
    public CEntity
{
public:
    static COMPONENT_TYPE GetComponentType(const wstring& _StrType);
    static CComponent* GetComponent(COMPONENT_TYPE);

private:
    const COMPONENT_TYPE m_Type;
    CGameObject* m_Owner;
    bool m_Active;
    bool m_Initialized;
    
public:
    COMPONENT_TYPE GetType() { return m_Type; }
    CGameObject* GetOwner() { return m_Owner; }
    void Active() { m_Active = true; }
    void SetActive(bool _Set) { m_Active = _Set; }
    void Deactive() { m_Active = false; }
    bool IsActive() { return m_Active; }
    bool IsInitialized() { return m_Initialized; }

    virtual void Activate() {}
    virtual void Deactivate() {}

    GET_OTHER_COMPONENT(Transform);
    GET_OTHER_COMPONENT(MeshRender);
    GET_OTHER_COMPONENT(Camera);
    GET_OTHER_COMPONENT(Collider2D);
    GET_OTHER_COMPONENT(FlipbookPlayer);
    GET_OTHER_COMPONENT(TileMap);
    GET_OTHER_COMPONENT(Light2D);
    GET_OTHER_COMPONENT(RigidBody2D);
    GET_OTHER_COMPONENT(ParticleSystem);
    GET_OTHER_COMPONENT(StateMachine);

public:
    virtual void Init() {};
    virtual void Begin() {};
    virtual void Tick() {};
    virtual void Destroy() {};
    virtual void FinalTick() = 0;

public:
    virtual CComponent* Clone() = 0;

    virtual void SaveToLevel(FILE* _File) override;
    virtual void LoadFromLevel(FILE* _File) override;

    virtual void SaveComponent(FILE* _File) = 0;
    virtual void LoadComponent(FILE* _File) = 0;

    CComponent(COMPONENT_TYPE _Type);
    ~CComponent();

    friend class CGameObject;
};

typedef void(CComponent::*COM_DELEGATE)(void);