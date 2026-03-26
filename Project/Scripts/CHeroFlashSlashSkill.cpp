#include "pch.h"
#include "CHeroFlashSlashSkill.h"
#include "CProjectileScript.h"
#include "CMonsterScript.h"

CHeroFlashSlashSkill::CHeroFlashSlashSkill(UINT _Type)
	: CSkillScript(_Type)
	, m_Range(600.f, 400.f)
	, m_Speed(5.f)
{
}

CHeroFlashSlashSkill::CHeroFlashSlashSkill(const CHeroFlashSlashSkill& _Other)
	: CSkillScript(_Other)
{
}

CHeroFlashSlashSkill::~CHeroFlashSlashSkill()
{
}

void CHeroFlashSlashSkill::Detect(DWORD_PTR _Col, DWORD_PTR _Other)
{
	CCollider2D* pOther = (CCollider2D*)_Other;
	if (pOther->GetOwner()->GetLayerIdx() != (int)LAYER_INDEX::MONSTER)
		return;
	if (pOther->GetOwner()->GetScript<CMonsterScript>() == nullptr)
		return;

	Vector3 vMonsterPos = Vector3(pOther->GetFinalPos().x, pOther->GetFinalPos().y, 0.f); 
	
	// 좌우만 눌렀을 때는 일자 돌진
	if (!m_Dir[1] && !m_Dir[2])
		return;

	// 처음 감지된 몬스터는 바로 목적지로 설정
	if (m_Dir[0] == false)
	{
		SetDestination(vMonsterPos);
		m_Dir[0] = true;
		return;
	}


	if (m_Dir[3])		// 좌
	{
		if (m_Destination.x > vMonsterPos.x)
			SetDestination(vMonsterPos);
	}
	else if (m_Dir[4])	// 우
	{
		if (m_Destination.x < vMonsterPos.x)
			SetDestination(vMonsterPos);
	}

	
	if (m_Dir[1])		// 상
	{
		if (m_Destination.y < vMonsterPos.y)
			SetDestination(vMonsterPos);
	}
	else if (m_Dir[2])	// 하
	{
		if (m_Destination.y > vMonsterPos.y)
			SetDestination(vMonsterPos);
	}
}

void CHeroFlashSlashSkill::Hit(DWORD_PTR _Col, DWORD_PTR _Other)
{
	CCollider2D* pOther = (CCollider2D*)_Other;

	CMonsterScript* pMonster = pOther->GetOwner()->GetScript<CMonsterScript>();
	if (pMonster)
	{
		pMonster->Damage(10);
		PlayHitAnim(m_HitFlip, pMonster->GetOwner()->Transform()->GetWorldPos(), 1);
	}
}

void CHeroFlashSlashSkill::Use()
{
	CSkillScript::Use();

	GetOwner()->FlipbookPlayer()->Play(7, 10.f, false);

	m_Ratio = DT;

	// 상하좌우 정보 업데이트
	for (int i = 0; i < 5; ++i)
		m_Dir[i] = false;

	if (KEY_PRESSED(KEY::UP))
		m_Dir[1] = true;
	if (KEY_PRESSED(KEY::DOWN))
		m_Dir[2] = true;
	if (KEY_PRESSED(KEY::LEFT))
		m_Dir[3] = true;
	if (KEY_PRESSED(KEY::RIGHT))
		m_Dir[4] = true;


	// 8방향 돌진 목적지 설정
	Vector3 vPos;
	if (KEY_PRESSED(KEY::LEFT))
		vPos.x -= m_Range.x / 2.f;
	else if (KEY_PRESSED(KEY::RIGHT))
		vPos.x += m_Range.x / 2.f;

	if (KEY_PRESSED(KEY::UP))
		vPos.y += m_Range.y / 2.f;
	else if (KEY_PRESSED(KEY::DOWN))
		vPos.y -= m_Range.y / 2.f;
	SetDestination(GetOwner()->Transform()->GetWorldPos() + 2 * vPos);

	// 돌진 방향으로 몬스터 추적 범위 설정
	m_Detect->SetActive(true);
	m_Detect->Transform()->SetRelativePos(vPos.x < 0 ? vPos.x : -vPos.x, vPos.y, 0.f);

	m_HitBox->SetActive(true);

	// 이동 시 물리 미적용
	GetOwner()->RigidBody2D()->UseGravity(false);
}

void CHeroFlashSlashSkill::End()
{
	CSkillScript::End();

	// 스킬 초기화
	m_Detect->SetActive(false);
	m_HitBox->SetActive(false);
	GetOwner()->RigidBody2D()->UseGravity(true);
}

void CHeroFlashSlashSkill::SkillKeyTap()
{
	Use();
}

void CHeroFlashSlashSkill::Init()
{
	m_AttackFlip = CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\HeroFlashSlash_Attack.flip");
	m_HitFlip = CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\HeroFlashSlash_Hit.flip");
	m_MoveFlip = CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\HeroFlashSlash_Move.flip");
}

void CHeroFlashSlashSkill::Begin()
{
	// Detect Object
	m_Detect = CreateTriggerObject();
	m_Detect->SetName(L"FlashSlash Detect");
	m_Detect->SetLayerIdx((int)LAYER_INDEX::PLAYER_OBJECT);
	m_Detect->SetActive(false);

	m_Detect->Transform()->UseIndependentScale(true);
	m_Detect->Transform()->SetRelativeScale(Vector3(m_Range.x, m_Range.y, 0.f));

	CProjectileScript* pScript = new CProjectileScript;
	pScript->BindDelegate(this, (SCRIPT_DELEGATE_2)&CHeroFlashSlashSkill::Detect, nullptr, nullptr);
	m_Detect->AddComponent(pScript);

	GetOwner()->AddChild(m_Detect);

	// Hit Object
	m_HitBox = CreateTriggerObject();
	m_HitBox->SetName(L"FlashSlash HitBox");
	m_HitBox->SetLayerIdx((int)LAYER_INDEX::PLAYER_OBJECT);
	//m_HitBox->SetActive(false);

	m_HitBox->Transform()->UseIndependentScale(true);
	m_HitBox->Transform()->SetRelativePos(Vector3(0.f, 32.f, 0.f));
	m_HitBox->Transform()->SetRelativeScale(Vector3(m_Range.x / 5.f, m_Range.y / 3.f, 0.f));

	pScript = new CProjectileScript;
	pScript->BindDelegate(this, (SCRIPT_DELEGATE_2)&CHeroFlashSlashSkill::Hit, nullptr, nullptr);
	m_HitBox->AddComponent(pScript);

	GetOwner()->AddChild(m_HitBox);

	// Move Effect Object
	m_MoveEff = CreateAnimObject();
	m_MoveEff->SetName(L"FlashSlash Move");
	m_MoveEff->SetLayerIdx((int)LAYER_INDEX::OBJECT_1);
	m_MoveEff->SetActive(false);

	m_MoveEff->FlipbookPlayer()->AddFlipbook(0, m_MoveFlip);
	m_MoveEff->FlipbookPlayer()->Play(0, 10.f, false);
	CreateObject(m_MoveEff, (int)LAYER_INDEX::OBJECT_1, false);
}

void CHeroFlashSlashSkill::Tick()
{
	if (IsStarted())
	{
		m_Speed = 20.f;
		m_Ratio += m_Ratio / m_Speed;
		Vector3 vNextPos = Vector3::Lerp(GetOwner()->Transform()->GetWorldPos(), m_Destination, m_Ratio);
		GetOwner()->Transform()->SetWorldPos(vNextPos);

		if (m_Ratio >= 1.f)
			End();
	}
}
