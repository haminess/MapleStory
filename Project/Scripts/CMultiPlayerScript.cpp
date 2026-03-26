#include "pch.h"
#include "CMultiPlayerScript.h"

CMultiPlayerScript::CMultiPlayerScript()
	: CScript(SCRIPT_TYPE::MULTIPLAYERSCRIPT)
{
}

CMultiPlayerScript::CMultiPlayerScript(const CMultiPlayerScript& _Other)
	: CScript(SCRIPT_TYPE::MULTIPLAYERSCRIPT)
{
}

CMultiPlayerScript::~CMultiPlayerScript()
{
}


void CMultiPlayerScript::AddFlipbook(int _idx, Ptr<CFlipbook> _Flip)
{
	if (nullptr == _Flip) return;

	// 기존 플레이어에 자리 있으면 넣기
	for (int i = 0; i < m_Players.size(); ++i)
	{
		if (nullptr == m_Players[i]->GetFlipbook(_idx))
		{
			m_Players[i]->AddFlipbook(_idx, _Flip);
			return;
		}
	}

	// 자리 없으면 새로 플레이어 생성
	CGameObject* pChild = new CGameObject;
	pChild->SetName(_Flip->GetName());
	GetOwner()->AddChild(pChild);

	CFlipbookPlayer* pNewPlayer = (CFlipbookPlayer*)pChild->AddComponent(new CFlipbookPlayer);
	pNewPlayer->AddFlipbook(_idx, _Flip);
	m_Players.push_back(pNewPlayer);
}

bool CMultiPlayerScript::IsPlaying()
{
	if (m_Players.size() > 0)
	{
		return m_Players[0]->IsPlaying();
	}

	return false;
}

void CMultiPlayerScript::Tick()
{
}
