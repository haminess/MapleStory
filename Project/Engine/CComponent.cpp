#include "pch.h"
#include "CComponent.h"
#include "components.h"

CComponent::CComponent(COMPONENT_TYPE _Type)
	: m_Type(_Type)
	, m_Active(true)
	, m_Initialized(false)
{
}

CComponent::~CComponent()
{
}

void CComponent::SaveToLevel(FILE* _File)
{
	CEntity::SaveToLevel(_File);

	SaveComponent(_File);
}

void CComponent::LoadFromLevel(FILE* _File)
{
	CEntity::LoadFromLevel(_File);

	LoadComponent(_File);
}

COMPONENT_TYPE CComponent::GetComponentType(const wstring& _StrType)
{
	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		string str = (char*)COMPONENT_TYPE_STRING[i];
		wstring strCompare = wstring(str.begin(), str.end());

		if (_StrType == strCompare)
			return (COMPONENT_TYPE)i;
	}
	return COMPONENT_TYPE::END;
}

CComponent* CComponent::GetComponent(COMPONENT_TYPE _ComType)
{
	switch (_ComType)
	{
	case COMPONENT_TYPE::TRANSFORM:
		return new CTransform;
	case COMPONENT_TYPE::COLLIDER2D:
		return new CCollider2D;
		//case COMPONENT_TYPE::COLLIDER3D:
			//return new CCollider3d;
	case COMPONENT_TYPE::RIGIDBODY2D:
		return new CRigidBody2D;
	case COMPONENT_TYPE::FLIPBOOKPLAYER:
		return new CFlipbookPlayer;
		//case COMPONENT_TYPE::ANIMATOR3D:
			//return new CAnimator3d;
	case COMPONENT_TYPE::CAMERA:
		return new CCamera;
	case COMPONENT_TYPE::LIGHT2D:
		return new CLight2D;
		//case COMPONENT_TYPE::LIGHT3D:
			//return new CLight3d;
	case COMPONENT_TYPE::STATEMACHINE:
		return new CStateMachine;
	case COMPONENT_TYPE::MESHRENDER:
		return new CMeshRender;
	case COMPONENT_TYPE::TILEMAP:
		return new MTileMap;
	case COMPONENT_TYPE::PARTICLE_SYSTEM:
		return new CParticleSystem;
	case COMPONENT_TYPE::MTILEMAP:
		return new MTileMap;
		//case COMPONENT_TYPE::SKYBOX:
			//return new CSkybox;
		//case COMPONENT_TYPE::DECAL:
			//return new CDecal;
		//case COMPONENT_TYPE::LANDSCALE:
			//return new CLandscale;
	}
	return nullptr;
}

