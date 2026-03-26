#include "pch.h"
#include "CLucidDragonScript.h"
#include "CLucidDragonState_Move.h"
#include "CLucidDragonState_Attack.h"

#include "CPlayerScript.h"

#include <Engine/CTaskMgr.h>

CLucidDragonScript::CLucidDragonScript()
	: CScript(SCRIPT_TYPE::LUCIDDRAGONSCRIPT)
	, m_Offset(Vector2(-469.f, -130.f))
	, m_Count(6)
	, m_Distance(240.f)
	, m_HitRange(Vector2(1800.f, 421.f))
	, m_HitPos(Vector2(-1145.f, -112.f))
{
	AddScriptParam({SCRIPT_PARAM::VEC2, "Offset", &m_Offset});
	AddScriptParam({SCRIPT_PARAM::INT, "Count", &m_Count});
	AddScriptParam({SCRIPT_PARAM::FLOAT, "Distance", &m_Distance});

	AddScriptParam({SCRIPT_PARAM::VEC2, "Hit Pos", &m_HitPos });
	AddScriptParam({SCRIPT_PARAM::VEC2, "Hit Range", &m_HitRange});
	AddScriptParam({SCRIPT_PARAM::FUNCTION, "Spawn", this, (SCRIPT_DELEGATE)&CLucidDragonScript::Spawn});
	AddScriptParam({SCRIPT_PARAM::FUNCTION, "Attack", this, (SCRIPT_DELEGATE)&CLucidDragonScript::BeginBreath});
}

CLucidDragonScript::CLucidDragonScript(const CLucidDragonScript& _Other)
	: CScript(_Other)
	, m_Offset(_Other.m_Offset)
	, m_Count(_Other.m_Count)
	, m_Distance(_Other.m_Distance)
	, m_HitRange(_Other.m_HitRange)
	, m_HitPos(_Other.m_HitPos)
{
	// 복제된 Param 정보들의 pData가 원본 멤버변수를 가리키기 때문에 초기화 후 재설정
	ClearScriptParam();

	AddScriptParam({ SCRIPT_PARAM::VEC2, "Offset", &m_Offset });
	AddScriptParam({ SCRIPT_PARAM::INT, "Count", &m_Count });
	AddScriptParam({ SCRIPT_PARAM::FLOAT, "Distance", &m_Distance });

	AddScriptParam({ SCRIPT_PARAM::VEC2, "Hit Pos", &m_HitPos });
	AddScriptParam({ SCRIPT_PARAM::VEC2, "Hit Range", &m_HitRange });
	AddScriptParam({ SCRIPT_PARAM::FUNCTION, "Spawn", this, (SCRIPT_DELEGATE)&CLucidDragonScript::Spawn });
	AddScriptParam({ SCRIPT_PARAM::FUNCTION, "Attack", this, (SCRIPT_DELEGATE)&CLucidDragonScript::BeginBreath });
}

CLucidDragonScript::~CLucidDragonScript()
{
}


void CLucidDragonScript::Spawn()
{
	int rand = RandomRange(0, 1);

	Vector3 vSpawnPos = rand > 0? Vector3(-1054.f, 30.f, 0.f) : Vector3(1057.f, 30.f, 0.f);
	Vector3 vSpawnRot = rand > 0? Vector3(0.f, Radian(180.f), 0.f) : Vector3(0.f, 0.f, 0.f);

	GetOwner()->SetActive(true);
	GetOwner()->Transform()->SetRelativePos(vSpawnPos);
	GetOwner()->Transform()->SetRelativeRotation(vSpawnRot);
	GetOwner()->StateMachine()->ChangeState(L"MoveState");
	GetOwner()->StateMachine()->GetCurrentState()->Init();
}

void CLucidDragonScript::Attack()
{
	StateMachine()->ChangeState(L"AttackState");
}

void CLucidDragonScript::BeginBreath()
{
	GetOwner()->Collider2D()->SetActive(true);
	GetOwner()->Collider2D()->SetOffset(m_HitPos);
	GetOwner()->Collider2D()->SetScale(m_HitRange);

	Ptr<CFlipbook> pBreathAnim = FIND_ANIM(L"Flipbook\\LucidDragon_Breath.flip");

	for (int i = 0; i < m_Count; ++i)
	{
		// 추후 삭제
		if (i >= m_vecBreathAnim.size())
		{
			CGameObject* pBreathObj = CreateAnimObject();
			pBreathObj->SetName(L"LucidBreath");
			pBreathObj->SetLayerIdx((int)LAYER_INDEX::MONSTER_OBJECT);
			pBreathObj->Transform()->UseIndependentScale(true);
			pBreathObj->FlipbookPlayer()->AddFlipbook(0, pBreathAnim);
			pBreathObj->SetActive(false);
			m_vecBreathAnim.push_back(pBreathObj);

			GetOwner()->AddChild(pBreathObj);
		}
		// 추후 삭제
		Vector3 vSpawnPos = m_Offset;
		vSpawnPos.x -= m_Distance * i;

		m_vecBreathAnim[i]->SetActive(true);
		m_vecBreathAnim[i]->Transform()->SetRelativePos(vSpawnPos);
		m_vecBreathAnim[i]->FlipbookPlayer()->Play(0, 10.f, true);
	}
}
void CLucidDragonScript::EndBreath()
{
	GetOwner()->Collider2D()->SetActive(false);

	for (int i = 0; i < m_vecBreathAnim.size(); ++i)
	{
		m_vecBreathAnim[i]->SetActive(false);
	}
}

void CLucidDragonScript::Init()
{
	GetOwner()->SetName(L"Lucid Dragon");
	GetOwner()->SetLayerIdx((int)LAYER_INDEX::MONSTER);

	// 컴포넌트 추가
	GetOwner()->AddComponent(new CMeshRender);
	GetOwner()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));

	GetOwner()->AddComponent(new CFlipbookPlayer);
	GetOwner()->FlipbookPlayer()->AddFlipbook(0, FIND_ANIM(L"Flipbook\\LucidDragon_Move.flip"));
	GetOwner()->FlipbookPlayer()->AddFlipbook(1, FIND_ANIM(L"Flipbook\\LucidDragon_Attack.flip"));

	GetOwner()->AddComponent(new CCollider2D);
	GetOwner()->Collider2D()->SetIndependentScale(true);

	GetOwner()->AddComponent(new CStateMachine);
	GetOwner()->StateMachine()->AddState(L"MoveState", new CLucidDragonState_Move);
	GetOwner()->StateMachine()->AddState(L"AttackState", new CLucidDragonState_Attack);
}

void CLucidDragonScript::Begin()
{
	GetOwner()->StateMachine()->ChangeState(L"MoveState");

	Ptr<CFlipbook> pBreathAnim = FIND_ANIM(L"Flipbook\\LucidDragon_Breath.flip");

	for (int i = 0; i < m_Count; ++i)
	{
		CGameObject* pBreathObj = CreateAnimObject();
		pBreathObj->SetName(L"LucidBreath");
		pBreathObj->Transform()->UseIndependentScale(true);
		pBreathObj->FlipbookPlayer()->AddFlipbook(0, pBreathAnim);
		pBreathObj->SetActive(false);
		m_vecBreathAnim.push_back(pBreathObj);


		GetOwner()->AddChild(pBreathObj);
	}
}

void CLucidDragonScript::Tick()
{
}

void CLucidDragonScript::SaveComponent(FILE* _File)
{
	fwrite(&m_Offset, sizeof(m_Offset), 1, _File);
	fwrite(&m_Count, sizeof(m_Offset), 1, _File);
	fwrite(&m_Distance, sizeof(m_Offset), 1, _File);
	fwrite(&m_HitRange, sizeof(m_HitRange), 1, _File);
	fwrite(&m_HitPos, sizeof(m_HitPos), 1, _File);
}

void CLucidDragonScript::LoadComponent(FILE* _FILE)
{
	fread(&m_Offset, sizeof(m_Offset), 1, _FILE);
	fread(&m_Count, sizeof(m_Offset), 1, _FILE);
	fread(&m_Distance, sizeof(m_Offset), 1, _FILE);
	fread(&m_HitRange, sizeof(m_HitRange), 1, _FILE);
	fread(&m_HitPos, sizeof(m_HitPos), 1, _FILE);
}

void CLucidDragonScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetName() == L"Player")
	{
		CPlayerScript* pPlayer = _OtherObject->GetScript<CPlayerScript>();
		if (pPlayer)
			pPlayer->DamageByPercent(1.f);
	}
}


/////////////////////////
// Dragon State : Move //
/////////////////////////

CLucidDragonState_Move::CLucidDragonState_Move()
	: CState(SCRIPT_TYPE::LUCIDDRAGONSTATE_MOVE)
	, m_IsDowned(false)
{
}

CLucidDragonState_Move::~CLucidDragonState_Move()
{
}

void CLucidDragonState_Move::Init()
{
	m_IsDowned = false;
}

void CLucidDragonState_Move::Enter()
{
	GetStateMachine()->FlipbookPlayer()->PlayForDuration(0, 1.8f, true);
}

void CLucidDragonState_Move::Tick()
{
	float ratio = GetStateMachine()->GetStateElapsedTime() / 3.f;

	// 드래곤 이동
	Vector3 vPos = GetStateMachine()->Transform()->GetWorldPos();

	if (m_IsDowned)
		vPos.y = lerp(30.f, 688.f, ratio);
	else
		vPos.y = lerp(688.f, 30.f, ratio);

	GetStateMachine()->Transform()->SetWorldPos(vPos);

	if (ratio > 1.f)
	{
		if (m_IsDowned)
			GetStateMachine()->GetOwner()->SetActive(false);
		else
			GetStateMachine()->ChangeState(L"AttackState");
	}
}

void CLucidDragonState_Move::Exit()
{
	m_IsDowned = !m_IsDowned;
}

///////////////////////////
// Dragon State : Attack //
///////////////////////////
CLucidDragonState_Attack::CLucidDragonState_Attack()
	: CState(SCRIPT_TYPE::LUCIDDRAGONSTATE_ATTACK)
{
}

CLucidDragonState_Attack::~CLucidDragonState_Attack()
{
}


void CLucidDragonState_Attack::Init()
{
}

void CLucidDragonState_Attack::Enter()
{
	m_DragonScript = GetStateMachine()->GetOwner()->GetScript<CLucidDragonScript>();
	GetStateMachine()->FlipbookPlayer()->PlayForDuration(1, 5.4f, false);
}

void CLucidDragonState_Attack::Tick()
{
	if (GetStateMachine()->FlipbookPlayer()->IsFinishedOnce())
	{
		GetStateMachine()->ChangeState(L"MoveState");
	}
	else if (GetStateMachine()->FlipbookPlayer()->GetCurSpriteIdx() == 10)
	{
		m_DragonScript->BeginBreath();
	}
	else if (GetStateMachine()->FlipbookPlayer()->GetCurSpriteIdx() == 34)
	{
		m_DragonScript->EndBreath();
	}
}

void CLucidDragonState_Attack::Exit()
{
	m_DragonScript->EndBreath();
}