#include "pch.h"
#include "Inspector.h"
#include "ComponentUI.h"
#include "CImGuiMgr.h"
#include "Outliner.h"
#include "AssetUI.h"
#include "LevelInfoUI.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/assets.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTaskMgr.h>

Inspector::Inspector()
	: EditorUI("Inspector")
	, m_arrComUI{}
{
	CreateComponentUI();
	CreateAssetUI();

	SetTargetObject(nullptr);
	SetTargetAsset(nullptr);
}

Inspector::~Inspector()
{
}


void Inspector::SetTargetObject(CGameObject* _Target)
{
	// 자식 UI 들에게 TargetObject 를 찾아서 알려준다.
	m_TargetObject = _Target;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == m_arrComUI[i])
			continue;

		m_arrComUI[i]->SetTargetObject(m_TargetObject);
	}

	// 오브젝트가 소유한 Script 에 대응하는 ScriptUI 를 생성해서 매칭시켜준다.
	if (nullptr == m_TargetObject)
	{
		for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
		{
			m_vecScriptUI[i]->SetScript(nullptr);
		}
	}
	else
	{
		const vector<CScript*>& vecScripts = m_TargetObject->GetScripts();

		for (size_t i = 0; i < vecScripts.size(); ++i)
		{
			if (m_vecScriptUI.size() <= i)
			{
				ScriptUI* pUI = new ScriptUI;
				m_vecScriptUI.push_back(pUI);

				AddChildUI(m_vecScriptUI.back());
			}

			m_vecScriptUI[i]->SetScript(vecScripts[i]);
		}

		for (size_t i = vecScripts.size(); i < m_vecScriptUI.size(); ++i)
		{
			m_vecScriptUI[i]->SetScript(nullptr);
		}
	}

	// AssetUI 는 비활성화
	if (nullptr != _Target)
		SetTargetAsset(nullptr);

	m_LevelUI->SetActive(false);


//#ifdef _DEBUG
//	const vector<CScript*>& scripts = m_TargetObject->GetScripts();
//	ScriptUI* pScriptUI = nullptr;
//	int ScriptCount = 0;
//	for (UINT i = 0; i < scripts.size(); ++i)
//	{
//		if (scripts[i]->GetScriptName() == "")
//			continue;
//
//		// scriptui 있으면 타깃만 바꿔주기
//		if (m_vecScriptUI.size() > ScriptCount)
//		{
//			pScriptUI = m_vecScriptUI[ScriptCount];
//			pScriptUI->SetActive(true);
//		}
//
//		// 없으면 생성
//		else
//		{
//			pScriptUI = new ScriptUI;
//			m_vecScriptUI.push_back(pScriptUI);
//
//			AddChildUI(pScriptUI);
//		}
//
//		pScriptUI->SetTargetScript(scripts[i]);
//		++ScriptCount;
//	}
//#endif

}


void Inspector::SetTargetAsset(Ptr<CAsset> _Asset)
{
	m_TargetAsset = _Asset;

	// Asset 타입에 따라 인스펙터에 출력
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		if (nullptr == m_TargetAsset || i != (UINT)m_TargetAsset->GetAssetType())
		{
			m_arrAssetUI[i]->SetActive(false);
		}
		else
		{
			m_arrAssetUI[i]->SetAsset(m_TargetAsset);
			m_arrAssetUI[i]->SetActive(true);
		}
	}

	// 기존 컴포넌트 UI는 비활성화
	if (nullptr != m_TargetAsset)
	{
		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (nullptr == m_arrComUI[i])
				continue;

			m_arrComUI[i]->SetActive(false);
		}
	}

	m_LevelUI->SetActive(false);


	// AssetUI 는 비활성화
	if (nullptr != _Asset)
		SetTargetObject(nullptr);
}

void Inspector::SetTargetLevel()
{
	SetTargetObject(nullptr);
	SetTargetAsset(nullptr);

	m_LevelUI->SetActive(true);	
}

void Inspector::RenewGameObject()
{
	Outliner* pOut = (Outliner*)CImGuiMgr::GetInst()->FindUI("Outliner");
	pOut->RenewGameObject();
}

void Inspector::Render_Update()
{
	if (CTaskMgr::GetInst()->IsLevelChanged())
	{
		if(m_TargetObject && !m_TargetObject->IsValid())
			SetTargetObject(nullptr);
	}

	if (m_TargetObject)
	{
		// Scale Debug Rendering
		DrawDebugRect(Vector4(1.f, 1.f, 1.f, 0.5f), m_TargetObject->Transform()->GetWorldMat());

		
		// Pivot Debug Rendering
		DrawDebugRect(Vector4(1.f, 0.f, 0.f, 0.5f),
			m_TargetObject->Transform()->GetWorldPos(),
			Vector2(5.f, 5.f),
			Vector3(0.f, 0.f, 0.f),
			false,
			0.f,
			true);

		// 오브젝트 이름
		ImGui::Text("Name");
		ImGui::SameLine(100);
		wstring wname = m_TargetObject->GetName() != L""? m_TargetObject->GetName() : L"Nonamed";
		string name = string(wname.begin(), wname.end());

		char buffer[256] = {};
		strcpy_s(buffer, name.c_str());

		if (ImGui::InputText("##ObjectName", buffer, sizeof(buffer)))
		{
			name = buffer;
			wname = wstring(name.begin(), name.end());
			m_TargetObject->SetName(wname);
			RenewGameObject();
		}


		// 레이어 바꾸기 콤보박스로 구현
		ImGui::Text("Layer");
		ImGui::SameLine(100);
		int LayerNum = m_TargetObject->GetLayerIdx();
		string LayerName = std::to_string(LayerNum) + " : " + LAYER_INDEX_STRING[LayerNum];

		if (ImGui::BeginCombo("##LayerIdx", LayerName.c_str(), 0))
		{
			string itemName;
			for (int i = 0; i < MAX_LAYER; i++)
			{
				bool CurNum = i == LayerNum ? true : false;

				LayerName = std::to_string(i) + " : " + LAYER_INDEX_STRING[i];
				if (ImGui::Selectable(LayerName.c_str(), &CurNum))
				{
					m_TargetObject->DisconnectWithLayer();
					CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(i)->AddObject(m_TargetObject, false);
				}
			}
			ImGui::EndCombo();
		}

		// 오브젝트 활성화
		ImGui::Text("Enabled");
		ImGui::SameLine(100);
		bool enabled = m_TargetObject->IsActive();
		if (ImGui::Checkbox("##ObjEnabled", &enabled))
		{
			m_TargetObject->SetActive(enabled);
		}

		if (KEY_PRESSED(KEY::CTRL) && KEY_TAP(KEY::D))
		{
			CreateObject(m_TargetObject->Clone(), 0, false);
		}
		else if (KEY_TAP(KEY::DEL) && ImGui::IsWindowFocused)
		{
			if (nullptr != m_TargetObject)
			{
				DestroyObject(m_TargetObject);
				SetTargetObject(nullptr);
			}
		}
	}
}



#include "TransformUI.h"
#include "Collider2DUI.h"
#include "CameraUI.h"
#include "Light2DUI.h"
#include "MeshRenderUI.h"
#include "ParticleSystemUI.h"
#include "FlipbookPlayerUI.h"
#include "RigidBody2DUI.h"
#include "StateMachineUI.h"

void Inspector::CreateComponentUI()
{
	// InspectorUI 의 자식UI 추가
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM] = (ComponentUI*)AddChildUI(new TransformUI);

	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D] = (ComponentUI*)AddChildUI(new Collider2DUI);

	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA] = (ComponentUI*)AddChildUI(new CameraUI);

	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D] = (ComponentUI*)AddChildUI(new Light2DUI);

	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER] = (ComponentUI*)AddChildUI(new MeshRenderUI);

	m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLE_SYSTEM] = (ComponentUI*)AddChildUI(new ParticleSystemUI);

	m_arrComUI[(UINT)COMPONENT_TYPE::FLIPBOOKPLAYER] = (ComponentUI*)AddChildUI(new FlipbookPlayerUI);

	m_arrComUI[(UINT)COMPONENT_TYPE::RIGIDBODY2D] = (ComponentUI*)AddChildUI(new RigidBody2DUI);

	m_arrComUI[(UINT)COMPONENT_TYPE::STATEMACHINE] = (ComponentUI*)AddChildUI(new StateMachineUI);
}

#include "MeshUI.h"
#include "MeshDataUI.h"
#include "TextureUI.h"
#include "SoundUI.h"
#include "PrefabUI.h"
#include "FlipbookUI.h"
#include "SpriteUI.h"
#include "MaterialUI.h"
#include "GraphicShaderUI.h"
#include "ComputeShaderUI.h"
#include "FontUI.h"

void Inspector::CreateAssetUI()
{
	m_arrAssetUI[(UINT)ASSET_TYPE::MESH] = new MeshUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::MESH_DATA] = new MeshDataUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::TEXTURE] = new TextureUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::SOUND] = new SoundUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::PREFAB] = new PrefabUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::FLIPBOOK] = new FlipbookUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::SPRITE] = new SpriteUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::MATERIAL] = new MaterialUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::GRAPHIC_SHADER] = new GraphicShaderUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::COMPUTE_SHADER] = new ComputeShaderUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::MTILE] = new TextureUI;
	m_arrAssetUI[(UINT)ASSET_TYPE::FONT] = new FontUI;

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		AddChildUI(m_arrAssetUI[i]);
	}

	m_LevelUI = new LevelInfoUI;
	AddChildUI(m_LevelUI);
}
