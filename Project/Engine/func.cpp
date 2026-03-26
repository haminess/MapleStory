#include "pch.h"
#include "func.h"
#include "CPathMgr.h"
#include "CRenderMgr.h"
#include "CTaskMgr.h"
#include "CLevelMgr.h"
#include "CEngine.h"
#include "CGameObject.h"
#include "CLevel.h"
#include "CFlipbook.h"


void CreateObject(CGameObject* _NewObj, int LayerIdx, bool _bMoveChild)
{
	tTask task = {};
	task.Type = TASK_TYPE::CREATE_OBJECT;
	task.Param0 = (DWORD_PTR)_NewObj;
	task.Param1 = LayerIdx;
	task.Param2 = _bMoveChild;

	_NewObj->SetLayerIdx(LayerIdx);

	CTaskMgr::GetInst()->AddTask(task);
}

void DestroyObject(CGameObject* _TargetObj)
{
	tTask task = {};
	task.Type = TASK_TYPE::DELETE_OBJECT;
	task.Param0 = (DWORD_PTR)_TargetObj;

	CTaskMgr::GetInst()->AddTask(task);
}

void ActivateObject(CGameObject* _TargetObj)
{
	_TargetObj->SetActive(true);
}

void DeactivateObject(CGameObject* _TargetObj)
{
	tTask task = {};
	task.Type = TASK_TYPE::DISABLE_OBJECT;
	task.Param0 = (DWORD_PTR)_TargetObj;

	CTaskMgr::GetInst()->AddTask(task);
}

void ChangeLevelState(LEVEL_STATE _NextState)
{
	tTask task = {};
	task.Type = TASK_TYPE::CHANGE_LEVEL_STATE;
	task.Param0 = (DWORD_PTR)_NextState;

	CTaskMgr::GetInst()->AddTask(task);
}

void ChangeLevel(wstring _LevelName)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += L"Level\\" + _LevelName + L".lv";

	// 레벨 정보 읽기
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");

	assert(pFile);

	CLevel* pLevel = CLevelMgr::GetInst()->Load(strFilePath);

	fclose(pFile);

	ChangeLevel(pLevel, LEVEL_STATE::PLAY);
}

void ChangeLevel(CLevel* _Level, LEVEL_STATE _NextState)
{
	tTask task = {};

	task.Type = TASK_TYPE::CHANGE_LEVEL;
	task.Param0 = (DWORD_PTR)_Level;
	task.Param1 = (DWORD_PTR)_NextState;

	CTaskMgr::GetInst()->AddTask(task);
}



void AddChild(CGameObject* _Parent, CGameObject* _Child)
{
	tTask task = {};

	task.Type = TASK_TYPE::ADD_CHILD;
	task.Param0 = (DWORD_PTR)_Parent;
	task.Param1 = (DWORD_PTR)_Child;

	CTaskMgr::GetInst()->AddTask(task);
}

CGameObject* CreateAnimObject()
{
	CGameObject* pNew = new CGameObject;
	pNew->SetName(L"Anim Object");

	// 컴포넌트 추가
	pNew->AddComponent(new CMeshRender);
	pNew->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));

	pNew->AddComponent(new CFlipbookPlayer);

	return pNew;
}

CGameObject* CreateTriggerObject()
{
	CGameObject* pNew = CreateAnimObject();
	pNew->SetName(L"Trigger Object");

	// 컴포넌트 추가
	pNew->AddComponent(new CCollider2D);
	pNew->Collider2D()->SetIndependentScale(false);
	pNew->Collider2D()->SetOffset(Vector2(0.f, 0.f));
	pNew->Collider2D()->SetScale(Vector2(1.f, 1.f));

	return pNew;
}

CGameObject* Instantiate(const wstring& _FileName)
{
	Ptr<CPrefab> pPrefab = CAssetMgr::GetInst()->FindPrefabByName(_FileName);

	if (pPrefab != nullptr)
		return pPrefab->Instantiate();

	return nullptr;
}


void Invoke(void(*_Func)(), float _Time, bool _Loop)
{
	tTask task = {};

	task.Type = TASK_TYPE::INVOKE;
	task.Param0 = (DWORD_PTR)_Func;
	task.Timer = _Time;
	task.ElapsedTime = 0.f;
	task.IsLoop = _Loop;

	CTaskMgr::GetInst()->AddTask(task);
}

void Invoke_1(void(*_Func)(), DWORD_PTR _Param, float _Time, bool _Loop)
{
	tTask task = {};

	task.Type = TASK_TYPE::INVOKE;
	task.Param0 = (DWORD_PTR)_Func;
	task.Param1 = (DWORD_PTR)_Param;
	task.Timer = _Time;
	task.ElapsedTime = 0.f;
	task.IsLoop = _Loop;

	CTaskMgr::GetInst()->AddTask(task);
}


void InvokeWithParam(void(*_Func)(DWORD_PTR), DWORD_PTR _Param, float _Time, bool _Loop)
{
	tTask task = {};

	task.Type = TASK_TYPE::INVOKE_PARAM1;
	task.Param0 = (DWORD_PTR)_Func;
	task.Param1 = (DWORD_PTR)_Param;
	task.Param2 = (DWORD_PTR)_Param;
	task.Timer = _Time;
	task.ElapsedTime = 0.f;
	task.IsLoop = _Loop;

	CTaskMgr::GetInst()->AddTask(task);
}

void InvokeWithParam2(void(*_Func)(DWORD_PTR, DWORD_PTR), DWORD_PTR _Param1, DWORD_PTR _Param2, float _Time, bool _Loop)
{
	tTask task = {};

	task.Type = TASK_TYPE::INVOKE_PARAM2;
	task.Param0 = (DWORD_PTR)_Func;
	task.Param1 = (DWORD_PTR)_Param1;
	task.Param2 = (DWORD_PTR)_Param2;
	task.Timer = _Time;
	task.ElapsedTime = 0.f;
	task.IsLoop = _Loop;

	CTaskMgr::GetInst()->AddTask(task);
}

void Invoke(std::function<void()> _Func, float _Time, bool _Loop)
{
	tTask task = {};
	task.Type = TASK_TYPE::INVOKE_FUNC;
	task.Param0 = (DWORD_PTR)new std::function<void()>(_Func);
	task.Timer = _Time;
	task.ElapsedTime = 0.f;
	task.IsLoop = _Loop;
	CTaskMgr::GetInst()->AddTask(task);
}



void DrawDebugRect(Vector4 _Color, Vector3 _Pos, Vector2 _Scale, Vector3 _Rotation, bool _DepthTest, float _Duration, bool _Filled)
{
	tDebugShapeInfo info = {};
	if (_Filled)
		info.Shape = DEBUG_SHAPE::RECT_FILLED;
	else
		info.Shape = DEBUG_SHAPE::RECT;
	info.Color = _Color;
	info.Scale = Vector3(_Scale.x, _Scale.y, 1.f);
	info.Rotation = _Rotation;
	info.WorldPos = _Pos;
	info.matWorld = XMMatrixIdentity();
	info.Duration = _Duration;
	info.DepthTest = _DepthTest;

	CRenderMgr::GetInst()->AddDebugShape(info);
}

void DrawDebugRect(Vector4 _Color, const Matrix& _matWorld, bool _DepthTest, float _Duration, bool _Filled)
{
	tDebugShapeInfo info = {};
	if (_Filled)
		info.Shape = DEBUG_SHAPE::RECT_FILLED;
	else
		info.Shape = DEBUG_SHAPE::RECT;
	info.Color = _Color;
	info.matWorld = _matWorld;
	info.Duration = _Duration;
	info.DepthTest = _DepthTest;

	CRenderMgr::GetInst()->AddDebugShape(info);
}

void DrawDebugCircle(Vector4 _Color, Vector3 _Pos, float _Radius, bool _DepthTest, float _Duration, bool _Filled)
{
	tDebugShapeInfo info = {};
	if (_Filled)
		info.Shape = DEBUG_SHAPE::CIRCLE_FILLED;
	else
		info.Shape = DEBUG_SHAPE::CIRCLE;
	info.Color = _Color;
	info.Scale = Vector3(_Radius, _Radius, 1.f);
	info.WorldPos = _Pos;
	info.matWorld = XMMatrixIdentity();
	info.Duration = _Duration;
	info.DepthTest = _DepthTest;

	CRenderMgr::GetInst()->AddDebugShape(info);
}

void DrawDebugLine(Vector4 _Color, Vector3 _Start, Vector3 _End, bool _DepthTest, float _Duration)
{
	Vector3 vecDir = _End - _Start;


	tDebugShapeInfo info = {};
	info.Shape = DEBUG_SHAPE::LINE;
	info.Color = _Color;
	info.Scale = Vector3(vecDir.Length(), 5.f, 5.f);
	info.WorldPos = _Start;
	info.Rotation = Vector4(0.f, 0.f, atan2f(vecDir.y, vecDir.x), 1.f);
	info.matWorld = XMMatrixIdentity();
	info.Duration = _Duration;
	info.DepthTest = _DepthTest;

	CRenderMgr::GetInst()->AddDebugShape(info);
}

void DrawDebugLog(string& _Str)
{
	_Str += "\n";
	OutputDebugStringA(_Str.c_str());
}

void DrawDebugLogW(wstring& _Str)
{
	_Str += L"\n";
	OutputDebugStringW(_Str.c_str());
}

void TestLibrary()
{
	SetWindowText(CEngine::GetInst()->GetMainWnd(), L"라이브러리가 잘 연동되고 있습니다.");
}

Vector2 GetIntersectionPoint(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4)
{
	Vector2 v1 = p2 - p1;
	Vector2 v2 = p4 - p3;
	Vector2 v3 = p3 - p1;

	float cross_v1_v2 = v1.x * v2.y - v1.y * v2.x;

	// 평행 체크를 위한 임계값 추가
	const float EPSILON = 1e-6f;
	if (abs(cross_v1_v2) < EPSILON)
	{
		// Lines are parallel
		return Vector2(FLT_MAX, FLT_MAX);
	}

	float t = (v3.x * v2.y - v3.y * v2.x) / cross_v1_v2;
	float s = (v3.x * v1.y - v3.y * v1.x) / cross_v1_v2;

	// 선분 범위 체크
	if (t < 0.0f || t > 1.0f || s < 0.0f || s > 1.0f)
	{
		// Intersection outside segments
		return Vector2(FLT_MAX, FLT_MAX);
	}

	return p1 + v1 * t;
}

bool GetSegmentIntersection(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, Vector2& intersection)
{
	Vector2 v1 = p2 - p1;
	Vector2 v2 = p4 - p3;
	Vector2 v3 = p3 - p1;

	float cross_v1_v2 = v1.x * v2.y - v1.y * v2.x;

	if (abs(cross_v1_v2) < FLT_EPSILON)
		return false;  // 평행

	float t = (v3.x * v2.y - v3.y * v2.x) / cross_v1_v2;
	float s = (v3.x * v1.y - v3.y * v1.x) / cross_v1_v2;

	// 선분 범위 검사 (0 <= t,s <= 1)
	if (t < 0.0f || t > 1.0f || s < 0.0f || s > 1.0f)
		return false;

	intersection = p1 + v1 * t;
	return true;
}

Vector3 GetRandomPosAround(const Vector3& _Center, float _Radius)
{
	// 랜덤 위치 생성 시 중심 쏠림을 방지하기 위해 
	// 거리값에 제곱근을 적용한 균등 분포 방식을 사용
	float x = ((float)rand() / RAND_MAX * 2.f - 1.f) * _Radius;
	float y = ((float)rand() / RAND_MAX * 2.f - 1.f) * _Radius;

	return Vector3(_Center.x + x, _Center.y + y, _Center.z);
}

float RandomRange(float min, float max)
{
	// 0 ~ 1
	float t = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	return min + (max - min) * t;
}

int RandomRange(int min, int max)
{
	return min + rand() % (max - min + 1);
}

void PlayHitAnim(Ptr<CFlipbook> _Anim, Vector3 _Pos, int _Count)
{
	while (_Count--)
	{
		CRenderMgr::GetInst()->PlayAnimEff(_Anim, nullptr, GetRandomPosAround(_Pos, 20.f));
	}
}

void SaveWString(const wstring& _str, FILE* _File)
{
	size_t Len = _str.length();
	fwrite(&Len, sizeof(size_t), 1, _File);
	fwrite(_str.c_str(), sizeof(wchar_t), Len, _File);
}

void LoadWString(wstring& _str, FILE* _File)
{
	wchar_t szBuffer[255] = {};

	size_t Len = 0;
	fread(&Len, sizeof(size_t), 1, _File);
	fread(szBuffer, sizeof(wchar_t), Len, _File);
	_str = szBuffer;
}