#pragma once
#include "CEntity.h"
#include "CLayer.h"
#include "CScript.h"

class CLayer;
class CGameObject;

class CLevel :
	public CEntity
{
private:
	LEVEL_STATE	m_State;
	CLayer m_arrLayer[MAX_LAYER];
	list<CGameObject*> m_vecParents;

public:
	CLayer* GetLayer(int _LayerIdx) { return &m_arrLayer[_LayerIdx]; }

	CGameObject* FindObjectByName(const wstring& _Name);
	void FindObjectsByName(const wstring& _Name, vector<CGameObject*>& _Out);

	list<CGameObject*> GetParentObjects() { return m_vecParents; }
	LEVEL_STATE GetState() { return m_State; }

	void RegisterClear()
	{
		// 이전프레임에 등록받았던 오브젝트 목록을 Clear 한다.
		// 클리어하지 않으면 regist object되면서
		// 렌더링 오브젝트들이 프레임마다 중첩될 것
		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			m_arrLayer[i].m_vecObjects.clear();
		}
	}

public:
	void Begin();
	void Tick();
	void FinalTick();

public:
	void AddObject(int _LayerIdx, CGameObject* _Object, bool _MoveWithChild);

private:
	void ChangeState(LEVEL_STATE _NextState);


public:
	virtual CLevel* Clone() { return new CLevel(*this); }
	CLevel();
	~CLevel();

	friend class CLevelMgr;
};

