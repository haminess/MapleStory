#include "pch.h"
#include "CWallScript.h"
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CWallScript::CWallScript()
	: CScript(SCRIPT_TYPE::WALLSCRIPT)
	, m_Dir(BLOCK_DIR::BOTH)
{
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "Block", &m_Dir });
}

CWallScript::CWallScript(const CWallScript& _Other)
    : CScript(SCRIPT_TYPE::WALLSCRIPT)
    , m_Dir(_Other.m_Dir)
{
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "Block", &m_Dir });
}

CWallScript::~CWallScript()
{
}


void CWallScript::SaveComponent(FILE* _File)
{
    int dir = (int)m_Dir;
    fwrite(&dir, sizeof(int), 1, _File);
}

void CWallScript::LoadComponent(FILE* _File)
{
    int dir = 0;
    fread(&dir, sizeof(int), 1, _File);
    m_Dir = (BLOCK_DIR)dir;
}


void CWallScript::Init()
{
    CGameObject* pObject = GetOwner();
    pObject->SetName(L"WallObject");
    pObject->SetLayerIdx(23);
    if (!pObject->GetComponent(COMPONENT_TYPE::COLLIDER2D)) pObject->AddComponent(new CCollider2D);

    if (!Collider2D()->IsIndependentScale())
    {
        Collider2D()->SetIndependentScale(false);
        Collider2D()->SetScale(Vector2(1.f, 1.f));
    }
}

void CWallScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    if (!_OtherObject->RigidBody2D())
        return;

    Vector3 vVelo = _OtherObject->RigidBody2D()->GetVelocity();
    Vector3 vOtherPos = _OtherObject->Transform()->GetWorldPos();
    Vector3 vWallPos = Transform()->GetWorldPos();
    Vector3 vOtherColliderPos = _OtherCollider->GetFinalPos();
    Vector3 vWallColliderPos = _Collider->GetFinalPos();

    // 콜라이더 크기 구하기
    Vector3 vWallScale = _Collider->GetFinalScale();
    Vector3 vOtherScale = _OtherCollider->GetFinalScale();

    switch (m_Dir)
    {
    case BLOCK_DIR::LEFT:
    {
        // 왼쪽에서 충돌했을 때만 막기
        if (vOtherColliderPos.x < vWallColliderPos.x && vVelo.x > 0.f)
        {
            float pushOutPos = vWallColliderPos.x - (vWallScale.x / 2.f) - (vOtherScale.x / 2.f);
            vOtherPos.x = pushOutPos;
            _OtherObject->Transform()->SetWorldPos(vOtherPos);

            // 오른쪽으로 가려는 속도 제거
            Vector3 vVelo = _OtherObject->RigidBody2D()->GetVelocity();
            if (vVelo.x > 0)
                _OtherObject->RigidBody2D()->SetVelocity(Vector3(0.f, vVelo.y, vVelo.z));
        }
    }
    break;

    case BLOCK_DIR::RIGHT:
    {
        // 오른쪽에서 충돌했을 때만 막기
        if (vOtherColliderPos.x > vWallColliderPos.x && vVelo.x < 0.f)
        {
            float pushOutPos = vWallColliderPos.x + (vWallScale.x / 2.f) + (vOtherScale.x / 2.f);
            vOtherPos.x = pushOutPos;
            _OtherObject->Transform()->SetWorldPos(vOtherPos);

            // 왼쪽으로 가려는 속도 제거
            Vector3 vVelo = _OtherObject->RigidBody2D()->GetVelocity();
            if (vVelo.x < 0)
                _OtherObject->RigidBody2D()->SetVelocity(Vector3(0.f, vVelo.y, vVelo.z));
        }
    }
    break;

    case BLOCK_DIR::BOTH:
    {
        // 왼쪽에서 충돌했을 때만 막기
        if (vOtherColliderPos.x < vWallColliderPos.x && vVelo.x > 0.f)
        {
            float pushOutPos = vWallColliderPos.x - (vWallScale.x / 2.f) - (vOtherScale.x / 2.f);
            vOtherPos.x = pushOutPos;
            _OtherObject->Transform()->SetWorldPos(vOtherPos);

            // 오른쪽으로 가려는 속도 제거
            Vector3 vVelo = _OtherObject->RigidBody2D()->GetVelocity();
            if (vVelo.x > 0)
                _OtherObject->RigidBody2D()->SetVelocity(Vector3(0.f, vVelo.y, vVelo.z));
        }

        // 오른쪽에서 충돌했을 때만 막기
        else if (vOtherColliderPos.x > vWallColliderPos.x && vVelo.x < 0.f)
        {
            float pushOutPos = vWallColliderPos.x + (vWallScale.x / 2.f) + (vOtherScale.x / 2.f);
            vOtherPos.x = pushOutPos;
            _OtherObject->Transform()->SetWorldPos(vOtherPos);

            // 왼쪽으로 가려는 속도 제거
            Vector3 vVelo = _OtherObject->RigidBody2D()->GetVelocity();
            if (vVelo.x < 0)
                _OtherObject->RigidBody2D()->SetVelocity(Vector3(0.f, vVelo.y, vVelo.z));
        }
    }
    break;
    }
}

void CWallScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    if (!_OtherObject->RigidBody2D())
        return;

    Vector3 vVelo = _OtherObject->RigidBody2D()->GetVelocity();
    Vector3 vOtherPos = _OtherObject->Transform()->GetWorldPos();
    Vector3 vWallPos = Transform()->GetWorldPos();
    Vector3 vOtherColliderPos = _OtherCollider->GetFinalPos();
    Vector3 vWallColliderPos = _Collider->GetFinalPos();

    // 콜라이더 크기 구하기
    Vector3 vWallScale = _Collider->GetFinalScale();
    Vector3 vOtherScale = _OtherCollider->GetFinalScale();

    switch (m_Dir)
    {
    case BLOCK_DIR::LEFT:
    {
        // 왼쪽에서 충돌했을 때만 막기
        if (vOtherColliderPos.x <= vWallColliderPos.x - (vWallScale.x / 2.f) && vVelo.x > 0.f)
        {
            float pushOutPos = vWallColliderPos.x - (vWallScale.x / 2.f) - (vOtherScale.x / 2.f);
            vOtherPos.x = pushOutPos;
            _OtherObject->Transform()->SetWorldPos(vOtherPos);

            // 오른쪽으로 가려는 속도 제거
            Vector3 vVelo = _OtherObject->RigidBody2D()->GetVelocity();
            if (vVelo.x > 0)
                _OtherObject->RigidBody2D()->SetVelocity(Vector3(0.f, vVelo.y, vVelo.z));
        }
    }
    break;

    case BLOCK_DIR::RIGHT:
    {
        // 오른쪽에서 충돌했을 때만 막기
        if (vOtherColliderPos.x > vWallColliderPos.x + (vWallScale.x / 2.f) && vVelo.x < 0.f)
        {
            float pushOutPos = vWallColliderPos.x + (vWallScale.x / 2.f) + (vOtherScale.x / 2.f);
            vOtherPos.x = pushOutPos;
            _OtherObject->Transform()->SetWorldPos(vOtherPos);

            // 왼쪽으로 가려는 속도 제거
            Vector3 vVelo = _OtherObject->RigidBody2D()->GetVelocity();
            if (vVelo.x < 0)
                _OtherObject->RigidBody2D()->SetVelocity(Vector3(0.f, vVelo.y, vVelo.z));
        }
    }
    break;

    case BLOCK_DIR::BOTH:
    {
        // 왼쪽에서 충돌했을 때만 막기
        if (vOtherColliderPos.x <= vWallColliderPos.x - (vWallScale.x / 2.f) && vVelo.x > 0.f)
        {
            float pushOutPos = vWallColliderPos.x - (vWallScale.x / 2.f) - (vOtherScale.x / 2.f);
            vOtherPos.x = pushOutPos;
            _OtherObject->Transform()->SetWorldPos(vOtherPos);

            // 오른쪽으로 가려는 속도 제거
            Vector3 vVelo = _OtherObject->RigidBody2D()->GetVelocity();
            if (vVelo.x > 0)
                _OtherObject->RigidBody2D()->SetVelocity(Vector3(0.f, vVelo.y, vVelo.z));
        }

        // 오른쪽에서 충돌했을 때만 막기
        else if (vOtherColliderPos.x > vWallColliderPos.x + (vWallScale.x / 2.f) && vVelo.x < 0.f)
        {
            float pushOutPos = vWallColliderPos.x + (vWallScale.x / 2.f) + (vOtherScale.x / 2.f);
            vOtherPos.x = pushOutPos;
            _OtherObject->Transform()->SetWorldPos(vOtherPos);

            // 왼쪽으로 가려는 속도 제거
            Vector3 vVelo = _OtherObject->RigidBody2D()->GetVelocity();
            if (vVelo.x < 0)
                _OtherObject->RigidBody2D()->SetVelocity(Vector3(0.f, vVelo.y, vVelo.z));
        }
    }
    break;
    }
}