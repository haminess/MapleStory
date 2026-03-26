#pragma once


typedef DirectX::SimpleMath::Vector2	Vector2;
typedef DirectX::SimpleMath::Vector3	Vector3;
typedef DirectX::SimpleMath::Vector4	Vector4;
typedef DirectX::SimpleMath::Matrix		Matrix;

typedef int(*GAMEOBJECT_SAVE)(class CGameObject*, FILE*);
typedef class CGameObject* (*GAMEOBJECT_LOAD)(FILE*);

// CScript를 상속받은 여러 스크립트 클래스들이 공용으로 사용할 수 있는 델리게이트
// 델리게이트 타입을 CScript로 하고 함수 포인터에 cscript를 상속받은 자식 클래스 스크립트의 함수를 등록
class CScript;
typedef void(CScript::* SCRIPT_DELEGATE)(void);
typedef void(CScript::* SCRIPT_DELEGATE_1)(DWORD_PTR);
typedef void(CScript::* SCRIPT_DELEGATE_2)(DWORD_PTR, DWORD_PTR);


#define RELEASE(Inst) if(nullptr != Inst) Inst->Release();

#define DEVICE CDevice::GetInst()->GetDevice().Get()
#define CONTEXT CDevice::GetInst()->GetContext().Get()

#define DELETE(p) if(nullptr != p) {delete p; p = nullptr;}
#define DELETE_ARR(p) if(nullptr != p) {delete[] p; p = nullptr;}

#define MAX_LAYER 32


#define KEY_CHECK(KEY, STATE) CKeyMgr::GetInst()->GetKeyState(KEY) == STATE

#define KEY_TAP(KEY) KEY_CHECK(KEY, KEY_STATE::TAP)
#define KEY_RELEASED(KEY) KEY_CHECK(KEY, KEY_STATE::RELEASED)
#define KEY_PRESSED(KEY) KEY_CHECK(KEY, KEY_STATE::PRESSED)
#define KEY_NONE(KEY) KEY_CHECK(KEY, KEY_STATE::NONE)

#define DT CTimeMgr::GetInst()->GetDeltaTime()
#define GameTime CTimeMgr::GetInst()->GetTime()
#define EngineDT CTimeMgr::GetInst()->GetEngineDeltaTime()

#define XI_PI 3.141592653f

#define Radian(_Degree) _Degree / 180.f * XM_PI
#define Degree(_Radian) _Radian / XI_PI * 180.f

#define DEBUG_LOG(Message) DrawDebugLog(string(Message));

#define CLONE(Type) virtual Type* Clone() { return new Type(*this); }
#define CLONE_DISABLE(Type) virtual Type* Clone() { return nullptr; }\
							Type(const Type& _Other) = delete;

#define FIND_ANIM(Key) CAssetMgr::GetInst()->FindAsset<CFlipbook>(Key)
#define FIND_PREFAB(Key) CAssetMgr::GetInst()->FindAsset<CPrefab>(Key)

#define X(EName) EName,
