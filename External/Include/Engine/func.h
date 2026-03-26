#pragma once

void CreateObject(class CGameObject* _NewObj, int LayerIdx, bool _bMoveChild);
void DestroyObject(class CGameObject* _TargetObj);
void ActivateObject(class CGameObject* _TargetObj);
void DeactivateObject(class CGameObject* _TargetObj);
void ChangeLevelState(LEVEL_STATE _NextState);
void ChangeLevel(wstring _LevelName);
void ChangeLevel(class CLevel* Level, LEVEL_STATE NextState);
void AddChild(CGameObject* _Parent, CGameObject* _Child);

CGameObject* CreateAnimObject();
CGameObject* CreateTriggerObject();
CGameObject* Instantiate(const wstring& _Key);

// 전역변수 인보크
void Invoke(void(*_Func)(), float _Time = 0.f, bool _Loop = false);
void Invoke_1(void(*_Func)(), DWORD_PTR _Param, float _Time = 0.f, bool _Loop = false);
void InvokeWithParam(void(*_Func)(DWORD_PTR), DWORD_PTR _Param, float _Time = 0.f, bool _Loop = false);
void InvokeWithParam2(void(*_Func)(DWORD_PTR, DWORD_PTR), DWORD_PTR _Param1, DWORD_PTR _Param2, float _Time = 0.f, bool _Loop = false);
void Invoke(std::function<void()> _Func, float _Time, bool _Loop = false);


void DrawDebugRect(Vector4 _Color, Vector3 _Pos, Vector2 _Scale, Vector3 _Rotation, bool _DepthTest = false , float _Duration = 0.f, bool _Filled = false);
void DrawDebugRect(Vector4 _Color, const Matrix& _matWorld, bool _DepthTest = false, float _Duration = 0.f, bool _Filled = false);

void DrawDebugCircle(Vector4 _Color, Vector3 _Pos, float _Radius, bool _DepthTest = false, float _Duration = 0.f, bool _Filled = false);

void DrawDebugLine(Vector4 _Color, Vector3 _Start, Vector3 _End, bool _DepthTest = false, float _Duration = 0.f);

void DrawDebugLog(string& _Str);
void DrawDebugLogW(wstring& _Str);

void TestLibrary();

Vector2 GetIntersectionPoint(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4);

bool GetSegmentIntersection(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, Vector2& intersection);

Vector3 GetRandomPosAround(const Vector3& _Center, float _Radius = 5.f);

float RandomRange(float min, float max);

int RandomRange(int min, int max);

void SaveWString(const wstring& _str, FILE* _File);
void LoadWString(wstring& _str, FILE* _File);
 
#include "Ptr.h"
#include "CAsset.h"

class CFlipbook;
void PlayHitAnim(Ptr<CFlipbook> _Anim, Vector3 _Pos, int _Count);

template<typename T>
void SaveAssetRef(Ptr<T> _Asset, FILE* _File)
{
	// 에셋에 대한 참조 정보를 저장
	bool bAsset = _Asset.Get();
	fwrite(&bAsset, sizeof(bool), 1, _File);

	if (bAsset)
	{
		SaveWString(_Asset->GetName(), _File);
		SaveWString(_Asset->GetKey(), _File);
		SaveWString(_Asset->GetRelativePath(), _File);
	}
}

template<typename T>
void LoadAssetRef(Ptr<T>& _Asset, FILE* _File)
{
	bool bAsset;
	fread(&bAsset, sizeof(bool), 1, _File);

	if (bAsset)
	{
		wstring Name, Key, Path;
		LoadWString(Name, _File);
		LoadWString(Key, _File);
		LoadWString(Path, _File);

		_Asset = CAssetMgr::GetInst()->Load<T>(Key, Path);
		_Asset->SetName(Name);
	}
}

template<typename T>
T* LoadAssetRef(FILE* _File)
{
	Ptr<T> pAsset = nullptr;

	bool bAsset;
	fread(&bAsset, sizeof(bool), 1, _File);

	if (bAsset)
	{
		wstring Name, Key, Path;
		LoadWString(Name, _File);
		LoadWString(Key, _File);
		LoadWString(Path, _File);

		pAsset = CAssetMgr::GetInst()->Load<T>(Key, Path);
		pAsset->SetName(Name);
	}

	return pAsset.Get();
}




template<typename T>
void CopyVec(vector<T>& _copy, const vector<T>& _orgin)
{
	_copy.clear();
	for (size_t i = 0; i < _orgin.size(); ++i)
	{
		_copy.push_back(_orgin[i]);
	}
}


template<typename T, int Count>
void DeleteArray(T* (&arr)[Count])
{
	for (UINT i = 0; i < Count; ++i)
	{
		DELETE(arr[i]);
	}

}


template<typename T>
void DeleteVec(vector<T>& _vec)
{
	for (size_t i = 0; i < _vec.size(); ++i)
	{
		DELETE(_vec[i]);
	}
	_vec.clear();
}


template<typename T>
void DeleteList(list<T>& _list)
{
	for (auto item : _list)
	{
		DELETE(item);
	}
	_list.clear();
}

template<typename T1, typename T2>
void DeleteMap(map<T1, T2>& _map)
{
	for (const auto& pair : _map)
	{
		delete pair.second;
	}
	_map.clear();
}
