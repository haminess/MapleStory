#include "pch.h"
#include "CStateMachine.h"
#include "CState.h"

#include <Scripts/CScriptMgr.h>

CStateMachine::CStateMachine()
	: CComponent(COMPONENT_TYPE::STATEMACHINE)
	, m_CurState(nullptr)
{
}

CStateMachine::CStateMachine(const CStateMachine& _Other)
	: CComponent(_Other)
	, m_CurState(nullptr)
{
	map<wstring, CState*>::const_iterator iter = _Other.m_mapState.begin();
	for (; iter != _Other.m_mapState.end(); ++iter)
	{
		CState* pCloneState = iter->second->Clone();
		AddState(iter->first, pCloneState);
	}

	if (nullptr != _Other.m_CurState)
	{
		ChangeState(_Other.m_CurState->GetStateKey());
	}
}

CStateMachine::~CStateMachine()
{
	DeleteMap(m_mapState);
	//ReleaseMap(m_mapState);
}

void CStateMachine::SaveComponent(FILE* _File)
{
	size_t count = m_mapState.size();
	fwrite(&count, sizeof(size_t), 1, _File);
	map<wstring, CState*>::iterator iter = m_mapState.begin();
	for (; iter != m_mapState.end(); ++iter)
	{
		SaveWString(iter->first, _File);					// Key
		SaveWString(iter->second->GetName(), _File);		// Class Type
	}
}

void CStateMachine::LoadComponent(FILE* _File)
{
	size_t count = 0;
	fread(&count, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < count; ++i)
	{
		wstring StateKey, StateName;
		LoadWString(StateKey, _File);
		LoadWString(StateName, _File);
		AddState(StateKey, dynamic_cast<CState*>(CScriptMgr::GetScript(StateName)));
	}
}




void CStateMachine::Init()
{
	for (auto& state : m_mapState)
	{
		state.second->Init();
	}
}

void CStateMachine::FinalTick()
{
	if (nullptr == m_CurState)
		return;

	m_CurState->Tick();
}

void CStateMachine::AddState(const wstring& _StateKey, CState* _State)
{
	CState* pState = FindState(_StateKey);
	//assert(pState == nullptr);
	if (pState)
	{
		delete _State;
		return;
	}

	_State->SetName(CScriptMgr::GetScriptName(_State)? CScriptMgr::GetScriptName(_State) : _StateKey);
	_State->m_StateKey = _StateKey;

	m_mapState.insert(make_pair(_StateKey, _State));
	_State->m_Owner = this;

	_State->Init();
}

CState* CStateMachine::FindState(const wstring& _StateName)
{
	map<wstring, CState*>::iterator iter = m_mapState.find(_StateName);

	if (iter == m_mapState.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CStateMachine::ChangeState(const wstring& _StateName)
{
	CState* pNextState = FindState(_StateName);

	// 기존 상태를 빠져나오고(Exit)
	if (nullptr != m_CurState)
	{
		m_CurState->Exit();
	}

	// 새로운 상태를 가리키고
	m_CurState = pNextState;
	//assert(m_CurState);

	// 새로운 상태로 진입(Enter) 한다.
	if (m_CurState)
	{
		m_CurState->Enter();
		m_EnterTime = CTimeMgr::GetInst()->GetTime();
	}
}

float CStateMachine::GetStateEnteredTime() 
{ 
	if (m_CurState)
		return m_EnterTime;
	else
		return 0.f;
}

float CStateMachine::GetStateElapsedTime() 
{ 
	if (m_CurState)
		return CTimeMgr::GetInst()->GetTime() - m_EnterTime;
	else
		return 0.f;
}
