#pragma once
#include "pch.h"
#include "func_ui.h"

#include "imgui/imgui.h"
#include "TreeUI.h"
#include "ListUI.h"
#include "CImGuiMgr.h"
#include <Engine/CTexture.h>
#include <Engine/CAssetMgr.h>

// return string id
string DrawNameUI(const string& _Name)
{
	ImGui::Text(_Name.c_str());
	ImGui::SameLine(100);
	return "##" + _Name;
}

bool DrawTitleUI(const string& _Name)
{
	ImGui::PushID(0);

	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));

	bool bOpen = ImGui::Button(_Name.c_str());

	ImGui::PopStyleColor(3);
	ImGui::PopID();

	return bOpen;
}

bool DrawTextUI(const string& _Name, string& _Value)
{
	const string& Id = DrawNameUI(_Name);
	return ImGui::InputText(Id.c_str(), (char*)_Value.c_str(), _Value.length());
}

bool DrawInputTextUI(const string& _Name, string& _Value)
{
	char buffer[256] = {};
	strcpy_s(buffer, _Value.c_str());

	const string& Id = DrawNameUI(_Name);

	ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 30);
	if (ImGui::InputText(Id.c_str(), buffer, sizeof(buffer)))
	{
		_Value = buffer;
		return true;
	}

	return false;
}

bool DrawTextReadOnlyUI(const string& _Name, const string& _Value)
{
	const string& Id = DrawNameUI(_Name);
	return ImGui::InputText(Id.c_str(), (char*)_Value.c_str(), _Value.length(), ImGuiInputTextFlags_ReadOnly);
}


bool DrawVector2UI(const string& _Name, Vector2* _Value)
{
	const string& Id = DrawNameUI(_Name);
	return ImGui::DragFloat2(Id.c_str(), *_Value);
}

bool DrawNameInt2UI(const string& _Name, const string& _Name1, const string& _Name2, Vector2* _Value, int width)
{
	bool isInput = false;
	int x = _Value->x;
	int y = _Value->y;
	string Id = "##" + _Name + _Name1;
	DrawNameUI(_Name);
	ImGui::SameLine(100);
	ImGui::Text(_Name1.c_str());
	ImGui::SameLine(115);
	ImGui::SetNextItemWidth(width);
	isInput = ImGui::DragInt(Id.c_str(), &x);
	_Value->x = x;

	Id = "##" + _Name + _Name2;
	ImGui::SameLine(120 + width);
	ImGui::Text(_Name2.c_str());
	ImGui::SameLine(135 + width);
	ImGui::SetNextItemWidth(width);
	isInput += ImGui::DragInt(Id.c_str(), &y);
	_Value->y = y;

	return isInput;
}

bool DrawNameFloat2UI(const string& _Name, const string& _Name1, const string& _Name2, Vector2* _Value, int width)
{
	string Id = "##" + _Name + _Name1;
	DrawNameUI(_Name);
	ImGui::SameLine(100);
	ImGui::Text(_Name1.c_str());
	ImGui::SameLine(110);
	ImGui::SetNextItemWidth(75);
	ImGui::DragFloat(Id.c_str(), &(_Value->x));

	Id = "##" + _Name + _Name2;
	ImGui::SameLine(120 + width);
	ImGui::Text(_Name2.c_str());
	ImGui::SameLine(135 + width);
	ImGui::SetNextItemWidth(width);
	return ImGui::DragFloat(Id.c_str(), &(_Value->y));
}

bool DrawVector3UI(const string& _Name, Vector3* _Value)
{
	const string& Id = DrawNameUI(_Name);
	return ImGui::DragFloat3(Id.c_str(), *_Value);
}

bool DrawVector4UI(const string& _Name, Vector4* _Value)
{
	const string& Id = DrawNameUI(_Name);
	return ImGui::DragFloat4(Id.c_str(), *_Value);
}

void DrawBoldTextUI(const string& _Name)
{
	ImGui::Text(_Name.c_str());
	ImGui::SameLine(1);
	ImGui::Text(_Name.c_str());
	ImGui::SameLine(1);
	ImGui::Text(_Name.c_str());
}

bool DrawFloatUI(const string& _Name, float* _Value)
{
	const string& Id = DrawNameUI(_Name);
	return ImGui::DragFloat(Id.c_str(), _Value);
}

bool DrawIntUI(const string& _Name, int* _Value)
{
	const string& Id = DrawNameUI(_Name);
	return ImGui::InputInt(Id.c_str(), _Value);
}

bool DrawUINTUI(const string& _Name, UINT* _Value)
{
	const string& Id = DrawNameUI(_Name);
	bool bResult = ImGui::InputInt(Id.c_str(), (int*)_Value);
	if (bResult)
	{
		if (*_Value < 0)
			*_Value = abs(*(int*)_Value);
	}
	return bResult;
}

bool DrawCheckUI(const string& _Name, bool* _Value)
{
	const string& Id = DrawNameUI(_Name);
	return ImGui::Checkbox(Id.c_str(), (bool*)_Value);
}

bool DrawColorUI(const string& _Name, Vector3* _Value)
{
	const string& Id = DrawNameUI(_Name);
	return ImGui::ColorEdit3(Id.c_str(), (float*)_Value, 0);
}

bool DrawColorUI(const string& _Name, Vector4* _Value)
{
	const string& Id = DrawNameUI(_Name);
	return ImGui::ColorEdit4(Id.c_str(), (float*)_Value, 0);
}


wstring SaveFileExplorer(wstring _RelPath, wstring _Filter)
{
	wstring strContentPath = CPathMgr::GetInst()->GetContentPath();
	strContentPath += _RelPath;

	wstring strFilter = _Filter + L"\0ALL\0*.*";

	// 파일 경로 문자열
	wchar_t szFilePath[255] = {};

	OPENFILENAME Desc = {};

	Desc.lStructSize = sizeof(OPENFILENAME);
	Desc.hwndOwner = nullptr;
	Desc.lpstrFile = szFilePath;
	Desc.nMaxFile = 255;
	Desc.lpstrFilter = strFilter.c_str();
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	Desc.lpstrInitialDir = strContentPath.c_str();

	if (GetSaveFileName(&Desc))
	{
		wstring filePath = Desc.lpstrFile;
		return filePath;
	}

	return L"";
}


wstring OpenFileExplorer(wstring _RelPath, wstring _Filter)
{
	wstring strContentPath = CPathMgr::GetInst()->GetContentPath();
	strContentPath += _RelPath;

	wstring strFilter = _Filter + L"\0ALL\0*.*";

	// 파일 경로 문자열
	wchar_t szFilePath[255] = {};

	OPENFILENAME Desc = {};

	Desc.lStructSize = sizeof(OPENFILENAME);
	Desc.hwndOwner = nullptr;
	Desc.lpstrFile = szFilePath;
	Desc.nMaxFile = 255;
	Desc.lpstrFilter = strFilter.c_str();
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	Desc.lpstrInitialDir = strContentPath.c_str();

	if (GetOpenFileName(&Desc))
	{
		wstring filePath = Desc.lpstrFile;
		return filePath;
	}

	return L"";
}

// 메이플 전용 frame 정렬 함수
int GetFirstNumberBeforeUnderscore(const std::wstring& filename)
{
	std::wstring stem = std::filesystem::path(filename).stem().wstring();

	size_t underscorePos = stem.find(L'_');
	if (underscorePos == std::wstring::npos)
		return 0;

	return std::stoi(stem.substr(0, underscorePos));
}

void OpenAssetListUI(ASSET_TYPE _AssetType, EditorUI* _Inst, EUI_DELEGATE_2 _Delgate)
{
	// ListUI 를 활성화 시키기
	ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
	pListUI->SetName("Asset");
	pListUI->SetActive(true);

	// ListUI 에 넣어줄 문자열 정보 가져오기
	pListUI->AddItem("None");

	vector<wstring> vecTexNames;
	CAssetMgr::GetInst()->GetAssetNames(_AssetType, vecTexNames);
	pListUI->AddItem(vecTexNames);

	// 더블 클릭 시 호출시킬 함수 등록
	pListUI->AddDynamicDoubleClicked(_Inst, _Delgate);
}
