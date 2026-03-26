#include "pch.h"
#include "ParamUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CComponent.h>

#include "CImGuiMgr.h"
#include "imgui/imgui.h"
#include "TreeUI.h"
#include "ListUI.h"

UINT ParamUI::g_ParamID = 0;

bool ParamUI::Param_Bool(const string& _Desc, bool* _Data)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine(120);

	char szID[255] = {};
	sprintf_s(szID, "##Param_Bool_%d", g_ParamID++);

	if (ImGui::Checkbox(szID, _Data))
	{
		return true;
	}

	return false;
}

bool ParamUI::Param_Int(const string& _Desc, int* _Data, bool _Drag)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine(120);

	char szID[255] = {};
	sprintf_s(szID, "##Param_Int_%d", g_ParamID++);

	if (_Drag)
	{
		if (ImGui::DragInt(szID, _Data, 1.f))
		{
			return true;
		}
	}
	else
	{
		if (ImGui::InputInt(szID, _Data, 0))
		{
			return true;
		}
	}

	return false;
}

bool ParamUI::Param_Float(const string& _Desc, float* _Data, bool _Drag)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine(120);

	char szID[255] = {};
	sprintf_s(szID, "##Param_Float_%d", g_ParamID++);


	if (_Drag)
	{
		if (ImGui::DragFloat(szID, _Data, 1.f))
		{
			return true;
		}
	}
	else
	{
		if (ImGui::InputFloat(szID, _Data, 0))
		{
			return true;
		}
	}

	return false;
}

bool ParamUI::Param_Vec2(const string& _Desc, Vector2* _Data, bool _Drag)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine(120);

	char szID[255] = {};
	sprintf_s(szID, "##Param_Vec2_%d", g_ParamID++);


	float arr[2] = { _Data->x, _Data->y };

	if (_Drag)
	{
		if (ImGui::DragFloat2(szID, arr, 1.f))
		{
			_Data->x = arr[0];
			_Data->y = arr[1];
			return true;
		}
	}
	else
	{
		if (ImGui::InputFloat2(szID, arr, 0))
		{
			_Data->x = arr[0];
			_Data->y = arr[1];
			return true;
		}
	}

	return false;
}

bool ParamUI::Param_Vec3(const string& _Desc, Vector3* _Data, bool _Drag)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine(120);

	char szID[255] = {};
	sprintf_s(szID, "##Param_Vec3_%d", g_ParamID++);


	float arr[3] = { _Data->x, _Data->y, _Data->z };

	if (_Drag)
	{
		if (ImGui::DragFloat3(szID, arr, 1.f))
		{
			_Data->x = arr[0];
			_Data->y = arr[1];
			_Data->z = arr[2];
			return true;
		}
	}
	else
	{
		if (ImGui::InputFloat3(szID, arr, 0))
		{
			_Data->x = arr[0];
			_Data->y = arr[1];
			_Data->z = arr[2];
			return true;
		}
	}

	return false;
}

bool ParamUI::Param_Vec4(const string& _Desc, Vector4* _Data, bool _Drag)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine(120);

	char szID[255] = {};
	sprintf_s(szID, "##Param_Vec4_%d", g_ParamID++);


	float arr[4] = { _Data->x, _Data->y, _Data->z, _Data->w };

	if (_Drag)
	{
		if (ImGui::DragFloat4(szID, arr, 1.f))
		{
			*_Data = Vector4(arr[0], arr[1], arr[2], arr[3]);
			return true;
		}
	}
	else
	{
		if (ImGui::InputFloat(szID, arr, 0))
		{
			*_Data = Vector4(arr[0], arr[1], arr[2], arr[3]);
			return true;
		}
	}

	return false;
}

bool ParamUI::Param_Tex(const string& _Desc, Ptr<CTexture>& _Tex
	, EditorUI* _Inst, EUI_DELEGATE_2 _MemFunc)
{
	char szID[255] = {};

	ImGui::Text(_Desc.c_str());

	ImVec2 uv_min = ImVec2(0.0f, 0.0f);
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);
	ImVec4 tint_col = ImVec4(1.f, 1.f, 1.f, 1.f);
	ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);

	if (nullptr == _Tex)
		ImGui::Image(nullptr, ImVec2(100.f, 100.f), uv_min, uv_max, tint_col, border_col);
	else
		ImGui::Image(_Tex->GetSRV().Get(), ImVec2(100.f, 100.f), uv_min, uv_max, tint_col, border_col);

	if (ImGui::BeginDragDropTarget())
	{
		if (ImGui::AcceptDragDropPayload("ContentUI"))
		{
			const ImGuiPayload* pPayload = ImGui::GetDragDropPayload();
			TreeNode* pNode = *((TreeNode**)pPayload->Data);
			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (pAsset->GetAssetType() == ASSET_TYPE::TEXTURE)
			{
				_Tex = ((CTexture*)pAsset.Get());
				ImGui::SetWindowFocus(nullptr);
				return true;
			}
		}

		ImGui::EndDragDropTarget();
	}


	if (_Inst && _MemFunc)
	{
		ImGui::SameLine();

		sprintf_s(szID, "##TexBtn_%d", g_ParamID++);
		if (ImGui::Button(szID, ImVec2(18.f, 18.f)))
		{
			// ListUI 를 활성화 시키기
			ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
			pListUI->SetName("Texture");
			pListUI->SetActive(true);

			// ListUI 에 넣어줄 문자열 정보 가져오기
			pListUI->AddItem("None");

			vector<wstring> vecAssetNames;
			CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecAssetNames);
			pListUI->AddItem(vecAssetNames);

			// 더블 클릭 시 호출시킬 함수 등록
			pListUI->AddDynamicDoubleClicked(_Inst, (EUI_DELEGATE_2)_MemFunc);

			return true;
		}
	}

	return false;
}

bool ParamUI::Param_Prefab(const string& _Desc, Ptr<CPrefab>& _Prefab, EditorUI* _Inst, EUI_DELEGATE_2 _MemFunc)
{
	char szID[255] = {};

	ImGui::Text(_Desc.c_str());
	ImGui::SameLine(100);

	string strKey;
	if (nullptr != _Prefab)
		strKey = string(_Prefab->GetKey().begin(), _Prefab->GetKey().end());
	else
		strKey = "NONE";

	sprintf_s(szID, "##PrefabName_%d", g_ParamID++);
	ImGui::InputText(szID, (char*)strKey.c_str(), strKey.length(), ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		if (ImGui::AcceptDragDropPayload("ContentUI"))
		{
			const ImGuiPayload* pPayload = ImGui::GetDragDropPayload();
			TreeNode* pNode = *((TreeNode**)pPayload->Data);
			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (pAsset->GetAssetType() == ASSET_TYPE::PREFAB)
			{
				_Prefab = ((CPrefab*)pAsset.Get());
				ImGui::SetWindowFocus(nullptr);
				return true;
			}
		}

		ImGui::EndDragDropTarget();
	}

	if (_Inst && _MemFunc)
	{
		ImGui::SameLine();

		sprintf_s(szID, "##PrefabBtn_%d", g_ParamID++);
		if (ImGui::Button(szID, ImVec2(18.f, 18.f)))
		{
			// ListUI 를 활성화 시키기
			ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
			pListUI->SetName("Prefab");
			pListUI->SetActive(true);

			// ListUI 에 넣어줄 문자열 정보 가져오기
			pListUI->AddItem("None");

			vector<wstring> vecAssetNames;
			CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::PREFAB, vecAssetNames);
			pListUI->AddItem(vecAssetNames);

			// 더블 클릭 시 호출시킬 함수 등록
			pListUI->AddDynamicDoubleClicked(_Inst, (EUI_DELEGATE_2)_MemFunc);

			return true;
		}
	}

	return false;
}

bool ParamUI::Param_GameObject(const string& _Desc, CGameObject*& _Obj)
{
	char szID[255] = {};

	ImGui::Text(_Desc.c_str());
	ImGui::SameLine(100);

	string strKey;
	if (nullptr != _Obj)
		strKey = string(_Obj->GetName().begin(), _Obj->GetName().end());
	else
		strKey = "NONE";

	sprintf_s(szID, "##ObjectName_%d", g_ParamID++);
	ImGui::InputText(szID, (char*)strKey.c_str(), strKey.length(), ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		if (ImGui::AcceptDragDropPayload("Outliner"))
		{
			const ImGuiPayload* pPayload = ImGui::GetDragDropPayload();
			TreeNode* pNode = *((TreeNode**)pPayload->Data);
			CGameObject* pObj = (CGameObject*)pNode->GetData();
			if (pObj != nullptr)
			{
				_Obj = pObj;
				ImGui::SetWindowFocus(nullptr);
				return true;
			}
		}

		ImGui::EndDragDropTarget();
	}

	return false;
}

bool ParamUI::Param_Mtrl(const string& _Desc, Ptr<CMaterial>& _Mtrl)
{
	char szID[255] = {};

	ImGui::Text(_Desc.c_str());
	ImGui::SameLine(100);

	string strKey;
	if (nullptr != _Mtrl)
		strKey = string(_Mtrl->GetKey().begin(), _Mtrl->GetKey().end());
	else
		strKey = "NONE";

	sprintf_s(szID, "##MtrlName_%d", g_ParamID++);
	ImGui::InputText(szID, (char*)strKey.c_str(), strKey.length(), ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		if (ImGui::AcceptDragDropPayload("ContentUI"))
		{
			const ImGuiPayload* pPayload = ImGui::GetDragDropPayload();
			TreeNode* pNode = *((TreeNode**)pPayload->Data);
			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (pAsset->GetAssetType() == ASSET_TYPE::MATERIAL)
			{
				_Mtrl = ((CMaterial*)pAsset.Get());
				ImGui::SetWindowFocus(nullptr);
				return true;
			}
		}

		ImGui::EndDragDropTarget();
	}

	return false;
}

bool ParamUI::Param_String(const string& _Desc, wstring* _Str)
{
	char szID[255] = {};

	string str;
	str.resize(255);
	str = string((*_Str).begin(), (*_Str).end());

	ImGui::Text(_Desc.c_str());
	ImGui::SameLine(100);


	wstring wname = *_Str;
	string name = string(wname.begin(), wname.end());

	char buffer[256] = {};
	strcpy_s(buffer, name.c_str());

	sprintf_s(szID, "##StrName_%d", g_ParamID++);
	if (ImGui::InputText(szID, buffer, sizeof(buffer)))
	{
		name = buffer;
		wname = wstring(name.begin(), name.end());
		(*_Str) = wname;
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (ImGui::AcceptDragDropPayload("LevelUI"))
		{
			const ImGuiPayload* pPayload = ImGui::GetDragDropPayload();
			TreeNode* pNode = *((TreeNode**)pPayload->Data);

			string name = pNode->GetName();
			(*_Str) = wstring(name.begin(), name.end());
		}

		ImGui::EndDragDropTarget();
	}

	return false;
}

bool ParamUI::Param_Func(const string& _Desc, CScript* _Inst, SCRIPT_DELEGATE _MemFunc)
{
	if (_Inst && _MemFunc)
	{
		if (ImGui::Button(_Desc.c_str()))
		{
			(_Inst->*_MemFunc)();
		}
	}

	return false;
}

bool ParamUI::Param_Bool32(const string& _Desc, UINT* _State)
{
	ImGui::Text(_Desc.c_str());

	static bool bState;
	for (int i = 0; i < 32; ++i)
	{
		ImGui::SameLine(100 + 22 * i);

		char szID[255] = {};
		sprintf_s(szID, "##Param_Bool32_%d", g_ParamID++);

		bState = (*_State) & (1 << i);
		if (ImGui::Checkbox(szID, &bState))
		{
			if (bState)
			{
				*_State &= ~(1 << i);
			}
			else
			{
				*_State |= (1 << i);
			}
		}
	}

	return false;
}


