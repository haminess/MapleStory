#pragma once
#include "CSkillScript.h"
class CHeroSlashBlastSkill :
    public CSkillScript
{
private:
    float   m_Speed;    // 화살 이동 속도

public:
    virtual void Use() override { DEBUG_LOG("공격 Use"); }
    virtual void SkillKeyTap() override { DEBUG_LOG("공격 Enter"); }

    virtual void Init() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _FILE) override;

public:
    CLONE(CHeroSlashBlastSkill);
    CHeroSlashBlastSkill(UINT _Type = (UINT)SCRIPT_TYPE::HEROSLASHBLASTSKILL);
    CHeroSlashBlastSkill(const CHeroSlashBlastSkill& _Other);
    ~CHeroSlashBlastSkill();
};