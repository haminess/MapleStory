#pragma once
#include "CEntity.h"

class CComponent;
class CRenderComponent;
class CScript;

class CGameObject :
	public CEntity
{
private:
	CComponent* m_arrCom[(UINT)COMPONENT_TYPE::END];
	CRenderComponent*		m_RenderCom;    // 1개의 게임오브젝트 객체는 1개의 렌더 컴포넌트를 가질 수 있다.
	vector<CScript*>        m_vecScripts;   // 보유 스크립트들
	vector<string>			m_vecTags;		

	CGameObject* m_Parent;       // 부모 오브젝트
	vector<CGameObject*>    m_vecChild;     // 자식 오브젝트들

	int                     m_LayerIdx;     // 오브젝트가 속해있는 레이어 인덱스 번호, -1 : 무소속, Level에 쓰이지 않음
	bool                    m_Active;       // 오브젝트 활성화 상태
	bool                    m_Valid;
	bool                    m_Dead;         // 오브젝트의 삭제 예정인 상태값
	bool                    m_Render;

	// 레이어에 관하여..
	// 1) 렌더링
	// 일반 Material은 Z값에 따라 깊이테스트가 잘된다.
	// 이와 달리 AlphaBlend Material의 깊이 테스트는 layer index가 1순위, Z값이 2순위로 연산된다.
	// 깊이 테스트는 미리 그려진 픽셀과 이번에 새로 그릴 픽셀의 깊이를 비교한 후 렌더링한다.
	// 미리 그려진 픽셀보다 나중에 그릴 픽셀이 더 뒤에 있다면 그 픽셀은 버린다.
	// 따라서 레이어 1의 z 100 오브젝트와 레이어 2의 z -100 오브젝트가 있다면 후자는 렌더링되지 않음
	// 2) Tick
	// 이벤트함수 호출 순서는 다음과 같다. 
	// Level -> Layer 순서 -> Object ComponentTick / ScriptTick / ComponentFinalTick
	// Layer -> (Object Tick -> Child Tick) -> Object FinalTick

public:
	void Begin();
	void Tick();
	void FinalTick();
	void Destroy();
	void Render();

private:
	bool IsInCameraView();


public:
	// Game Object
	void SetActive(bool _bActive);
	bool IsActive() { return m_Active; }

	virtual void Activate();
	virtual void Deactivate();

	int GetLayerIdx() { return m_LayerIdx; }
	void SetLayerIdx(int _idx);
	bool IsDead() { return m_Dead; }
	bool IsValid() { return !m_Dead && m_Valid && m_Active; }
	bool IsRendering() { return m_Render; }
	bool IsAncestor(CGameObject* _Other);

	// child
	void AddChild(CGameObject* _Child);

	CGameObject* GetParent() { return m_Parent; }
	vector<CGameObject*> GetChild() { return m_vecChild; }
	CGameObject* GetChild(int _Num) { return _Num < m_vecChild.size()? m_vecChild[_Num] : nullptr; }


	// component
	CComponent* GetComponent(COMPONENT_TYPE _Type) { return m_arrCom[(UINT)_Type]; }
	CRenderComponent* GetRenderComponent() { return m_RenderCom; }

	CComponent* AddComponent(CComponent* _Component);
	CComponent* AddComponent(COMPONENT_TYPE _ComType);
	void DeleteComponent(COMPONENT_TYPE _ComType);


	template<typename T>
	T* GetScript()
	{
		T* pScript = nullptr;

		for (int i = 0; i < m_vecScripts.size(); ++i)
		{
			pScript = dynamic_cast<T*>(m_vecScripts[i]);
			if (pScript)
				return pScript;
		}
		return nullptr;
	}

	const vector<CScript*>& GetScripts() { return m_vecScripts; }
	class CTransform* Transform() { return (CTransform*)GetComponent(COMPONENT_TYPE::TRANSFORM); }
	class CMeshRender* MeshRender() { return (CMeshRender*)GetComponent(COMPONENT_TYPE::MESHRENDER); }
	class CCamera* Camera() { return (CCamera*)GetComponent(COMPONENT_TYPE::CAMERA); }
	class CCollider2D* Collider2D() { return (CCollider2D*)GetComponent(COMPONENT_TYPE::COLLIDER2D); }
	class CFlipbookPlayer* FlipbookPlayer() { return (CFlipbookPlayer*)GetComponent(COMPONENT_TYPE::FLIPBOOKPLAYER); }
	class CTileMap* TileMap() { return (CTileMap*)GetComponent(COMPONENT_TYPE::TILEMAP); }
	class CLight2D* Light2D() { return (CLight2D*)GetComponent(COMPONENT_TYPE::LIGHT2D); }
	class CRigidBody2D* RigidBody2D() { return (CRigidBody2D*)GetComponent(COMPONENT_TYPE::RIGIDBODY2D); }
	class CParticleSystem* ParticleSystem() { return (CParticleSystem*)GetComponent(COMPONENT_TYPE::PARTICLE_SYSTEM); }
	class CStateMachine* StateMachine() { return (CStateMachine*)GetComponent(COMPONENT_TYPE::STATEMACHINE); }


public:
	void DisconnectWithLayer();
	void DisconnectWithParent();
	void RegisterAsParent();

public:
	CLONE(CGameObject);
	CGameObject();
	CGameObject(const CGameObject& _Origin);
	~CGameObject();

	friend class CLayer;
	friend class CTaskMgr;
};

