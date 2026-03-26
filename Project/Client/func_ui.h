#pragma once

string DrawNameUI(const string& _Name);
bool DrawTitleUI(const string& _Name);
bool DrawTextUI(const string& _Name, string& _Value);
bool DrawInputTextUI(const string& _Name, string& _Value);
bool DrawTextReadOnlyUI(const string& _Name, const string& _Value);
void DrawBoldTextUI(const string& _Name);

bool DrawVector2UI(const string& _Name, Vector2* _Value);
bool DrawNameInt2UI(const string& _Name, const string& _Name1, const string& _Name2, Vector2* _Value, int width = 75);
bool DrawNameFloat2UI(const string& _Name, const string& _Name1, const string& _Name2, Vector2* _Value, int width = 75);

bool DrawVector3UI(const string& _Name, Vector3* _Value);
bool DrawVector4UI(const string& _Name, Vector4* _Value);

bool DrawFloatUI(const string& _Name, float* _Value);
bool DrawIntUI(const string& _Name, int* _Value);
bool DrawUINTUI(const string& _Name, UINT* _Value);
bool DrawCheckUI(const string& _Name, bool* _Value);

bool DrawColorUI(const string& _Name, Vector3* _Value);
bool DrawColorUI(const string& _Name, Vector4* _Value);

wstring SaveFileExplorer(wstring _RelPath = L"", wstring _Filter = L"");
wstring OpenFileExplorer(wstring _RelPath = L"", wstring _Filter = L"");

int GetFirstNumberBeforeUnderscore(const std::wstring& filename);

//class EditorUI;
//typedef void(EditorUI::* EUI_DELEGATE_2)(DWORD_PTR, DWORD_PTR);
//void OpenAssetListUI(ASSET_TYPE _AssetType, EditorUI* _Inst, EUI_DELEGATE_2 _Delgate);