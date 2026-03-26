#pragma once
//#include "EditorUI.h"
#include "PresetList.h"
#include "Engine/assets.h"

enum class EObjectEditorMode
{
	NONE,
	PLATFORM,
	ROPE,
	WALL,
};

class CTexture;
class MObjectEditor :
	public PresetList
{
private:
	vector<Ptr<CPrefab>> m_vecPrefabs;
	CGameObject* m_NewObject;

	// Create Platform
	bool m_IsPlatformMode;
	EObjectEditorMode m_EditMode;

	// Mouse Drag
	Vector3 m_MouseStartPos;

public:
	void LoadPrefab(Ptr<CPrefab> _New);
	void CreateTexturePrefab(DWORD_PTR , DWORD_PTR);
	void AddPrefabFromListUI(DWORD_PTR , DWORD_PTR);
	void CreatePresetObject(int _LayerIdx = 0);
	void CreatePlatform(Vector2& _MouseCurPos);
	void CreateRope(Vector2& _MouseCurPos);
	void CreateWall(Vector2& _MouseCurPos);
	virtual void SelectPreset(int _Idx) override;


public:
	virtual void Render_Editor() override;

public:
	void Load();
	void Save();

public:
	MObjectEditor();
	~MObjectEditor();
};

