#include "pch.h"
#include "CScriptMgr.h"

#include "CArrowSkill.h"
#include "CAttackState.h"
#include "CBackScript.h"
#include "CBellumAttackState.h"
#include "CBellumIdleState.h"
#include "CBellumScript.h"
#include "CBowSkill.h"
#include "CCameraScript.h"
#include "CDamageSkinScript.h"
#include "CDieState.h"
#include "CDoubleJumpSkill.h"
#include "CHeroAuraBladeSkill.h"
#include "CHeroComboAttackSkill.h"
#include "CHeroFlashSlashSkill.h"
#include "CHeroRagingBlowSkill.h"
#include "CHeroSlashBlastSkill.h"
#include "CHeroSOBSState_Attack.h"
#include "CHeroSOBSState_Die.h"
#include "CHeroSOBSState_Idle.h"
#include "CHeroSOBSState_Regen.h"
#include "CHeroSwordOfBurningSoulScript.h"
#include "CHeroSwordOfBurningSoulSkill.h"
#include "CHeroWarLeapSkill.h"
#include "CHitState.h"
#include "CIdleState.h"
#include "CLevelScript.h"
#include "CLifeScript.h"
#include "CLinePlatform.h"
#include "CLineWall.h"
#include "CLucidDragonScript.h"
#include "CLucidDragonSkill.h"
#include "CLucidDragonState_Attack.h"
#include "CLucidDragonState_Move.h"
#include "CLucidExplosionPrisonSkill.h"
#include "CLucidFairyDustSkill.h"
#include "CLucidFlowerExplosionScript.h"
#include "CLucidGolemScript.h"
#include "CLucidGolemState_Drop.h"
#include "CLucidGolemState_Spawn.h"
#include "CLucidHornScript.h"
#include "CLucidLittleButterflyBombScript.h"
#include "CLucidLittleButterflyScript.h"
#include "CLucidLittleButterflySpawnScript.h"
#include "CLucidNightmareButterflyScript.h"
#include "CLucidNightmareButterflyState_Attack.h"
#include "CLucidNightmareButterflyState_Die.h"
#include "CLucidNightmareButterflyState_Fly.h"
#include "CLucidNightmareButterflyState_Hit.h"
#include "CLucidNightmareButterflyState_Regen.h"
#include "CLucidPhase1Script.h"
#include "CLucidPhase2Script.h"
#include "CLucidPrisonState_Die.h"
#include "CLucidPrisonState_Regen.h"
#include "CLucidPrisonState_Stay.h"
#include "CLucidScript.h"
#include "CLucidToadstoolScript.h"
#include "CMissileScript.h"
#include "CMonsterScript.h"
#include "CMouseScript.h"
#include "CMultiPlayerScript.h"
#include "CParticleSystemScript.h"
#include "CPatrolState.h"
#include "CPlatformerScript.h"
#include "CPlatformScript.h"
#include "CPlayerAttackState.h"
#include "CPlayerIdleState.h"
#include "CPlayerScript.h"
#include "CPortalScript.h"
#include "CProjectileScript.h"
#include "CRope.h"
#include "CSkillManager.h"
#include "CSkillScript.h"
#include "CSkillState.h"
#include "CSpawnerScript.h"
#include "CSpawnState.h"
#include "CStrikeSkill.h"
#include "CTraceState.h"
#include "CWallScript.h"
#include "CWindWalkSkill.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CArrowSkill");
	_vec.push_back(L"CAttackState");
	_vec.push_back(L"CBackScript");
	_vec.push_back(L"CBellumAttackState");
	_vec.push_back(L"CBellumIdleState");
	_vec.push_back(L"CBellumScript");
	_vec.push_back(L"CBowSkill");
	_vec.push_back(L"CCameraScript");
	_vec.push_back(L"CDamageSkinScript");
	_vec.push_back(L"CDieState");
	_vec.push_back(L"CDoubleJumpSkill");
	_vec.push_back(L"CHeroAuraBladeSkill");
	_vec.push_back(L"CHeroComboAttackSkill");
	_vec.push_back(L"CHeroFlashSlashSkill");
	_vec.push_back(L"CHeroRagingBlowSkill");
	_vec.push_back(L"CHeroSlashBlastSkill");
	_vec.push_back(L"CHeroSOBSState_Attack");
	_vec.push_back(L"CHeroSOBSState_Die");
	_vec.push_back(L"CHeroSOBSState_Idle");
	_vec.push_back(L"CHeroSOBSState_Regen");
	_vec.push_back(L"CHeroSwordOfBurningSoulScript");
	_vec.push_back(L"CHeroSwordOfBurningSoulSkill");
	_vec.push_back(L"CHeroWarLeapSkill");
	_vec.push_back(L"CHitState");
	_vec.push_back(L"CIdleState");
	_vec.push_back(L"CLevelScript");
	_vec.push_back(L"CLifeScript");
	_vec.push_back(L"CLinePlatform");
	_vec.push_back(L"CLineWall");
	_vec.push_back(L"CLucidDragonScript");
	_vec.push_back(L"CLucidDragonSkill");
	_vec.push_back(L"CLucidDragonState_Attack");
	_vec.push_back(L"CLucidDragonState_Move");
	_vec.push_back(L"CLucidExplosionPrisonSkill");
	_vec.push_back(L"CLucidFairyDustSkill");
	_vec.push_back(L"CLucidFlowerExplosionScript");
	_vec.push_back(L"CLucidGolemScript");
	_vec.push_back(L"CLucidGolemState_Drop");
	_vec.push_back(L"CLucidGolemState_Spawn");
	_vec.push_back(L"CLucidHornScript");
	_vec.push_back(L"CLucidLittleButterflyBombScript");
	_vec.push_back(L"CLucidLittleButterflyScript");
	_vec.push_back(L"CLucidLittleButterflySpawnScript");
	_vec.push_back(L"CLucidNightmareButterflyScript");
	_vec.push_back(L"CLucidNightmareButterflyState_Attack");
	_vec.push_back(L"CLucidNightmareButterflyState_Die");
	_vec.push_back(L"CLucidNightmareButterflyState_Fly");
	_vec.push_back(L"CLucidNightmareButterflyState_Hit");
	_vec.push_back(L"CLucidNightmareButterflyState_Regen");
	_vec.push_back(L"CLucidPhase1Script");
	_vec.push_back(L"CLucidPhase2Script");
	_vec.push_back(L"CLucidPrisonState_Die");
	_vec.push_back(L"CLucidPrisonState_Regen");
	_vec.push_back(L"CLucidPrisonState_Stay");
	_vec.push_back(L"CLucidScript");
	_vec.push_back(L"CLucidToadstoolScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CMouseScript");
	_vec.push_back(L"CMultiPlayerScript");
	_vec.push_back(L"CParticleSystemScript");
	_vec.push_back(L"CPatrolState");
	_vec.push_back(L"CPlatformerScript");
	_vec.push_back(L"CPlatformScript");
	_vec.push_back(L"CPlayerAttackState");
	_vec.push_back(L"CPlayerIdleState");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CPortalScript");
	_vec.push_back(L"CProjectileScript");
	_vec.push_back(L"CRope");
	_vec.push_back(L"CSkillManager");
	_vec.push_back(L"CSkillScript");
	_vec.push_back(L"CSkillState");
	_vec.push_back(L"CSpawnerScript");
	_vec.push_back(L"CSpawnState");
	_vec.push_back(L"CStrikeSkill");
	_vec.push_back(L"CTraceState");
	_vec.push_back(L"CWallScript");
	_vec.push_back(L"CWindWalkSkill");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CArrowSkill" == _strScriptName)
		return new CArrowSkill;
	if (L"CAttackState" == _strScriptName)
		return new CAttackState;
	if (L"CBackScript" == _strScriptName)
		return new CBackScript;
	if (L"CBellumAttackState" == _strScriptName)
		return new CBellumAttackState;
	if (L"CBellumIdleState" == _strScriptName)
		return new CBellumIdleState;
	if (L"CBellumScript" == _strScriptName)
		return new CBellumScript;
	if (L"CBowSkill" == _strScriptName)
		return new CBowSkill;
	if (L"CCameraScript" == _strScriptName)
		return new CCameraScript;
	if (L"CDamageSkinScript" == _strScriptName)
		return new CDamageSkinScript;
	if (L"CDieState" == _strScriptName)
		return new CDieState;
	if (L"CDoubleJumpSkill" == _strScriptName)
		return new CDoubleJumpSkill;
	if (L"CHeroAuraBladeSkill" == _strScriptName)
		return new CHeroAuraBladeSkill;
	if (L"CHeroComboAttackSkill" == _strScriptName)
		return new CHeroComboAttackSkill;
	if (L"CHeroFlashSlashSkill" == _strScriptName)
		return new CHeroFlashSlashSkill;
	if (L"CHeroRagingBlowSkill" == _strScriptName)
		return new CHeroRagingBlowSkill;
	if (L"CHeroSlashBlastSkill" == _strScriptName)
		return new CHeroSlashBlastSkill;
	if (L"CHeroSOBSState_Attack" == _strScriptName)
		return new CHeroSOBSState_Attack;
	if (L"CHeroSOBSState_Die" == _strScriptName)
		return new CHeroSOBSState_Die;
	if (L"CHeroSOBSState_Idle" == _strScriptName)
		return new CHeroSOBSState_Idle;
	if (L"CHeroSOBSState_Regen" == _strScriptName)
		return new CHeroSOBSState_Regen;
	if (L"CHeroSwordOfBurningSoulScript" == _strScriptName)
		return new CHeroSwordOfBurningSoulScript;
	if (L"CHeroSwordOfBurningSoulSkill" == _strScriptName)
		return new CHeroSwordOfBurningSoulSkill;
	if (L"CHeroWarLeapSkill" == _strScriptName)
		return new CHeroWarLeapSkill;
	if (L"CHitState" == _strScriptName)
		return new CHitState;
	if (L"CIdleState" == _strScriptName)
		return new CIdleState;
	if (L"CLevelScript" == _strScriptName)
		return new CLevelScript;
	if (L"CLifeScript" == _strScriptName)
		return new CLifeScript;
	if (L"CLinePlatform" == _strScriptName)
		return new CLinePlatform;
	if (L"CLineWall" == _strScriptName)
		return new CLineWall;
	if (L"CLucidDragonScript" == _strScriptName)
		return new CLucidDragonScript;
	if (L"CLucidDragonSkill" == _strScriptName)
		return new CLucidDragonSkill;
	if (L"CLucidDragonState_Attack" == _strScriptName)
		return new CLucidDragonState_Attack;
	if (L"CLucidDragonState_Move" == _strScriptName)
		return new CLucidDragonState_Move;
	if (L"CLucidExplosionPrisonSkill" == _strScriptName)
		return new CLucidExplosionPrisonSkill;
	if (L"CLucidFairyDustSkill" == _strScriptName)
		return new CLucidFairyDustSkill;
	if (L"CLucidFlowerExplosionScript" == _strScriptName)
		return new CLucidFlowerExplosionScript;
	if (L"CLucidGolemScript" == _strScriptName)
		return new CLucidGolemScript;
	if (L"CLucidGolemState_Drop" == _strScriptName)
		return new CLucidGolemState_Drop;
	if (L"CLucidGolemState_Spawn" == _strScriptName)
		return new CLucidGolemState_Spawn;
	if (L"CLucidHornScript" == _strScriptName)
		return new CLucidHornScript;
	if (L"CLucidLittleButterflyBombScript" == _strScriptName)
		return new CLucidLittleButterflyBombScript;
	if (L"CLucidLittleButterflyScript" == _strScriptName)
		return new CLucidLittleButterflyScript;
	if (L"CLucidLittleButterflySpawnScript" == _strScriptName)
		return new CLucidLittleButterflySpawnScript;
	if (L"CLucidNightmareButterflyScript" == _strScriptName)
		return new CLucidNightmareButterflyScript;
	if (L"CLucidNightmareButterflyState_Attack" == _strScriptName)
		return new CLucidNightmareButterflyState_Attack;
	if (L"CLucidNightmareButterflyState_Die" == _strScriptName)
		return new CLucidNightmareButterflyState_Die;
	if (L"CLucidNightmareButterflyState_Fly" == _strScriptName)
		return new CLucidNightmareButterflyState_Fly;
	if (L"CLucidNightmareButterflyState_Hit" == _strScriptName)
		return new CLucidNightmareButterflyState_Hit;
	if (L"CLucidNightmareButterflyState_Regen" == _strScriptName)
		return new CLucidNightmareButterflyState_Regen;
	if (L"CLucidPhase1Script" == _strScriptName)
		return new CLucidPhase1Script;
	if (L"CLucidPhase2Script" == _strScriptName)
		return new CLucidPhase2Script;
	if (L"CLucidPrisonState_Die" == _strScriptName)
		return new CLucidPrisonState_Die;
	if (L"CLucidPrisonState_Regen" == _strScriptName)
		return new CLucidPrisonState_Regen;
	if (L"CLucidPrisonState_Stay" == _strScriptName)
		return new CLucidPrisonState_Stay;
	if (L"CLucidScript" == _strScriptName)
		return new CLucidScript;
	if (L"CLucidToadstoolScript" == _strScriptName)
		return new CLucidToadstoolScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"CMouseScript" == _strScriptName)
		return new CMouseScript;
	if (L"CMultiPlayerScript" == _strScriptName)
		return new CMultiPlayerScript;
	if (L"CParticleSystemScript" == _strScriptName)
		return new CParticleSystemScript;
	if (L"CPatrolState" == _strScriptName)
		return new CPatrolState;
	if (L"CPlatformerScript" == _strScriptName)
		return new CPlatformerScript;
	if (L"CPlatformScript" == _strScriptName)
		return new CPlatformScript;
	if (L"CPlayerAttackState" == _strScriptName)
		return new CPlayerAttackState;
	if (L"CPlayerIdleState" == _strScriptName)
		return new CPlayerIdleState;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CPortalScript" == _strScriptName)
		return new CPortalScript;
	if (L"CProjectileScript" == _strScriptName)
		return new CProjectileScript;
	if (L"CRope" == _strScriptName)
		return new CRope;
	if (L"CSkillManager" == _strScriptName)
		return new CSkillManager;
	if (L"CSkillScript" == _strScriptName)
		return new CSkillScript;
	if (L"CSkillState" == _strScriptName)
		return new CSkillState;
	if (L"CSpawnerScript" == _strScriptName)
		return new CSpawnerScript;
	if (L"CSpawnState" == _strScriptName)
		return new CSpawnState;
	if (L"CStrikeSkill" == _strScriptName)
		return new CStrikeSkill;
	if (L"CTraceState" == _strScriptName)
		return new CTraceState;
	if (L"CWallScript" == _strScriptName)
		return new CWallScript;
	if (L"CWindWalkSkill" == _strScriptName)
		return new CWindWalkSkill;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::ARROWSKILL:
		return new CArrowSkill;
		break;
	case (UINT)SCRIPT_TYPE::ATTACKSTATE:
		return new CAttackState;
		break;
	case (UINT)SCRIPT_TYPE::BACKSCRIPT:
		return new CBackScript;
		break;
	case (UINT)SCRIPT_TYPE::BELLUMATTACKSTATE:
		return new CBellumAttackState;
		break;
	case (UINT)SCRIPT_TYPE::BELLUMIDLESTATE:
		return new CBellumIdleState;
		break;
	case (UINT)SCRIPT_TYPE::BELLUMSCRIPT:
		return new CBellumScript;
		break;
	case (UINT)SCRIPT_TYPE::BOWSKILL:
		return new CBowSkill;
		break;
	case (UINT)SCRIPT_TYPE::CAMERASCRIPT:
		return new CCameraScript;
		break;
	case (UINT)SCRIPT_TYPE::DAMAGESKINSCRIPT:
		return new CDamageSkinScript;
		break;
	case (UINT)SCRIPT_TYPE::DIESTATE:
		return new CDieState;
		break;
	case (UINT)SCRIPT_TYPE::DOUBLEJUMPSKILL:
		return new CDoubleJumpSkill;
		break;
	case (UINT)SCRIPT_TYPE::HEROAURABLADESKILL:
		return new CHeroAuraBladeSkill;
		break;
	case (UINT)SCRIPT_TYPE::HEROCOMBOATTACKSKILL:
		return new CHeroComboAttackSkill;
		break;
	case (UINT)SCRIPT_TYPE::HEROFLASHSLASHSKILL:
		return new CHeroFlashSlashSkill;
		break;
	case (UINT)SCRIPT_TYPE::HERORAGINGBLOWSKILL:
		return new CHeroRagingBlowSkill;
		break;
	case (UINT)SCRIPT_TYPE::HEROSLASHBLASTSKILL:
		return new CHeroSlashBlastSkill;
		break;
	case (UINT)SCRIPT_TYPE::HEROSOBSSTATE_ATTACK:
		return new CHeroSOBSState_Attack;
		break;
	case (UINT)SCRIPT_TYPE::HEROSOBSSTATE_DIE:
		return new CHeroSOBSState_Die;
		break;
	case (UINT)SCRIPT_TYPE::HEROSOBSSTATE_IDLE:
		return new CHeroSOBSState_Idle;
		break;
	case (UINT)SCRIPT_TYPE::HEROSOBSSTATE_REGEN:
		return new CHeroSOBSState_Regen;
		break;
	case (UINT)SCRIPT_TYPE::HEROSWORDOFBURNINGSOULSCRIPT:
		return new CHeroSwordOfBurningSoulScript;
		break;
	case (UINT)SCRIPT_TYPE::HEROSWORDOFBURNINGSOULSKILL:
		return new CHeroSwordOfBurningSoulSkill;
		break;
	case (UINT)SCRIPT_TYPE::HEROWARLEAPSKILL:
		return new CHeroWarLeapSkill;
		break;
	case (UINT)SCRIPT_TYPE::HITSTATE:
		return new CHitState;
		break;
	case (UINT)SCRIPT_TYPE::IDLESTATE:
		return new CIdleState;
		break;
	case (UINT)SCRIPT_TYPE::LEVELSCRIPT:
		return new CLevelScript;
		break;
	case (UINT)SCRIPT_TYPE::LIFESCRIPT:
		return new CLifeScript;
		break;
	case (UINT)SCRIPT_TYPE::LINEPLATFORM:
		return new CLinePlatform;
		break;
	case (UINT)SCRIPT_TYPE::LINEWALL:
		return new CLineWall;
		break;
	case (UINT)SCRIPT_TYPE::LUCIDDRAGONSCRIPT:
		return new CLucidDragonScript;
		break;
	case (UINT)SCRIPT_TYPE::LUCIDDRAGONSKILL:
		return new CLucidDragonSkill;
		break;
	case (UINT)SCRIPT_TYPE::LUCIDDRAGONSTATE_ATTACK:
		return new CLucidDragonState_Attack;
		break;
	case (UINT)SCRIPT_TYPE::LUCIDDRAGONSTATE_MOVE:
		return new CLucidDragonState_Move;
		break;
	case (UINT)SCRIPT_TYPE::LUCIDEXPLOSIONPRISONSKILL:
		return new CLucidExplosionPrisonSkill;
		break;
	case (UINT)SCRIPT_TYPE::LUCIDFAIRYDUSTSKILL:
		return new CLucidFairyDustSkill;
		break;
	case (UINT)SCRIPT_TYPE::LUCIDFLOWEREXPLOSIONSCRIPT:
		return new CLucidFlowerExplosionScript;
		break;
	case (UINT)SCRIPT_TYPE::LUCIDGOLEMSCRIPT:
		return new CLucidGolemScript;
		break;
	case (UINT)SCRIPT_TYPE::LUCIDGOLEMSTATE_DROP:
		return new CLucidGolemState_Drop;
		break;
	case (UINT)SCRIPT_TYPE::LUCIDGOLEMSTATE_SPAWN:
		return new CLucidGolemState_Spawn;
		break;
	case (UINT)SCRIPT_TYPE::LUCIDHORNSCRIPT:
		return new CLucidHornScript;
		break;
	case (UINT)SCRIPT_TYPE::LUCIDLITTLEBUTTERFLYBOMBSCRIPT:
		return new CLucidLittleButterflyBombScript;
		break;
	case (UINT)SCRIPT_TYPE::LUCIDLITTLEBUTTERFLYSCRIPT:
		return new CLucidLittleButterflyScript;
		break;
	case (UINT)SCRIPT_TYPE::LUCIDLITTLEBUTTERFLYSPAWNSCRIPT:
		return new CLucidLittleButterflySpawnScript;
		break;
	case (UINT)SCRIPT_TYPE::LUCIDNIGHTMAREBUTTERFLYSCRIPT:
		return new CLucidNightmareButterflyScript;
		break;
	case (UINT)SCRIPT_TYPE::LUCIDNIGHTMAREBUTTERFLYSTATE_ATTACK:
		return new CLucidNightmareButterflyState_Attack;
		break;
	case (UINT)SCRIPT_TYPE::LUCIDNIGHTMAREBUTTERFLYSTATE_DIE:
		return new CLucidNightmareButterflyState_Die;
		break;
	case (UINT)SCRIPT_TYPE::LUCIDNIGHTMAREBUTTERFLYSTATE_FLY:
		return new CLucidNightmareButterflyState_Fly;
		break;
	case (UINT)SCRIPT_TYPE::LUCIDNIGHTMAREBUTTERFLYSTATE_HIT:
		return new CLucidNightmareButterflyState_Hit;
		break;
	case (UINT)SCRIPT_TYPE::LUCIDNIGHTMAREBUTTERFLYSTATE_REGEN:
		return new CLucidNightmareButterflyState_Regen;
		break;
	case (UINT)SCRIPT_TYPE::LUCIDPHASE1SCRIPT:
		return new CLucidPhase1Script;
		break;
	case (UINT)SCRIPT_TYPE::LUCIDPHASE2SCRIPT:
		return new CLucidPhase2Script;
		break;
	case (UINT)SCRIPT_TYPE::LUCIDPRISONSTATE_DIE:
		return new CLucidPrisonState_Die;
		break;
	case (UINT)SCRIPT_TYPE::LUCIDPRISONSTATE_REGEN:
		return new CLucidPrisonState_Regen;
		break;
	case (UINT)SCRIPT_TYPE::LUCIDPRISONSTATE_STAY:
		return new CLucidPrisonState_Stay;
		break;
	case (UINT)SCRIPT_TYPE::LUCIDSCRIPT:
		return new CLucidScript;
		break;
	case (UINT)SCRIPT_TYPE::LUCIDTOADSTOOLSCRIPT:
		return new CLucidToadstoolScript;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT:
		return new CMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::MOUSESCRIPT:
		return new CMouseScript;
		break;
	case (UINT)SCRIPT_TYPE::MULTIPLAYERSCRIPT:
		return new CMultiPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::PARTICLESYSTEMSCRIPT:
		return new CParticleSystemScript;
		break;
	case (UINT)SCRIPT_TYPE::PATROLSTATE:
		return new CPatrolState;
		break;
	case (UINT)SCRIPT_TYPE::PLATFORMERSCRIPT:
		return new CPlatformerScript;
		break;
	case (UINT)SCRIPT_TYPE::PLATFORMSCRIPT:
		return new CPlatformScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERATTACKSTATE:
		return new CPlayerAttackState;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERIDLESTATE:
		return new CPlayerIdleState;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::PORTALSCRIPT:
		return new CPortalScript;
		break;
	case (UINT)SCRIPT_TYPE::PROJECTILESCRIPT:
		return new CProjectileScript;
		break;
	case (UINT)SCRIPT_TYPE::ROPE:
		return new CRope;
		break;
	case (UINT)SCRIPT_TYPE::SKILLMANAGER:
		return new CSkillManager;
		break;
	case (UINT)SCRIPT_TYPE::SKILLSCRIPT:
		return new CSkillScript;
		break;
	case (UINT)SCRIPT_TYPE::SKILLSTATE:
		return new CSkillState;
		break;
	case (UINT)SCRIPT_TYPE::SPAWNERSCRIPT:
		return new CSpawnerScript;
		break;
	case (UINT)SCRIPT_TYPE::SPAWNSTATE:
		return new CSpawnState;
		break;
	case (UINT)SCRIPT_TYPE::STRIKESKILL:
		return new CStrikeSkill;
		break;
	case (UINT)SCRIPT_TYPE::TRACESTATE:
		return new CTraceState;
		break;
	case (UINT)SCRIPT_TYPE::WALLSCRIPT:
		return new CWallScript;
		break;
	case (UINT)SCRIPT_TYPE::WINDWALKSKILL:
		return new CWindWalkSkill;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::ARROWSKILL:
		return L"CArrowSkill";
		break;

	case SCRIPT_TYPE::ATTACKSTATE:
		return L"CAttackState";
		break;

	case SCRIPT_TYPE::BACKSCRIPT:
		return L"CBackScript";
		break;

	case SCRIPT_TYPE::BELLUMATTACKSTATE:
		return L"CBellumAttackState";
		break;

	case SCRIPT_TYPE::BELLUMIDLESTATE:
		return L"CBellumIdleState";
		break;

	case SCRIPT_TYPE::BELLUMSCRIPT:
		return L"CBellumScript";
		break;

	case SCRIPT_TYPE::BOWSKILL:
		return L"CBowSkill";
		break;

	case SCRIPT_TYPE::CAMERASCRIPT:
		return L"CCameraScript";
		break;

	case SCRIPT_TYPE::DAMAGESKINSCRIPT:
		return L"CDamageSkinScript";
		break;

	case SCRIPT_TYPE::DIESTATE:
		return L"CDieState";
		break;

	case SCRIPT_TYPE::DOUBLEJUMPSKILL:
		return L"CDoubleJumpSkill";
		break;

	case SCRIPT_TYPE::HEROAURABLADESKILL:
		return L"CHeroAuraBladeSkill";
		break;

	case SCRIPT_TYPE::HEROCOMBOATTACKSKILL:
		return L"CHeroComboAttackSkill";
		break;

	case SCRIPT_TYPE::HEROFLASHSLASHSKILL:
		return L"CHeroFlashSlashSkill";
		break;

	case SCRIPT_TYPE::HERORAGINGBLOWSKILL:
		return L"CHeroRagingBlowSkill";
		break;

	case SCRIPT_TYPE::HEROSLASHBLASTSKILL:
		return L"CHeroSlashBlastSkill";
		break;

	case SCRIPT_TYPE::HEROSOBSSTATE_ATTACK:
		return L"CHeroSOBSState_Attack";
		break;

	case SCRIPT_TYPE::HEROSOBSSTATE_DIE:
		return L"CHeroSOBSState_Die";
		break;

	case SCRIPT_TYPE::HEROSOBSSTATE_IDLE:
		return L"CHeroSOBSState_Idle";
		break;

	case SCRIPT_TYPE::HEROSOBSSTATE_REGEN:
		return L"CHeroSOBSState_Regen";
		break;

	case SCRIPT_TYPE::HEROSWORDOFBURNINGSOULSCRIPT:
		return L"CHeroSwordOfBurningSoulScript";
		break;

	case SCRIPT_TYPE::HEROSWORDOFBURNINGSOULSKILL:
		return L"CHeroSwordOfBurningSoulSkill";
		break;

	case SCRIPT_TYPE::HEROWARLEAPSKILL:
		return L"CHeroWarLeapSkill";
		break;

	case SCRIPT_TYPE::HITSTATE:
		return L"CHitState";
		break;

	case SCRIPT_TYPE::IDLESTATE:
		return L"CIdleState";
		break;

	case SCRIPT_TYPE::LEVELSCRIPT:
		return L"CLevelScript";
		break;

	case SCRIPT_TYPE::LIFESCRIPT:
		return L"CLifeScript";
		break;

	case SCRIPT_TYPE::LINEPLATFORM:
		return L"CLinePlatform";
		break;

	case SCRIPT_TYPE::LINEWALL:
		return L"CLineWall";
		break;

	case SCRIPT_TYPE::LUCIDDRAGONSCRIPT:
		return L"CLucidDragonScript";
		break;

	case SCRIPT_TYPE::LUCIDDRAGONSKILL:
		return L"CLucidDragonSkill";
		break;

	case SCRIPT_TYPE::LUCIDDRAGONSTATE_ATTACK:
		return L"CLucidDragonState_Attack";
		break;

	case SCRIPT_TYPE::LUCIDDRAGONSTATE_MOVE:
		return L"CLucidDragonState_Move";
		break;

	case SCRIPT_TYPE::LUCIDEXPLOSIONPRISONSKILL:
		return L"CLucidExplosionPrisonSkill";
		break;

	case SCRIPT_TYPE::LUCIDFAIRYDUSTSKILL:
		return L"CLucidFairyDustSkill";
		break;

	case SCRIPT_TYPE::LUCIDFLOWEREXPLOSIONSCRIPT:
		return L"CLucidFlowerExplosionScript";
		break;

	case SCRIPT_TYPE::LUCIDGOLEMSCRIPT:
		return L"CLucidGolemScript";
		break;

	case SCRIPT_TYPE::LUCIDGOLEMSTATE_DROP:
		return L"CLucidGolemState_Drop";
		break;

	case SCRIPT_TYPE::LUCIDGOLEMSTATE_SPAWN:
		return L"CLucidGolemState_Spawn";
		break;

	case SCRIPT_TYPE::LUCIDHORNSCRIPT:
		return L"CLucidHornScript";
		break;

	case SCRIPT_TYPE::LUCIDLITTLEBUTTERFLYBOMBSCRIPT:
		return L"CLucidLittleButterflyBombScript";
		break;

	case SCRIPT_TYPE::LUCIDLITTLEBUTTERFLYSCRIPT:
		return L"CLucidLittleButterflyScript";
		break;

	case SCRIPT_TYPE::LUCIDLITTLEBUTTERFLYSPAWNSCRIPT:
		return L"CLucidLittleButterflySpawnScript";
		break;

	case SCRIPT_TYPE::LUCIDNIGHTMAREBUTTERFLYSCRIPT:
		return L"CLucidNightmareButterflyScript";
		break;

	case SCRIPT_TYPE::LUCIDNIGHTMAREBUTTERFLYSTATE_ATTACK:
		return L"CLucidNightmareButterflyState_Attack";
		break;

	case SCRIPT_TYPE::LUCIDNIGHTMAREBUTTERFLYSTATE_DIE:
		return L"CLucidNightmareButterflyState_Die";
		break;

	case SCRIPT_TYPE::LUCIDNIGHTMAREBUTTERFLYSTATE_FLY:
		return L"CLucidNightmareButterflyState_Fly";
		break;

	case SCRIPT_TYPE::LUCIDNIGHTMAREBUTTERFLYSTATE_HIT:
		return L"CLucidNightmareButterflyState_Hit";
		break;

	case SCRIPT_TYPE::LUCIDNIGHTMAREBUTTERFLYSTATE_REGEN:
		return L"CLucidNightmareButterflyState_Regen";
		break;

	case SCRIPT_TYPE::LUCIDPHASE1SCRIPT:
		return L"CLucidPhase1Script";
		break;

	case SCRIPT_TYPE::LUCIDPHASE2SCRIPT:
		return L"CLucidPhase2Script";
		break;

	case SCRIPT_TYPE::LUCIDPRISONSTATE_DIE:
		return L"CLucidPrisonState_Die";
		break;

	case SCRIPT_TYPE::LUCIDPRISONSTATE_REGEN:
		return L"CLucidPrisonState_Regen";
		break;

	case SCRIPT_TYPE::LUCIDPRISONSTATE_STAY:
		return L"CLucidPrisonState_Stay";
		break;

	case SCRIPT_TYPE::LUCIDSCRIPT:
		return L"CLucidScript";
		break;

	case SCRIPT_TYPE::LUCIDTOADSTOOLSCRIPT:
		return L"CLucidToadstoolScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"CMonsterScript";
		break;

	case SCRIPT_TYPE::MOUSESCRIPT:
		return L"CMouseScript";
		break;

	case SCRIPT_TYPE::MULTIPLAYERSCRIPT:
		return L"CMultiPlayerScript";
		break;

	case SCRIPT_TYPE::PARTICLESYSTEMSCRIPT:
		return L"CParticleSystemScript";
		break;

	case SCRIPT_TYPE::PATROLSTATE:
		return L"CPatrolState";
		break;

	case SCRIPT_TYPE::PLATFORMERSCRIPT:
		return L"CPlatformerScript";
		break;

	case SCRIPT_TYPE::PLATFORMSCRIPT:
		return L"CPlatformScript";
		break;

	case SCRIPT_TYPE::PLAYERATTACKSTATE:
		return L"CPlayerAttackState";
		break;

	case SCRIPT_TYPE::PLAYERIDLESTATE:
		return L"CPlayerIdleState";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::PORTALSCRIPT:
		return L"CPortalScript";
		break;

	case SCRIPT_TYPE::PROJECTILESCRIPT:
		return L"CProjectileScript";
		break;

	case SCRIPT_TYPE::ROPE:
		return L"CRope";
		break;

	case SCRIPT_TYPE::SKILLMANAGER:
		return L"CSkillManager";
		break;

	case SCRIPT_TYPE::SKILLSCRIPT:
		return L"CSkillScript";
		break;

	case SCRIPT_TYPE::SKILLSTATE:
		return L"CSkillState";
		break;

	case SCRIPT_TYPE::SPAWNERSCRIPT:
		return L"CSpawnerScript";
		break;

	case SCRIPT_TYPE::SPAWNSTATE:
		return L"CSpawnState";
		break;

	case SCRIPT_TYPE::STRIKESKILL:
		return L"CStrikeSkill";
		break;

	case SCRIPT_TYPE::TRACESTATE:
		return L"CTraceState";
		break;

	case SCRIPT_TYPE::WALLSCRIPT:
		return L"CWallScript";
		break;

	case SCRIPT_TYPE::WINDWALKSKILL:
		return L"CWindWalkSkill";
		break;

	}
	return nullptr;
}