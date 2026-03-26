#include "pch.h"
#include "func.h"

#include "CProjectileScript.h"

CProjectileScript* CreateProjectile()
{
	CGameObject* pProjectile = CreateTriggerObject();
	pProjectile->SetName(L"Projectile");

	pProjectile->AddComponent(new CRigidBody2D);
	pProjectile->RigidBody2D()->UseGravity(false);

	CProjectileScript* pScript = new CProjectileScript;
	pProjectile->AddComponent(pScript);

	return pScript;
}

#include <Engine/CLevelMgr.h>
#include "CPlayerScript.h"
void PlayerDamage(int _Damage, Vector2 _Dir)
{
	CGameObject* pPlayer = CLevelMgr::GetInst()->GetCurrentPlayer();
	if (!pPlayer)
		return;

	pPlayer->GetScript<CPlayerScript>()->Damage(_Dir, _Damage);
}

void PlayerDamageByPercent(float _Per, Vector2 _Dir)
{
	CGameObject* pPlayer = CLevelMgr::GetInst()->GetCurrentPlayer();
	if (!pPlayer)
		return;

	pPlayer->RigidBody2D()->AddImpulse(Vector2(_Dir.x, _Dir.y));
	pPlayer->GetScript<CPlayerScript>()->DamageByPercent(_Per);
}

Vector2 GetKnockbackDir(CCollider2D* _Hitted, CCollider2D* _Other, float _Power)
{
	float power = (_Hitted->Transform()->GetWorldPos() - _Other->Transform()->GetWorldPos()).Normalize().x * _Power;
	return Vector2(power, 0.f);
}
