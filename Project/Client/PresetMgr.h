#pragma once
#include "EditorUI.h"
#include "MTileEditor.h"
#include "MObjectEditor.h"
#include "PresetList.h"

class PresetList;
class PresetMgr :
    public EditorUI
{
private:
	// Background		: 배경 선택 및 배경 편집할 수 있게 하기
	// Tile				: 선택한 타일을 설치 (TileMap 수정)
	// Object			: 오브젝트 편집 및 맵 설치 (CObj)
	// Monster			: 몬스터 편집 및 설치 (CObj)
	// NPC				: 
	// FootHold			: 
	// Ladder & Rope	: 
	// Portal			: 
	// Particle			: 
	// Trap				: 
	// Sound			: 
	// Item				: 
	// SpawnLocation	: 
	// UI -> UI 전용 카메라 : 

	vector<PresetList*> m_vecPresetEditor;
	PresetList* m_CurEditor;

public:
	// 1. 프리셋 리스트 출력
	// 2. 프리셋 클릭하면 기능 작동하게 함
	// - 예) 타일 클릭하면 해당 에셋 적용, 오브젝트 드래그하면 설치할 수 있게 하기
	virtual void Render_Update() override;

	void CreatePresetEditor(PresetList* _Editor);
	void ChangeFocusEditor(PresetList* _Editor);

	void Init();
	virtual void Activate() override;
	virtual void Deactivate() override;


public:
	PresetMgr();
	~PresetMgr();
};

