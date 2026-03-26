#include "pch.h"
#include "CTaskMgr.h"
#include "CGameObject.h"
#include "CLevel.h"
#include "CLevelMgr.h"
#include "CAssetMgr.h"
#include "CTimeMgr.h"
#include "CAsset.h"


CTaskMgr::CTaskMgr()
	: m_LevelChanged(false)
{
}
CTaskMgr::~CTaskMgr()
{
}

void CTaskMgr::Tick()
{
	m_LevelChanged = false;

	// 가비지 컬렉터
	for (size_t i = 0; i < m_vecGC.size(); ++i)
	{
		delete m_vecGC[i];
		m_LevelChanged = true;
	}
	m_vecGC.clear();

	// 활성 컬렉터
	for (size_t i = 0; i < m_vecDisabled.size(); ++i)
	{
		m_vecDisabled[i]->SetActive(false);
	}
	m_vecDisabled.clear();

	vector<tTask> vecLeftTask;
	for (size_t i = 0; i < m_vecTask.size(); ++i)
	{
		tTask& task = m_vecTask[i];

		// 타이머 설정
		if (0.f < task.Timer && task.Timer > task.ElapsedTime)
		{
			task.ElapsedTime += DT;
			vecLeftTask.push_back(task);
			continue;
		}


		switch (m_vecTask[i].Type)
		{
		case TASK_TYPE::INVOKE:
		{
			void (*pFunc)();
			pFunc = (void(*)())task.Param0;
			if (pFunc) pFunc();
		}
		case TASK_TYPE::INVOKE_PARAM1:
		{
			void (*pFunc)(DWORD_PTR);
			pFunc = (void(*)(DWORD_PTR))task.Param0;
			if (pFunc) pFunc(task.Param1);
		}
		break;
		case TASK_TYPE::INVOKE_PARAM2:
		{
			void (*pFunc)(DWORD_PTR, DWORD_PTR);
			pFunc = (void(*)(DWORD_PTR, DWORD_PTR))task.Param0;
			if (pFunc) pFunc(task.Param1, task.Param2);
		}
		break;
		case TASK_TYPE::INVOKE_FUNC:
		{
			auto func = (std::function<void()>*)task.Param0;
			(*func)();
			if (!task.IsLoop)
				delete func;
		}
		break;
		case TASK_TYPE::ADD_CHILD:
		{
			CGameObject* pParent = (CGameObject*)task.Param0;
			CGameObject* pChild = (CGameObject*)task.Param1;
			CGameObject* pPrevParent = pChild->GetParent();

			// 레벨 오브젝트 리스트에도 반영해야 한다.

			// 이동할 부모가 없을 때
			if (nullptr == pParent)
			{
				if (pPrevParent)
				{
					pChild->DisconnectWithParent();
					pChild->RegisterAsParent();
				}
			}

			// 이동할 부모가 있을 때
			else
			{
				// 움직일 오브젝트가 자식 오브젝트이면
				if (pPrevParent)
				{
					pChild->DisconnectWithParent();
				}

				// 움직일 오브젝트가 최상위 부모 오브젝트이면
				else
				{
					int LayerIdx = pChild->GetLayerIdx();
					pChild->DisconnectWithLayer();

					if (-1 == pChild->m_LayerIdx)
					{
						pChild->m_LayerIdx = pParent->GetLayerIdx();

						// 레벨 도중에 생성된 오브젝트는 Begin 호출되어야 한다.
						CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
						if (pCurLevel->GetState() == LEVEL_STATE::PLAY || pCurLevel->GetState() == LEVEL_STATE::PAUSE)
						{
							pChild->Begin();
						}
					}
					else
						pChild->SetLayerIdx(LayerIdx);
				}

				pParent->AddChild(pChild);
			}

			m_LevelChanged = true;
		}
		break;
		case TASK_TYPE::CHANGE_LEVEL:
		{
			m_vecScriptTimer.clear();

			// 레벨 전환
			CLevel* pNextLevel = (CLevel*)task.Param0;

			LEVEL_STATE eNextState = (LEVEL_STATE)task.Param1;

			CLevelMgr::GetInst()->ChangeLevel(pNextLevel, eNextState);

			m_LevelChanged = true;
		}
		break;
		case TASK_TYPE::CREATE_OBJECT:
		{
			CGameObject* pNewObject = (CGameObject*)task.Param0;
			int LayerIdx = (int)task.Param1;

			CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			pCurLevel->AddObject(LayerIdx, pNewObject, false);

			m_LevelChanged = true;

			// 레벨 플레이가 아닐 때 생성된 오브젝트는 Play 시작할 때 Begin 호출되어야 한다.
			// 레벨 도중에 생성된 오브젝트는 Begin 호출되어야 한다.
			if (pCurLevel->GetState() == LEVEL_STATE::PLAY || pCurLevel->GetState() == LEVEL_STATE::PAUSE)
			{
				pNewObject->Begin();
			}
		}
		break;
		case TASK_TYPE::DELETE_OBJECT:
		{
			CGameObject* pObject = (CGameObject*)task.Param0;
			if (!pObject->IsDead())
			{
				pObject->m_Dead = true;
				m_vecGC.push_back(pObject);
			}
		}
		break;
		case TASK_TYPE::DISABLE_OBJECT:
		{
			CGameObject* pObject = (CGameObject*)task.Param0;
			if (!pObject->IsDead())
			{
				pObject->m_Valid = false;
				m_vecDisabled.push_back(pObject);

				if (pObject->m_Active)
					pObject->Activate();
				else
					pObject->Deactivate();
			}
		}
		break;
		case TASK_TYPE::CHANGE_LEVEL_STATE:
		{
			LEVEL_STATE eNextState = (LEVEL_STATE)task.Param0;
			CLevelMgr::GetInst()->ChangeLevelState(eNextState);
		}
		break;
		case TASK_TYPE::DELETE_ASSET:
		{
			Ptr<CAsset> pAsset = (CAsset*)task.Param0;
			CAssetMgr::GetInst()->DeleteAsset(pAsset->GetAssetType(), pAsset->GetKey());
		}
		break;
		}
	}

	m_vecTask.clear();
	m_vecTask = vecLeftTask;
}

void CTaskMgr::ExcuteTimer()
{
	// 이번 프레임에 추가된 타이머 추가
	for (auto& Info : m_vecTimerAdd)
	{
		const size_t index = m_vecScriptTimer.size();
		m_vecScriptTimer.emplace_back(std::move(Info));

		m_TimerIndex[Info.Id] = index;
	}
	m_vecTimerAdd.clear();

	// 다음 프레임에도 실행할 타이머
	vector<tScriptTimerInfo> vecNextTimer;
	vecNextTimer.reserve(m_vecScriptTimer.size());

	// 타이머 ID 테이블 정보 업데이트
	unordered_map<UINT, size_t> nextIndex;
	nextIndex.reserve(m_vecScriptTimer.size());

	for (size_t i = 0; i < m_vecScriptTimer.size(); ++i)
	{
		tScriptTimerInfo& timer = m_vecScriptTimer[i];
		timer.ElapsedTime += DT;

		// Destroy된 오브젝트 예외처리
		if (timer.Inst == nullptr) continue;
		if (timer.Inst->IsActive() == false) continue;
		if (timer.Inst->GetOwner() == nullptr) continue;
		if (timer.Inst->GetOwner()->IsDead()) continue;

		// 타이머 설정
		if (timer.ElapsedTime < timer.Timer)
		{
			size_t idx = vecNextTimer.size();
			vecNextTimer.push_back(timer);
			nextIndex[timer.Id] = idx;
			continue;
		}

		// 델리게이트 실행
		if (timer.Func) (timer.Inst->*timer.Func)();
		else if (timer.Func_1) (timer.Inst->*timer.Func_1)(timer.Param0);
		else if (timer.Func_2) (timer.Inst->*timer.Func_2)(timer.Param0, timer.Param1);

		// Loop이면 다음 프레임에 추가
		if (timer.IsLoop)
		{
			size_t idx = vecNextTimer.size();
			nextIndex[timer.Id] = idx;

			timer.ElapsedTime = 0.f;
			vecNextTimer.push_back(timer);
		}
	}

	m_vecScriptTimer.swap(vecNextTimer);
	m_TimerIndex.swap(nextIndex);
}
