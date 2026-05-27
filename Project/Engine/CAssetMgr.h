#pragma once
#include "singleton.h"
#include "assets.h"
#include "CPathMgr.h"

class CAssetMgr :
	public singleton<CAssetMgr>
{
	SINGLE(CAssetMgr);
private:
	map<wstring, Ptr<CAsset>>	m_mapAsset[(UINT)ASSET_TYPE::END];
	bool                        m_bAssetChanged;

public:
	void Init();

	bool IsAssetChanged()
	{
		bool Changed = m_bAssetChanged;
		m_bAssetChanged = false;
		return Changed;
	}

private:
	void CreateEngineMesh();
	void CreateEngineTexture();
	void CreateEngineGraphicShader();
	void CreateEngineComputeShader();
	void CreateEngineMaterial();
	void CreateEngineSprite();
	void CreateEnginePrefab();

public:
	void SaveAllPrefab();


public:
	template<typename T>
	int AddAsset(const wstring& _Key, Ptr<T> _Asset);

	template<typename T>
	Ptr<T> FindAsset(const wstring& _Key);

	template<typename T>
	void DeleteAsset(const wstring& _Key);

	template<typename T>
	Ptr<T> Load(const wstring& _Key, const wstring& _RelativePath);

	// 부분 특수화(강사님피셜), 명시적 특수화(오피셜) 
	// ComputeShader 전용으로 사용하고 싶을 때 이와 같이 사용
	// template<> 선언하지 않으면 위 함수 T에 ComputeShader가 들어왔을 때를 가정해 같은 함수를 재정의했다고 간주해 에러 발생
	template<>
	Ptr<CComputeShader> Load(const wstring& _Key, const wstring& _RelativePath)
	{
		return FindAsset<CComputeShader>(_Key).Get();
	}



	Ptr<CTexture> CreateTexture(const wstring& _Key, UINT _Width, UINT _Height, DXGI_FORMAT _PixelFormat, UINT _BindFlag, D3D11_USAGE _Usage = D3D11_USAGE_DEFAULT);
	Ptr<CTexture> CreateTexture(const wstring& _Key, ComPtr<ID3D11Texture2D> _Tex2D);

	Ptr<CFlipbook> CreateFlipbook(const wstring& _Key, Ptr<CTexture> _Atlas, Vector2 _Count, Vector2 _Size, Vector2 _LeftTop = Vector2(0.f, 0.f));

	void GetAssetNames(ASSET_TYPE _Type, vector<wstring>& _vecAssetNames);
	map<wstring, Ptr<CAsset>> GetAssets(ASSET_TYPE _Type) { return m_mapAsset[(int)_Type]; }
	void DeleteAsset(ASSET_TYPE _Type, const wstring& _Key);

public:
	Ptr<CPrefab> FindPrefabByName(const wstring& _FileName)
	{
		return FindAsset<CPrefab>(L"Prefab\\" + _FileName + L".pref");
	}

};


// constexpr 변수 템플릿, 템플릿 특수화 사용
template<typename T1, typename T2>
constexpr bool IsTypeSame = false;
template<typename T>
constexpr bool IsTypeSame<T, T> = true;



template<typename T>
inline ASSET_TYPE GetAssetType()
{
	{

		//// 1. type_info
		//// 클래스 or 구조체 + 가상함수
		//const type_info& info = typeid(T);
		//size_t code = info.hash_code();

		////if (info.name == L"class CMesh")
		////	return ASSET_TYPE::MESH;
		//if (code == typeid(CMesh).hash_code())
		//	return ASSET_TYPE::MESH;

		////if (code == typeid(CMeshData).hash_code())
		////	return ASSET_TYPE::MESHDATA;
		//if (code == typeid(CGraphicShader).hash_code())
		//	return ASSET_TYPE::GRAPHIC_SHADER;
		////if (code == typeid(CMaterial).hash_code())
		////	return ASSET_TYPE::MATERIAL;
		////if (code == typeid(CPrefab).hash_code())
		////	return ASSET_TYPE::PREFAB;
		////if (code == typeid(CSound).hash_code())
		////	return ASSET_TYPE::SOUND;
		////if (code == typeid(CFlipBook).hash_code())
		////	return ASSET_TYPE::FLIPBOOK;
		////if (code == typeid(CSprite).hash_code())
		////	return ASSET_TYPE::SPRITE;
		////if (code == typeid(CTexture).hash_code())
		////	return ASSET_TYPE::TEXTURE;



		//// 2. constexpr 사용
		//// 엄격한 상수, 컴파일 시간 내에 값이 결정되어야 함
		//constexpr;
		//// 컴파일 시간에 비교 결과 정해짐
		//// 런타임 도중 비교 X
		//// 예를 들어 이 함수가 CMesh 템플릿이라면
		//// 비교구문 다 사라지고 return ASSET_TYPE::MESH 만 남음
		//if constexpr (info.name == L"class CMesh")
		//	return ASSET_TYPE::MESH;

		// if문에 constexpr 키워드 함께 사용되면
		// CMesh 전용 템플릿에서는 컴파일 단계에서 한줄만 남음
		// 컴파일 시간에 모든게 결정되기 때문
		// return ASSET_TYPE::MESH;
		// if constexpr (IsTypeSame<T, CMesh>)
		// 	return ASSET_TYPE::MESH;
	}

	if constexpr (std::is_same_v<T, CMesh>)
		return ASSET_TYPE::MESH;
	if constexpr (std::is_same_v<T, CGraphicShader>)
		return ASSET_TYPE::GRAPHIC_SHADER;
	if constexpr (IsTypeSame<T, CComputeShader>)
		return ASSET_TYPE::COMPUTE_SHADER;

	//if constexpr (IsTypeSame<T, CMeshData>)
	//	return ASSET_TYPE::MESHDATA;
	if constexpr (IsTypeSame<T, CMaterial>)
		return ASSET_TYPE::MATERIAL;
	if constexpr (IsTypeSame<T, CPrefab>)
		return ASSET_TYPE::PREFAB;
	if constexpr (IsTypeSame<T, CSound>)
		return ASSET_TYPE::SOUND;
	if constexpr (IsTypeSame<T, CFlipbook>)
		return ASSET_TYPE::FLIPBOOK;
	if constexpr (IsTypeSame<T, CSprite>)
		return ASSET_TYPE::SPRITE;
	if constexpr (IsTypeSame<T, CTexture>)
		return ASSET_TYPE::TEXTURE;
	if constexpr (IsTypeSame<T, MTile>)
		return ASSET_TYPE::MTILE;
	if constexpr (IsTypeSame<T, CFont>)
		return ASSET_TYPE::FONT;


	return ASSET_TYPE::END;
}

template<typename T>
inline int CAssetMgr::AddAsset(const wstring& _Key, Ptr<T> _Asset)
{
	ASSET_TYPE Type = GetAssetType<T>();
	_Asset->SetName(_Key);
	_Asset->SetKey(_Key);
	m_mapAsset[(UINT)Type].insert(make_pair(_Key, _Asset.Get()));

	m_bAssetChanged = true;

	return S_OK;
}

template<typename T>
inline Ptr<T> CAssetMgr::FindAsset(const wstring& _Key)
{
	ASSET_TYPE Type = GetAssetType<T>();

	map<wstring, Ptr<CAsset>>::iterator iter = m_mapAsset[(UINT)Type].find(_Key);

	if (iter == m_mapAsset[(UINT)Type].end())
		return nullptr;

	return (T*)iter->second.Get();
}

template<typename T>
inline void CAssetMgr::DeleteAsset(const wstring& _Key)
{
	ASSET_TYPE Type = GetAssetType<T>();

	map<wstring, Ptr<CAsset>>::iterator iter = m_mapAsset[(UINT)Type].find(_Key);

	if (iter == m_mapAsset[(UINT)Type].end())
		return;

	m_mapAsset[(UINT)Type].erase(iter);
}

template<typename T>
inline Ptr<T> CAssetMgr::Load(const wstring& _Key, const wstring& _RelativePath)
{
	Ptr<CAsset> pAsset = FindAsset<T>(_Key).Get();

	// 1. 있으면 그걸 반환
	if (nullptr != pAsset)
		return (T*)pAsset.Get();

	// 텍스쳐 파일 경로
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + _RelativePath;

	// 에셋 객체 생성 및 로딩
	pAsset = new T;

	pAsset->SetName(_Key);
	if (FAILED(pAsset->Load(strFilePath)))
	{
		pAsset = nullptr;
		return nullptr;
	}

	// 로딩이 완료된 에셋에 본인의 Key, RelativePath 세팅
	pAsset->SetKey(_Key);
	pAsset->SetRelativePath(_RelativePath);

	// 에셋을 맵에 등록
	ASSET_TYPE Type = GetAssetType<T>();
	m_mapAsset[(UINT)Type].insert(make_pair(_Key, pAsset));

	m_bAssetChanged = true;

	return (T*)pAsset.Get();
}

