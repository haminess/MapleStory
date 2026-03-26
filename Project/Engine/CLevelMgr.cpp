#include "pch.h"
#include "CLevelMgr.h"

#include "CLevel.h"
#include "CLayer.h"
#include "CCollisionMgr.h"
#include "CRenderMgr.h"

#include "CGameObject.h"
#include "components.h"

#include "CAssetMgr.h"
#include "assets.h"
#include "CDevice.h"

#include "CClearColorCS.h"
#include "CStructuredBuffer.h"

#include "CPrefab.h"

CLevelMgr::CLevelMgr()
	: m_CurLevel(nullptr)
{

}

CLevelMgr::~CLevelMgr()
{
	DELETE(m_CurLevel);
}

void CLevelMgr::Progress()
{
	m_Loaded = false;

	if (nullptr == m_CurLevel)
		return;

	m_CurLevel->RegisterClear();

	if (m_CurLevel->GetState() == LEVEL_STATE::PLAY)
	{
		m_CurLevel->Tick();
	}

	m_CurLevel->FinalTick();
}

void CLevelMgr::RegisterObject(CGameObject* _Object)
{
	int LayerIdx = _Object->GetLayerIdx();
	CLayer* pLayer = m_CurLevel->GetLayer(LayerIdx);
	pLayer->RegisterObject(_Object);
}

CGameObject* CLevelMgr::FindObjectByName(const wstring& _Name)
{
	if (nullptr == m_CurLevel)
		return nullptr;

	return m_CurLevel->FindObjectByName(_Name);
}



void CLevelMgr::FindObjectsByName(const wstring& _Name, vector<CGameObject*>& _Out)
{
	if (nullptr == m_CurLevel)
		return;

	m_CurLevel->FindObjectsByName(_Name, _Out);
}



void CLevelMgr::ChangeLevelState(LEVEL_STATE _NextState)
{
	assert(m_CurLevel);
	if (m_CurLevel->GetState() == _NextState)
		return;

	CRenderMgr::GetInst()->ClearAnimEffObjects();

	// 상태 모드에 따라 시간, 렌더링 제어
	// Play or None ->  Pause 
	//			    ㄴ> Stop	
	if (_NextState == LEVEL_STATE::PAUSE || _NextState == LEVEL_STATE::STOP)
	{
		CTimeMgr::GetInst()->SetStopMode(true);
		CRenderMgr::GetInst()->SetEditorMode(true);

		if(LEVEL_STATE::STOP == _NextState)
			CRenderMgr::GetInst()->DeregisterCamera();
	}

	// Pause, Stop -> Play
	else if (_NextState == LEVEL_STATE::PLAY)
	{
		CTimeMgr::GetInst()->SetStopMode(false);
		CRenderMgr::GetInst()->SetEditorMode(false);
	}

	// Stop -> Play
	if ((m_CurLevel->GetState() == LEVEL_STATE::STOP || m_CurLevel->GetState() == LEVEL_STATE::NONE)
		&& _NextState == LEVEL_STATE::PLAY)
	{
		m_CurLevel->ChangeState(_NextState);
		m_CurLevel->Begin();
	}
	else
	{
		m_CurLevel->ChangeState(_NextState);
	}
}

void CLevelMgr::ChangeLevel(CLevel* _NextLevel, LEVEL_STATE _NextLevelState)
{
	if (nullptr == _NextLevel)
		return;

	CLevel* pPrevLevel = m_CurLevel;

	m_CurLevel = _NextLevel;
	m_CurPlayer = nullptr;

	ChangeLevelState(_NextLevelState);

	// 이전 레벨은 삭제
	if (nullptr != pPrevLevel)
		delete pPrevLevel;

	CRenderMgr::GetInst()->SetRenderCamera(nullptr);
}

CLevel* CLevelMgr::Load(const wstring& _FilePath)
{
	if (m_Loaded)
		return nullptr;
	else
		m_Loaded = true;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"rb");

	if (!pFile)
		return nullptr;

	CLevel* pNewLevel = new CLevel;


	// Level 이름 불러오기
	pNewLevel->LoadFromLevel(pFile);

	// Level 이 보유하고있는 32개의 레이어를 불러온다.
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pNewLevel->GetLayer(i);

		// 레이어 이름 불러오기
		pLayer->LoadFromLevel(pFile);

		// 오브젝트 총 개수
		size_t ObjectCount = 0;
		fread(&ObjectCount, sizeof(size_t), 1, pFile);

		// 각 오브젝트의 정보
		for (size_t j = 0; j < ObjectCount; ++j)
		{
			CGameObject* pNewObject = CPrefab::g_ObjectLoad(pFile);
			pLayer->AddObject(pNewObject, false);
		}
	}

	fclose(pFile);

	return pNewLevel;
}
