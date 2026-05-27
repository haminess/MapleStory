#include "pch.h"
#include "CPlayerScript.h"
#include "CMissileScript.h"
#include "CPlatformScript.h"
#include "CPlatformerScript.h"
#include "CLevelScript.h"
#include "CPortalScript.h"
#include "CSkillManager.h"
#include "CCameraScript.h"
#include "CLucidLittleButterflyScript.h"
#include "CSliderUI.h"
#include "CTextUI.h"

#include "CHeroSlashBlastSkill.h"
#include "CHeroWarLeapSkill.h"
#include "CHeroRagingBlowSkill.h"
#include "CHeroFlashSlashSkill.h"
#include "CHeroSwordOfBurningSoulSkill.h"
#include "CHeroComboAttackSkill.h"
#include "CProjectileScript.h"

#include "CPlayerAttackState.h"

#include <Engine/CTaskMgr.h>
#include <Engine/CRigidBody2D.h>
#include <Engine/CSprite.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CRenderMgr.h>


CPlayerScript::CPlayerScript()
	: CLifeScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_PlayerSpeed(250.f)
	, m_PaperBurnIntence(0.f)
	, m_Platform(nullptr)
	, m_IsRope(false)
	, m_fBlinkAccTime(0.f)
	, m_fBlinkTime(1.f)
	, m_fBlinkInterval(0.05f)
	, m_bIsBlinking(false)
	, m_AnimSpeed(5.f)
{
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::FLOAT, "Player Speed", &m_PlayerSpeed });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::TEXTURE, "Test Texture", &m_TargetTex });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::PREFAB, "Missile", &m_Prefab });
}


CPlayerScript::~CPlayerScript()
{
	CLevelMgr::GetInst()->RegisterPlayer(nullptr);
}


void CPlayerScript::SaveComponent(FILE* _File)
{
	fwrite(&m_PlayerSpeed, sizeof(float), 1, _File);
	fwrite(&m_PaperBurnIntence, sizeof(float), 1, _File);
	SaveAssetRef(m_TargetTex, _File);
}

void CPlayerScript::LoadComponent(FILE* _File)
{
	fread(&m_PlayerSpeed, sizeof(float), 1, _File);
	fread(&m_PaperBurnIntence, sizeof(float), 1, _File);
	LoadAssetRef(m_TargetTex, _File);
}



void CPlayerScript::Move()
{
	// 기본 이동은 SetVelocity, 추가 물리힘은 AddImpulse 사용
	if (KEY_PRESSED(KEY::LEFT))
		RigidBody2D()->SetVelocity(Vector3(-m_PlayerSpeed, 0.f, 0.f));
	if (KEY_PRESSED(KEY::RIGHT))
		RigidBody2D()->SetVelocity(Vector3(m_PlayerSpeed, 0.f, 0.f));
}

void CPlayerScript::Jump()
{
	static bool bDoubleJump = false;

	// 아래 점프
	if (KEY_PRESSED(KEY::DOWN) && KEY_TAP(KEY::ALT) &&
		GetOwner()->RigidBody2D()->IsGround())
	{
		CPlatformerScript* pPlatformer = (CPlatformerScript*)GetOwner()->GetScript<CPlatformerScript>();
		if (pPlatformer && pPlatformer->GetCurPlatform() && pPlatformer->GetCurPlatform()->IsDownable())
			GetOwner()->RigidBody2D()->SetGround(false);

		// !) 아래 점프할 때 위로 튕겨주면 다시 원래 플랫폼에 안착하는 문제
		//GetOwner()->RigidBody2D()->AddVelociy(Vector3(0.f, 100.f, 0.f));
	}

	// 윗 점프
	else if (KEY_TAP(KEY::SPACE) || KEY_TAP(KEY::ALT))
	{
		if (GetOwner()->RigidBody2D()->IsGround())
		{
			GetOwner()->RigidBody2D()->Jump();
		}
		else if (!bDoubleJump)
		{
			GetOwner()->RigidBody2D()->Jump();
			bDoubleJump = true;
		}
	}

	if (GetOwner()->RigidBody2D()->IsGround())
	{
		bDoubleJump = false;
	}

	// 이단점프 막기

	// 아래로 점프
}

void CPlayerScript::Attack()
{
	if (KEY_TAP(KEY::CTRL))
	{
		BeginAttack();
	}
}


void CPlayerScript::Damage(Vector2 _AttackPos, int _Power)
{
	CLifeScript::Damage(_Power);

	// alert 애니메이션 출력

	// 캐릭터 깜빡거림 렌더링

	// 종료 타이머 설정

	// 깜빡임 시작
	m_bIsBlinking = true;
	m_fBlinkAccTime = 0.f;

	// 깜빡임 종료를 위한 타이머 설정
	Invoke(std::bind(&CPlayerScript::EndHit, this), 1.f, false);
}

void CPlayerScript::DamageByPercent(float _Per)
{
	CLifeScript::DamageByPercent(_Per);

	// 깜빡임 시작
	m_bIsBlinking = true;
	m_fBlinkAccTime = 0.f;

	// 깜빡임 종료를 위한 타이머 설정
	CTaskMgr::GetInst()->SetTimer(this, (SCRIPT_DELEGATE)&CPlayerScript::EndHit, 1.f, false);
}


void CPlayerScript::EndHit()
{
	m_bIsBlinking = false;

	// 알파값 복구
	if (m_PlayerModel && m_PlayerModel->FlipbookPlayer())
	{
		m_PlayerModel->FlipbookPlayer()->SetFlipbookAlpha(1.f);
	}
}

void CPlayerScript::BeginAttack()
{
	// 공격 애니메이션 출력

	// 공격 자식 오브젝트 활성화
	ActivateObject(m_Attack);

	// 공격 정보에 따라 Offset, Scale 설정

	// 종료 타이머 설정
	Invoke(std::bind(&CPlayerScript::EndAttack, this), 1.0f, false);
}


void CPlayerScript::EndAttack()
{
	DeactivateObject(m_Attack);
	GetOwner()->FlipbookPlayer()->Play(1, m_AnimSpeed, true);
}

void CPlayerScript::Animation()
{
	CFlipbookPlayer* pFP = GetOwner()->FlipbookPlayer();
	if (!pFP)
		return;

	CRigidBody2D* pRB = RigidBody2D();
	if (!pRB)
		return;

	Ptr<CFlipbook> pCurFlipbook = pFP->GetCurFlipbook();

	const bool bGround = pRB->IsGround();
	const bool bMoving = pRB->GetVelocity().Length() > 0.f;
	const bool bRope = IsRope();

	// 방향 전환
	if (KEY_TAP(KEY::LEFT))
		Transform()->SetRelativeRotation(0.f, 0.f, 0.f);

	if (KEY_TAP(KEY::RIGHT))
		Transform()->SetRelativeRotation(0.f, Radian(180.f), 0.f);

	// 공격 애니메이션 재생 중이면 끝날 때까지 다른 애니메이션으로 덮지 않음
	if (pCurFlipbook == pFP->GetFlipbook(6) &&
		!pFP->IsLoop() &&
		!pFP->IsFinishedOnce())
	{
		return;
	}

	// 줄타기
	if (bRope)
	{
		if (pCurFlipbook != pFP->GetFlipbook(5))
			pFP->Play(5, m_AnimSpeed, true);

		return;
	}

	// 공격
	if (KEY_TAP(KEY::CTRL))
	{
		pFP->Play(6, m_AnimSpeed, false);
		return;
	}

	// 공중 상태
	if (!bGround)
	{
		if (pCurFlipbook != pFP->GetFlipbook(3))
			pFP->Play(3, m_AnimSpeed, true);

		return;
	}

	// 점프
	if (KEY_TAP(KEY::SPACE) || KEY_TAP(KEY::ALT))
	{
		pFP->Play(3, m_AnimSpeed, true);
		return;
	}

	// 눕기
	if (KEY_PRESSED(KEY::DOWN))
	{
		if (pCurFlipbook != pFP->GetFlipbook(4))
			pFP->Play(4, m_AnimSpeed, true);

		return;
	}

	// 걷기
	if (KEY_PRESSED(KEY::LEFT) || KEY_PRESSED(KEY::RIGHT))
	{
		if (pCurFlipbook != pFP->GetFlipbook(2))
			pFP->Play(2, m_AnimSpeed, true);

		return;
	}

	// 기본 대기
	if (pCurFlipbook != pFP->GetFlipbook(1))
	{
		pFP->Play(1, m_AnimSpeed, true);
	}
}

void CPlayerScript::MoveToPortal(int _PortalID)
{
	if (_PortalID >= 0)
		m_MovePortalNum = _PortalID;

	// 없으면 Find
	if (!m_LevelScript)
	{
		CGameObject* pObject = CLevelMgr::GetInst()->FindObjectByName(L"LevelMgr");
		if (pObject) m_LevelScript = pObject->GetScript<CLevelScript>();
	}

	if (!m_LevelScript || m_LevelScript->GetPortal(m_MovePortalNum) == nullptr)
		return;

	CPortalScript* pPortal = m_LevelScript->GetPortal(m_MovePortalNum);
	if (!pPortal)
		return;

	// 포탈로 이동
	Vector3 vMovePos = pPortal->GetOwner()->Transform()->GetWorldPos();
	vMovePos.y -= pPortal->Transform()->GetRelativeScale().y / 2.f;

	GetOwner()->Transform()->SetWorldPos(vMovePos);
	RigidBody2D()->SetGround(false);
}

void CPlayerScript::ProjectileTestFunc(DWORD_PTR _Collider, DWORD_PTR _OtherCollider)
{
	CCollider2D* pMonster = (CCollider2D*)_OtherCollider;
	if (pMonster && pMonster->GetOwner()->GetName() == L"Monster")
	{
		DEBUG_LOG("몬스터를 맞췄다!");
	}
}

void CPlayerScript::Dead()
{
	// 플레이어 죽음
}

void CPlayerScript::Init()
{
	CLevelMgr::GetInst()->RegisterPlayer(GetOwner());
	if (!StateMachine())
	{
		GetOwner()->AddComponent(new CStateMachine);
		StateMachine()->AddState(L"AttackState", new CPlayerAttackState);
		StateMachine()->AddState(L"IdleState", new CPlayerIdleState);
	}


	if (GetOwner()->FlipbookPlayer() == nullptr)
		GetOwner()->AddComponent(new CFlipbookPlayer);
	GetOwner()->FlipbookPlayer()->AddFlipbook(1, FIND_ANIM(L"Flipbook\\HeroStand.flip"));
	GetOwner()->FlipbookPlayer()->AddFlipbook(2, FIND_ANIM(L"Flipbook\\HeroWalk.flip"));
	GetOwner()->FlipbookPlayer()->AddFlipbook(3, FIND_ANIM(L"Flipbook\\HeroJump.flip"));
	GetOwner()->FlipbookPlayer()->AddFlipbook(4, FIND_ANIM(L"Flipbook\\HeroProne.flip"));
	GetOwner()->FlipbookPlayer()->AddFlipbook(5, FIND_ANIM(L"Flipbook\\HeroRope.flip"));
	GetOwner()->FlipbookPlayer()->AddFlipbook(6, FIND_ANIM(L"Flipbook\\HeroSwingO1.flip"));
	GetOwner()->FlipbookPlayer()->AddFlipbook(7, FIND_ANIM(L"Flipbook\\HeroStabOF.flip"));
}

void CPlayerScript::Begin()
{
	GetOwner()->SetLayerIdx((int)LAYER_INDEX::PLAYER);
	m_PlayerModel = GetOwner();

	RigidBody2D()->SetMaxSpeed(1000.f);

	CLevelMgr::GetInst()->RegisterPlayer(GetOwner());

	// 카메라 추적
	CGameObject* pCamera = CLevelMgr::GetInst()->FindObjectByName(L"MainCamera");
	pCamera->GetScript<CCameraScript>()->SetTarget(GetOwner());

	m_Map = CLevelMgr::GetInst()->FindObjectByName(L"Map");

	CGameObject* pObject = CLevelMgr::GetInst()->FindObjectByName(L"LevelMgr");
	if (pObject) m_LevelScript = pObject->GetScript<CLevelScript>();

	CSkillManager* pSM = GetOwner()->GetScript<CSkillManager>();
	if (pSM)
	{
		pSM->RegisterSkill(KEY::ALT, (CSkillScript*)GetOwner()->AddComponent(new CHeroWarLeapSkill));
		pSM->RegisterSkill(KEY::CTRL, (CSkillScript*)GetOwner()->AddComponent(new CHeroRagingBlowSkill));
		pSM->RegisterSkill(KEY::X, (CSkillScript*)GetOwner()->AddComponent(new CHeroComboAttackSkill));
		pSM->RegisterSkill(KEY::LSHIFT, (CSkillScript*)GetOwner()->AddComponent(new CHeroFlashSlashSkill));
		pSM->RegisterSkill(KEY::C, (CSkillScript*)GetOwner()->AddComponent(new CHeroSwordOfBurningSoulSkill));
		//pSM->RegisterSkill(KEY::CTRL, new CHeroRagingBlowSkill);
		//pSM->RegisterSkill(KEY::LSHIFT, new CHeroFlashSlashSkill);
		//pSM->RegisterSkill(KEY::Z, new CHeroSwordOfBurningSoulSkill);
		//pSM->RegisterSkill(KEY::X, new CHeroComboAttackSkill);
	}

	MoveToPortal();

	CLevelMgr* pLevel = CLevelMgr::GetInst();
	CGameObject* pFind = nullptr;

	pFind = pLevel->FindObjectByName(L"UI_PlayerHP");
	if (pFind)
		m_HPSlider = pFind->GetScript<CSliderUI>();

	pFind = pLevel->FindObjectByName(L"Text_PlayerHP");
	if (pFind)
		m_HPText = pFind->GetScript<CTextUI>();

	pFind = pLevel->FindObjectByName(L"UI_PlayerMP");
	if (pFind)
		m_MPSlider = pFind->GetScript<CSliderUI>();

	pFind = pLevel->FindObjectByName(L"Text_PlayerMP");
	if (pFind)
		m_MPText = pFind->GetScript<CTextUI>();
}

#include "CLucidNightmareButterflyScript.h"
#include "CLucidLittleButterflyBombScript.h"
#include "CLucidGolemScript.h"
#include "CLucidToadstoolScript.h"
#include "CHeroSwordOfBurningSoulScript.h"
#include "CDamageSkinScript.h"
#include <Engine/CKeyMgr.h>

void CPlayerScript::Tick()
{

	if (KEY_TAP(KEY::MBTN))
	{
		static Ptr<CPrefab> pPrefab = FIND_PREFAB(L"Prefab\\Monster_Cleaner.pref");

		CGameObject* pObj = pPrefab->Instantiate(); // new CGameObject;
		//pObj->AddComponent(new CHeroSwordOfBurningSoulScript);

		pObj->Transform()->SetWorldPos(Vector3(CKeyMgr::GetInst()->GetMouseWorldPos().x, CKeyMgr::GetInst()->GetMouseWorldPos().y, 0.f));

		CreateObject(pObj, (int)LAYER_INDEX::MONSTER, true);
	}
	if (KEY_TAP(KEY::RBTN))
	{
		static Ptr<CPrefab> pPrefab = FIND_PREFAB(L"Prefab\\DamageSkin.pref");

		CGameObject* pObj = pPrefab->Instantiate(); // new CGameObject;
		pObj->Transform()->SetWorldPos(Vector3(CKeyMgr::GetInst()->GetMouseWorldPos().x, CKeyMgr::GetInst()->GetMouseWorldPos().y, 0.f));

		CDamageSkinScript* pDamage = pObj->GetScript<CDamageSkinScript>();
		if (pDamage)
		{
			static int cnt = 0;
			pDamage->Play(cnt++, pObj->Transform()->GetWorldPos(), 2.f, 15.f);
		}

		CreateObject(pObj, (int)LAYER_INDEX::EFFECT, true);
	}

	if (KEY_PRESSED(KEY::CTRL))
	{
		if (KEY_TAP(KEY::ENTER))
		{
			ChangeLevel(L"Lacheln_Boss1");
		}
	}

	m_PlayerPos = Transform()->GetWorldPos();

	// 플레이어 위치 라인 콜라이더
	Vector3 vPrevPos = Transform()->GetPrevPos();
	Vector3 vWorldPos = Transform()->GetWorldPos();

	Move();
	Jump();
	Animation();


	// 깜빡임 처리
	if (m_bIsBlinking)
	{
		m_fBlinkAccTime += DT;

		if (m_PlayerModel && m_PlayerModel->FlipbookPlayer())
		{
			Ptr<CSprite> pSprite = m_PlayerModel->FlipbookPlayer()->GetCurrentSprite();
			if (pSprite != nullptr)
			{
				// sin 함수를 사용하여 부드러운 깜빡임
				float fAlpha = (sinf(m_fBlinkAccTime * 20.f) + 1.f) * 0.5f;
				// 또는 단순 깜빡임
				// float fAlpha = ((int)(m_fBlinkAccTime / m_fBlinkInterval) % 2) ? 1.f : 0.f;

				m_PlayerModel->FlipbookPlayer()->SetFlipbookAlpha(fAlpha);
			}
		}
	}

	// 미사일 발사
	if (KEY_TAP(KEY::TAB))
	{
		Instantiate(m_Prefab, Transform()->GetWorldPos(), 0);
	}

	if (m_Map)
	{
		Vector3 vPlayerPos = Transform()->GetWorldPos();
		Vector3 vMapPos = m_Map->Transform()->GetRelativePos();
		Vector3 vMapHalfSize = m_Map->Transform()->GetRelativeScale() / 2.f;

		bool bOutOfMap =
			vPlayerPos.x < vMapPos.x - vMapHalfSize.x ||
			vPlayerPos.x > vMapPos.x + vMapHalfSize.x ||
			vPlayerPos.y < vMapPos.y - vMapHalfSize.y ||
			vPlayerPos.y > vMapPos.y + vMapHalfSize.y;

		if (bOutOfMap)
		{
			int a = 0;
			MoveToPortal();
			// Respawn();
		}
	}


	// UI
	if (m_HPSlider && m_HPText)
	{
		m_HPSlider->SetRatio((float)GetHP() / GetMaxHP());
		m_HPText->SetText(std::to_wstring(GetHP()) + L" / " + std::to_wstring(GetMaxHP()));
	}
	if (m_MPSlider && m_MPText)
	{
		m_MPSlider->SetRatio((float)GetMP() / GetMaxMP());
		m_MPText->SetText(std::to_wstring(GetMP()) + L" / " + std::to_wstring(GetMaxMP()));
	}

}



void CPlayerScript::SetRope(bool _Rope)
{
	m_IsRope = _Rope;
	RigidBody2D()->UseGravity(!_Rope);
}

