#include "pch.h"
#include "SpriteEditorUI.h"
#include "func_ui.h"
#include "CImGuiMgr.h"
#include "SE_Sub.h"
#include "SE_Detail.h"
#include "SE_AtlasView.h"
#include <Engine/CTexture.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CPathMgr.h>
#include <Engine/CSprite.h>
#include <Engine/CTexture.h>

SpriteEditorUI::SpriteEditorUI()
	: EditorUI("SpriteEditor")
	, m_ScaleFactor(100.f)
	, m_CurAtlas(nullptr)
	, m_CurSprite(nullptr)
	, m_SliceInput(0.f, 0.f)
{
	UseMenuBar(false);
}

SpriteEditorUI::~SpriteEditorUI()
{
}


void SpriteEditorUI::Render()
{
	if (IsActive())
	{
		ImGui::SetNextWindowPos(ImVec2(100.f, 100.f), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(1400, 600), ImGuiCond_FirstUseEver);
	}
	EditorUI::Render();
}

void SpriteEditorUI::Render_Update()
{
	// 메뉴바
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Load Sprite")) {}
			if (ImGui::MenuItem("Save Sprite")) {}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// 좌우 분할 레이아웃
	ImGui::Columns(2);
	ImVec2 vecWinSize = ImGui::GetWindowSize();
	ImGui::SetColumnWidth(0, vecWinSize.x * 0.7f); // 왼쪽 80%

	if (nullptr == m_CurAtlas)
		m_CurAtlas = CAssetMgr::GetInst()->FindAsset<CTexture>(L"LinkAtlas");

	// 왼쪽 영역 (텍스처 뷰어)
	{
		ImGui::BeginChild("Texture Viewer", ImVec2(0, 0), true,
			ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		ImVec2 winMin = ImGui::GetWindowPos();
		ImVec2 winMax = ImVec2(winMin.x + ImGui::GetWindowSize().x, winMin.y + ImGui::GetWindowSize().y);

		// 화면 확대
		if (ImGui::IsMouseHoveringRect(winMin, winMax))
		{
			float wheelDelta = ImGui::GetIO().MouseWheel;
			if (wheelDelta > 0.f)
			{
				m_ScaleFactor += DT * 10000.f;
			}
			if (wheelDelta < 0.f)
			{
				m_ScaleFactor -= DT * 10000.f;
			}
		}

		// 화면 이동
		if (KEY_PRESSED(KEY::MBTN))
		{
			m_AtlasPos += CKeyMgr::GetInst()->GetMouseDir();
		}

		// 아틀라스 불러오기
		Ptr<CTexture> pTex = CAssetMgr::GetInst()->Load<CTexture>(L"JumpAtlas", L"");
		ImVec2 uv_min = ImVec2(0.0f, 0.0f);
		ImVec2 uv_max = ImVec2(1.0f, 1.0f);
		ImVec4 tint_col = ImVec4(0.f, 1.f, 0.f, 1.f);
		ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
		float aspect_ratio = 1.f;
		float width = 50.f, height = 50.f;
		ImTextureID tex = nullptr;
		if (nullptr != m_CurAtlas)
		{
			aspect_ratio = m_CurAtlas->GetAspectRatio();
			tex = m_CurAtlas->GetSRV().Get();
		}
		ImGui::SetCursorPos(ImVec2(m_AtlasPos.x, m_AtlasPos.y));

		// 윈도우상 아틀라스 시작 위치 구하기
		Vector2 vAtlasPos = Vector2(winMin.x + m_AtlasPos.x, winMin.y + m_AtlasPos.y);
		// 윈도우상 아틀라스 크기 구하기
		Vector2 vAtlasScale = Vector2(m_ScaleFactor, m_ScaleFactor / aspect_ratio);
		// 마우스 좌표 가져오기
		Vector2 mousePos = Vector2(ImGui::GetMousePos().x, ImGui::GetMousePos().y);

		ImGui::Image(tex, ImVec2(vAtlasScale.x, vAtlasScale.y), uv_min, uv_max, ImVec4(1.f, 1.f, 1.f, 1.f), border_col);


		// Sprite 출력
		bool isHover = false;
		vector<CSprite*>& vecSprite = m_CurAtlas->GetSpritesForEdit();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		vector<CSprite*>::iterator iter = vecSprite.begin();
		for (; iter != vecSprite.end();)
		{
			CSprite* pSprite = *iter;
			Vector2 vSpritePos = vAtlasScale * pSprite->GetLeftTop();
			Vector2 vSpriteScale = vAtlasScale * pSprite->GetSlice();
			Vector2 vLT = vAtlasPos + vSpritePos;
			Vector2 vRB = vLT + vSpriteScale;
			Vector2 vCenter = vLT + (Vector2)(vSpriteScale / 2);


			if ((vLT.x < mousePos.x && vLT.y < mousePos.y && mousePos.x < vRB.x && mousePos.y < vRB.y))
			{
				isHover = true;

				// 스프라이트 선택
				if (KEY_TAP(KEY::LBTN))
					m_CurSprite = pSprite;

				// 스프라이트 이동
				if (KEY_PRESSED(KEY::LBTN) && m_CurSprite == pSprite && !m_OnCreate)
				{
					Vector2 vMovePx = CSprite::GetPixelFromUV(pSprite->GetLeftTop(), m_CurAtlas->GetResolution())
									+ CKeyMgr::GetInst()->GetMouseDir() / vAtlasScale * m_CurAtlas->GetResolution();	// 이동량의 픽셀상 좌표 구하기
					m_CurSprite->SetLeftTop(vMovePx);
				}
				draw_list->AddRect(ImVec2(vLT.x, vLT.y), ImVec2(vRB.x, vRB.y), IM_COL32(255, 0, 0, 255), 0.0f, 0, 1.f);
			}
			else if (m_CurSprite == pSprite)
			{
				draw_list->AddRect(ImVec2(vLT.x, vLT.y), ImVec2(vRB.x, vRB.y), IM_COL32(255, 0, 0, 255), 0.0f, 0, 1.f);
			}
			else
			{
				draw_list->AddRect(ImVec2(vLT.x, vLT.y), ImVec2(vRB.x, vRB.y), IM_COL32(0, 255, 0, 255), 0.0f, 0, 1.f);
			}


			draw_list->AddCircle(ImVec2(vCenter.x, vCenter.y), 2.f, IM_COL32(0, 255, 255, 255), 0.0f, 1.5f);

			++iter;
		}

		// 스프라이트 생성
		// 텍스처상 스프라이트 픽셀 좌표 구하기
		if (KEY_TAP(KEY::LBTN) && !isHover 
			&& mousePos.x > vAtlasPos.x && mousePos.y > vAtlasPos.y
			&& mousePos.x < vAtlasPos.x + vAtlasScale.x && mousePos.y < vAtlasPos.y + vAtlasScale.y)
		{
			m_OnCreate = true;

			CreateSpriteByEditor(mousePos - vAtlasPos, Vector2(0.f, 0.f));
		}
		if (m_OnCreate)
		{
			Vector2 vScalePixel = (mousePos - vAtlasPos - (m_CurSprite->GetLeftTop() * vAtlasScale)) 
								/ vAtlasScale * Vector2(m_CurAtlas->GetWidth(), m_CurAtlas->GetHeight());
			m_CurSprite->SetSlice(vScalePixel);
		}
		if (KEY_RELEASED(KEY::LBTN) && m_OnCreate)
		{
			m_OnCreate = false;
			Vector2 vSpriteScale = vAtlasScale * m_CurSprite->GetSlice();
			if (vSpriteScale.x < 10.f || vSpriteScale.y < 10.f)
			{
				DeleteSprite(m_CurSprite);
			}
		}

		// 스프라이트 삭제
		if (KEY_RELEASED(KEY::DEL) && nullptr != m_CurSprite)
		{
			DeleteSprite(m_CurSprite);
		}



		ImGui::EndChild();
	}

	ImGui::NextColumn();

	// 오른쪽 영역 (설정 패널)
	{
		const char* pivots[] = { "Center", "Left" , "Right" , "Top" , "TopLeft" , "TopRight" , "Bottom" , "BottomLeft" , "BottomRight", "Custom" };

		// Atlas Info
		{
			ImGui::BeginChild("Atlas Info", ImVec2(0, 200));
			ImVec2 backSize = ImGui::GetWindowSize();
			int itemWidth = (backSize.x - 150) / 2;

			DrawBoldTextUI("Atlas Info");
			if (nullptr != m_CurAtlas)
			{
				// name
				wstring wname = m_CurAtlas->GetName();
				string name = string(wname.begin(), wname.end());
				DrawTextUI("Name", name);

				// size
				string atlaswh = "Width : " + std::to_string(m_CurAtlas->GetWidth()) + "px  Height : " + std::to_string(m_CurAtlas->GetHeight()) + "px";
				ImGui::Text(atlaswh.c_str());
				ImGui::Text("");

				// button
				ImGui::SameLine(ImGui::GetColumnWidth() - 70 * 2);
				if (ImGui::Button("Load", ImVec2(60, 0))) {}

				ImGui::SameLine(ImGui::GetColumnWidth() - 70);
				if (ImGui::Button("Save", ImVec2(60, 0))) { Save(); }
			}
			// atlas list
			if (ImGui::BeginListBox("##TexList_textureditor", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
			{
				vector<wstring> vecNames;
				CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecNames);
				for (int n = 0; n < vecNames.size(); n++)
				{

					string items = string(vecNames[n].begin(), vecNames[n].end()) + "##textureditor";
					bool is_selected = (m_CurAtlasNum == n);
					if (ImGui::Selectable(items.c_str(), is_selected, 0))
					{
						m_CurAtlasNum = n;
						m_CurAtlas = CAssetMgr::GetInst()->FindAsset<CTexture>(vecNames[n]);
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndListBox();
			}

			ImGui::EndChild();
		}

		// Slice
		{
			ImGui::BeginChild("Slice Settings", ImVec2(0, 170));
			{
				DrawBoldTextUI("Slice");

				ImVec2 backSize = ImGui::GetWindowSize();
				int itemWidth = (ImGui::GetColumnWidth() - 150) / 2;

				const char* types[] = { "Grid By Cell Size" , "Grid By Cell Count" };
				static int typeIndex = 0;

				// slice type
				DrawNameUI("Type");
				ImGui::Combo("##Type", &typeIndex, types, IM_ARRAYSIZE(types));

				// slice by size
				if (typeIndex == 0)
				{
					//Vector2 vPixelSize = {}, vOffset = {}, vPadding = {};
					DrawNameInt2UI("Pixel Size", "X", "Y", &m_SliceInput, itemWidth);
					//DrawNameInt2UI("Offset", "X", "Y", &vOffset, itemWidth);
					//DrawNameInt2UI("Padding", "X", "Y", &vPadding, itemWidth);

					//static int pivotIndex = 0;

					//DrawNameUI("Pivot");
					//ImGui::Combo("##Pivot", &pivotIndex, pivots, IM_ARRAYSIZE(pivots));
					//if (pivotIndex == 9)
					//{
					//	Vector2 vCustomPivot = {};
					//	DrawVector2UI("Custom Pivot", &vCustomPivot);
					//}
				}

				// slice by count
				else if (typeIndex == 1)
				{
					//Vector2 vCount = {}, vOffset = {}, vPadding = {};
					DrawNameInt2UI("Pixel Count", "X", "Y", &m_SliceInput, itemWidth);
					//DrawNameInt2UI("Offset", "X", "Y", &vOffset, itemWidth);
					//DrawNameInt2UI("Padding", "X", "Y", &vPadding, itemWidth);

					//static int pivotIndex = 0;

					//DrawNameUI("Pivot");
					//ImGui::Combo("##Pivot", &pivotIndex, pivots, IM_ARRAYSIZE(pivots));
					//if (pivotIndex == 9)
					//{
					//	Vector2 vCustomPivot = {};
					//	DrawVector2UI("Custom Pivot", &vCustomPivot);
					//}
				}

				if (ImGui::Button("Slice", ImVec2(-1, 0))) 
				{
					if(typeIndex == 0)
						SliceBySize(m_SliceInput);
					else if(typeIndex == 1)
						SliceByCount(m_SliceInput);
				}
			}
			ImGui::EndChild();
		}

		ImGui::Spacing();

		// Sprite 설정
		{
			if (nullptr != m_CurSprite)
			{
				ImGui::BeginChild("Sprite Settings");
				{
					ImVec2 backSize = ImGui::GetWindowSize();
					int itemWidth = (ImGui::GetColumnWidth() - 130) / 2;
					DrawBoldTextUI("Sprite");

					wstring spriteWName = m_CurSprite->GetName();
					string spriteName = string(spriteWName.begin(), spriteWName.end());
					DrawTextUI("Name", spriteName);

					Vector2 vPos = m_CurSprite->GetLeftTop() * m_CurAtlas->GetResolution() , vScale = m_CurSprite->GetSlice() * m_CurAtlas->GetResolution();

					DrawNameInt2UI("Position", "X", "Y", &vPos, itemWidth);
					DrawNameInt2UI("", "W", "H", &vScale, itemWidth);
					//Vector2 vBorderLT = {};
					//Vector2 vBorderRB = {};
					//DrawNameInt2UI("Border", "L", "T", &vBorderLT, itemWidth);
					//DrawNameInt2UI("", "R", "B", &vBorderRB, itemWidth);


					//int pivotModeIndex = 9;
					//DrawNameUI("Pivot");
					//ImGui::Combo("##Pivot", &pivotModeIndex, pivots, IM_ARRAYSIZE(pivots));

					//if (pivotModeIndex == 9)
					//{
					//	Vector2 vCustomPivot = {};
					//	DrawNameInt2UI("Custom Pivot", "X", "Y", &vCustomPivot, itemWidth);
					//}
				}
				ImGui::EndChild();
			}
			else
			{
				ImGui::Text("Select a Sprite");
			}
		}
	}

}

void SpriteEditorUI::Load(Ptr<CTexture> _Tex)
{
	// 멤버 초기화

	// 렌더링 용 텍스쳐 포인터 가져오기

	// 스프라이트 정보 복사
	// Copy vector<Sprite>


}

void SpriteEditorUI::Save()
{
	if (nullptr == m_CurAtlas)
		return;

	wstring path = CPathMgr::GetInst()->GetContentPath() + L"Sprite\\";


	const auto& vecSprite = m_CurAtlas->GetSprites();
	for (auto& sprite : m_CurAtlas->GetSprites())
	{
		wstring fullPath = path + sprite->GetKey() + L".sprite";
		sprite->Save(fullPath);
	}
}


void SpriteEditorUI::SliceByCount(Vector2 _Count)
{
	Vector2 vAtlasScale = m_CurAtlas->GetResolution();

	if (_Count.x < 1 || _Count.y < 1 || _Count.x > vAtlasScale.x / 3 || _Count.y > vAtlasScale.y / 3)
		return;

	// 기존 스프라이트 삭제
	auto& vecSprite = m_CurAtlas->GetSpritesForEdit();
	while (vecSprite.size() > 0)
	{
		DeleteSprite(vecSprite[0]);
	}

	// 새로운 스프라이트 생성
	Vector2 vSpriteScale = vAtlasScale / _Count;
	for (int cy = 0; cy < _Count.y; ++cy)
	{
		for (int cx = 0; cx < _Count.x; ++cx)
		{
			CreateSpriteByPixel(Vector2(vSpriteScale.x * cx, vSpriteScale.y * cy), vSpriteScale);
		}
	}
}

void SpriteEditorUI::SliceBySize(Vector2 _Size)
{
	Vector2 vAtlasScale = m_CurAtlas->GetResolution();

	if (_Size.x < 1 || _Size.y < 1 || _Size.x > vAtlasScale.x || _Size.y > vAtlasScale.y)
		return;

	// 기존 스프라이트 삭제
	auto& vecSprite = m_CurAtlas->GetSpritesForEdit();
	while (vecSprite.size() > 0)
	{
		DeleteSprite(vecSprite[0]);
	}

	// 새로운 스프라이트 생성
	for (int cy = 0; cy < (int)vAtlasScale.y / (int)_Size.y; ++cy)
	{
		for (int cx = 0; cx < (int)vAtlasScale.x / (int)_Size.x; ++cx)
		{
			CreateSpriteByPixel(Vector2(_Size.x * cx, _Size.y * cy), _Size);
		}
	}
}

void SpriteEditorUI::CreateSpriteByEditor(Vector2 _WinPos, Vector2 _WinScale)
{
	Vector2 vAtlasScale = Vector2(m_ScaleFactor, m_ScaleFactor / m_CurAtlas->GetAspectRatio());
	Vector2 vLeftTopPixel = _WinPos / vAtlasScale * m_CurAtlas->GetResolution();
	Vector2 vScalePixel = _WinScale / vAtlasScale * m_CurAtlas->GetResolution();
	CreateSpriteByPixel(vLeftTopPixel, _WinScale);
}

void SpriteEditorUI::CreateSpriteByPixel(Vector2 _Pos, Vector2 _Scale)
{
	Vector2 vAtlasScale = Vector2(m_ScaleFactor, m_ScaleFactor / m_CurAtlas->GetAspectRatio());

	// 스프라이트 생성
	m_CurSprite = new CSprite;

	// 텍스처에 등록
	m_CurSprite->SetAtlasTexture(m_CurAtlas);

	m_CurSprite->SetLeftTop(_Pos);
	m_CurSprite->SetSlice(_Scale);
	m_CurSprite->SetBackground(Vector2(m_CurAtlas->GetWidth(), m_CurAtlas->GetHeight()));


	// 에셋매니저에 등록
	const vector<CSprite*>& vecSprite = m_CurAtlas->GetSprites();
	wstring strKey;
	if(vecSprite.size() > 1)
		strKey = vecSprite[vecSprite.size() - 2]->GetKey();
	size_t pos = strKey.find_last_of(L'_');
	int lastNum = vecSprite.size();
	if (pos != wstring::npos) {
		wstring numStr = strKey.substr(pos + 1);
		lastNum = _wtoi(numStr.c_str());
	}
	wstring key = m_CurAtlas->GetName() + L"_" + std::to_wstring(++lastNum);

	CAssetMgr::GetInst()->AddAsset(key, m_CurSprite);

}

void SpriteEditorUI::DeleteSprite(Ptr<CSprite> _DelSprite)
{
	// 스프라이트 삭제
	vector<CSprite*>& vecSprite = m_CurAtlas->GetSpritesForEdit();

	// 텍스처에서 삭제
	auto iter = find(vecSprite.begin(), vecSprite.end(), _DelSprite);
	if(iter != vecSprite.end())
		vecSprite.erase(iter);

	// 에셋매니저에서 삭제
	CAssetMgr::GetInst()->DeleteAsset<CSprite>(_DelSprite->GetKey());

	// 애니메이션에서 혼자 참조하고 있는 경우도 고려할 것
	m_CurSprite = nullptr;
}
