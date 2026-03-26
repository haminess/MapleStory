#include "pch.h"
#include "CLucidFlowerExplosionScript.h"
#include "CProjectileScript.h"
#include "CPlayerScript.h"

#include <Engine/CTaskMgr.h>

CLucidFlowerExplosionScript::CLucidFlowerExplosionScript()
	: CScript((UINT)SCRIPT_TYPE::LUCIDFLOWEREXPLOSIONSCRIPT)
{
}

CLucidFlowerExplosionScript::CLucidFlowerExplosionScript(const CLucidFlowerExplosionScript& _Other)
	: CScript(_Other)
{
}

CLucidFlowerExplosionScript::~CLucidFlowerExplosionScript()
{
}

void CLucidFlowerExplosionScript::Init()
{
	const wstring animName[4] = { L"FlowerExplosion_MS", L"FlowerExplosion_M", L"FlowerExplosion_L", L"FlowerExplosion_XL" };
	for (int i = 0; i < 4; ++i)
	{
		m_FlowerAnim[i] = CAssetMgr::GetInst()->FindAsset<CFlipbook>(wstring(L"Flipbook\\") + animName[i].c_str() + wstring(L".flip"));
	}
}

void CLucidFlowerExplosionScript::Begin()
{
	for (int i = 0; i < 8; ++i)
	{
		CGameObject* pProjectile = CreateTriggerObject();
		pProjectile->SetName(L"FlowerExplosion_" + std::to_wstring(i));
		pProjectile->SetLayerIdx((UINT)LAYER_INDEX::MONSTER_OBJECT);

		pProjectile->Collider2D()->SetScale(Vector2(0.4f, 0.8f));

		CProjectileScript* pScript = new CProjectileScript;
		pScript->BindDelegate(this, (SCRIPT_DELEGATE_2)&CLucidFlowerExplosionScript::ProjectileBeginOverlap, nullptr, nullptr);
		pProjectile->AddComponent(pScript);

		pProjectile->FlipbookPlayer()->AddFlipbook(0, m_FlowerAnim[0]);
		pProjectile->FlipbookPlayer()->AddFlipbook(1, m_FlowerAnim[1]);
		pProjectile->FlipbookPlayer()->AddFlipbook(2, m_FlowerAnim[2]);
		pProjectile->FlipbookPlayer()->AddFlipbook(3, m_FlowerAnim[3]);
		pProjectile->FlipbookPlayer()->Play(0, 10, true);
		pProjectile->SetActive(false);

		m_Projectile.push_back(pProjectile);
		CreateObject(pProjectile, (int)LAYER_INDEX::MONSTER_OBJECT, true);
	}

	m_SpawnTimerHandle = CTaskMgr::GetInst()->SetTimer(this, (SCRIPT_DELEGATE)&CLucidFlowerExplosionScript::Spawn, 5.f, true);
}

void CLucidFlowerExplosionScript::Tick()
{
	// 애님 끝나면 비활성화
	if (m_Projectile.size() > 0 &&
		m_Projectile[0]->IsActive() &&
		m_Projectile[0]->FlipbookPlayer()->IsFinishedOnce())
	{
		ClearProjectiles();
	}
}

void CLucidFlowerExplosionScript::Activate()
{
	m_SpawnTimerHandle = CTaskMgr::GetInst()->SetTimer(this, (SCRIPT_DELEGATE)&CLucidFlowerExplosionScript::Spawn, 5.f, true);
}

void CLucidFlowerExplosionScript::Deactivate()
{
	CTaskMgr::GetInst()->StopTimer(m_SpawnTimerHandle);
	m_SpawnTimerHandle = 0;
}

void CLucidFlowerExplosionScript::SaveComponent(FILE* _File)
{
}

void CLucidFlowerExplosionScript::LoadComponent(FILE* _FILE)
{
}


void CLucidFlowerExplosionScript::Spawn()
{
	ClearProjectiles();

	m_Hitted = false;

	int RandIdx = RandomRange(0, LUCID_FE_PATTERN_SIZE - 1);
	const auto& curList = m_Desc[RandIdx];

	for (int i = 0; i < curList.size(); ++i)
	{
		m_Projectile[i]->SetActive(true);
		m_Projectile[i]->Transform()->SetWorldPos(Vector3(curList[i].pos.x, curList[i].pos.y, 0.f));
		m_Projectile[i]->Transform()->SetRelativeRotation(Vector3(0.f, 0.f, curList[i].angle));
		m_Projectile[i]->FlipbookPlayer()->Play(curList[i].size, 10.f, false);
	}

	// 1.5초 뒤에 콜라이더 활성화
	CTaskMgr::GetInst()->SetTimer(this, (SCRIPT_DELEGATE)&CLucidFlowerExplosionScript::ActivateBomb, 1.5f);
}

void CLucidFlowerExplosionScript::ProjectileBeginOverlap(DWORD_PTR _Collider, DWORD_PTR _OtherCollider)
{
	CCollider2D* pProjectile = (CCollider2D*)_Collider;
	CCollider2D* pOther = (CCollider2D*)_OtherCollider;
	
	if (pOther->GetOwner()->GetLayerIdx() == (int)LAYER_INDEX::PLAYER && !m_Hitted)
	{
		PlayerDamageByPercent(0.2f, GetKnockbackDir(pOther, pProjectile));
		PlayHitAnim(m_HitAnim, GetRandomPosAround(pOther->Transform()->GetWorldPos()), 1);
		m_Hitted = true;
	}
}

void CLucidFlowerExplosionScript::ClearProjectiles()
{
	for (int i = 0; i < m_Projectile.size(); ++i)
	{
		m_Projectile[i]->SetActive(false);
		m_Projectile[i]->Collider2D()->SetActive(false);
	}
}

void CLucidFlowerExplosionScript::ActivateBomb()
{
	for (int i = 0; i < m_Projectile.size(); ++i)
	{
		if (!m_Projectile[i]->IsActive())
			continue;

		m_Projectile[i]->Collider2D()->SetActive(true);
	}
}
