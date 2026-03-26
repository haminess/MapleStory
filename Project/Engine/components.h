#pragma once

#include "CTransform.h"
//
#include "CCollider2D.h"
#include "CCamera.h"
//#include "CCollider3D.h"
#include "CFlipbookPlayer.h"
#include "CMeshRender.h"
#include "CRenderComponent.h"
#include "CTileMap.h"
#include "CLight2D.h"
#include "CRigidBody2D.h"
#include "CParticleSystem.h"
#include "CScript.h"
#include "CStateMachine.h"
#include "MTileMap.h"

// component 추가 시
// components.h		: include 추가
// extern.cpp		: com type 변수 추가
// CGameObject.h	: access 함수 추가
// CGameObject.h	: AddComponent case 추가
// CComponent.h		: GET_OTHER_COMPONENT 추가