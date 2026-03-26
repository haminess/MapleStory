#include "pch.h"
#include "SE_AtlasView.h"

#include <Engine/CTimeMgr.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CSprite.h>


SE_AtlasView::SE_AtlasView()
	: SE_Sub("SE_AtlasView")
	, m_WidthSize(200.f)
	, m_WheelScale(1.f)
	, m_ScaleFactor(1.f)
	, m_Ratio(0.f)
{
	SetWndFlag(ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	SetMove(false);
	UseMenuBar(false);
}

SE_AtlasView::~SE_AtlasView()
{
}


void SE_AtlasView::Render_Update()
{
	Ptr<CTexture> pAtlas = GetOwner()->GetAtlas();
	if (nullptr == pAtlas)
		return;

	if(ImGui::IsWindowFocused())
		ControlView(m_CursorPos, m_WheelScale);

	// 이미지	
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);

	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 border_col = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);

	// 실제 해상도 대비 출력 Image 의 비율
	m_WidthSize = pAtlas->GetWidth();
	m_Ratio = (m_WidthSize * m_WheelScale) / pAtlas->GetWidth();

	ImGui::SetCursorPos(ImVec2(m_CursorPos.x, m_CursorPos.y));
	ImGui::Image(pAtlas->GetSRV().Get(), ImVec2((m_WidthSize * m_WheelScale), pAtlas->GetHeight() * m_Ratio)
		, uv_min, uv_max, tint_col, border_col);

	// Image 위젯 좌상단 좌표
	m_AtlasPos = ImGui::GetItemRectMin();

	// 현재 마우스 위치
	m_MousePos = ImGui::GetMousePos();

	// 스프라이트 정보 그리기
	Render_Sprite();
}

void SE_AtlasView::AutoSliceOnce(Vector2 _PixelPos)
{
	// 등록아이디 검사용 Set Clear
	m_PixelID.clear();

	CalcSpriteSize(_PixelPos);
}

void SE_AtlasView::AutoSliceAll()
{
	// 등록아이디 검사용 Set Clear
	m_PixelID.clear();

	for (int y = 0; y < GetOwner()->GetAtlas()->GetHeight(); ++y)
	{
		for (int x = 0; x < GetOwner()->GetAtlas()->GetWidth(); ++x)
		{
			if (CalcSpriteSize(Vector2(x, y)))
			{
				tSlice newSlice = { m_MouseLT , m_MouseRB };
				Vector2 vPos = Vector2(m_MouseLT.x - m_AtlasPos.x, m_MouseLT.y - m_AtlasPos.y);
				Vector2 vSize = Vector2(m_MouseRB.x - m_MouseLT.x, m_MouseRB.y - m_MouseLT.y);
				GetOwner()->CreateSprite(vPos, vSize);
			}
		}
	}
}



void SE_AtlasView::CreateDrag()
{
	// 마우스가 이미지 영역 안에 있는지 체크
	bool isInImageArea = (m_MousePos.x > m_AtlasPos.x && m_MousePos.y > m_AtlasPos.y
		&& m_MousePos.x < m_AtlasPos.x + m_WidthSize * m_WheelScale
		&& m_MousePos.y < m_AtlasPos.y + GetOwner()->GetAtlas()->GetHeight() * m_Ratio);


	// 드래그 시작
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && isInImageArea)
	{
		m_OnCreate = true;

		// 시작 위치 저장
		ImVec2 vDiff = ImVec2(m_MousePos.x - m_AtlasPos.x, m_MousePos.y - m_AtlasPos.y);
		m_MouseLT = ImVec2(vDiff.x / m_Ratio, vDiff.y / m_Ratio);
		m_MouseRB = m_MouseLT;  // 초기에는 LT와 RB가 같은 위치
	}
}



void SE_AtlasView::OnDrag()
{
	// 드래그 중
	if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && m_OnCreate)
	{
		// 현재 위치로 RB 업데이트
		ImVec2 vDiff = ImVec2(m_MousePos.x - m_AtlasPos.x, m_MousePos.y - m_AtlasPos.y);
		m_MouseRB = ImVec2(vDiff.x / m_Ratio, vDiff.y / m_Ratio);

		ImVec2 vMouseLT = ImVec2(m_MouseLT.x * m_Ratio + m_AtlasPos.x, m_MouseLT.y * m_Ratio + m_AtlasPos.y);
		ImVec2 vMouseRB = ImVec2(m_MouseRB.x * m_Ratio + m_AtlasPos.x, m_MouseRB.y * m_Ratio + m_AtlasPos.y);

		ImGui::GetWindowDrawList()->AddRect(vMouseLT, vMouseRB
			, ImGui::GetColorU32(ImVec4(0.f, 1.f, 0.f, 1.f)), 0.f, 0.f, 1.f);
	}

	// 드래그 끝
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && m_OnCreate)
	{
		m_OnCreate = false;

		// 드래그 영역이 최소 크기 이상인 경우에만 스프라이트 생성
		float width = abs(m_MouseRB.x - m_MouseLT.x);
		float height = abs(m_MouseRB.y - m_MouseLT.y);

		if (width > 10.f && height > 10.f)  // 최소 크기 체크
		{
			// 좌표 정규화 (LT가 항상 왼쪽 위에 있도록)
			ImVec2 finalLT, finalRB;
			finalLT.x = min(m_MouseLT.x, m_MouseRB.x);
			finalLT.y = min(m_MouseLT.y, m_MouseRB.y);
			finalRB.x = max(m_MouseLT.x, m_MouseRB.x);
			finalRB.y = max(m_MouseLT.y, m_MouseRB.y);

			// 스프라이트 생성
			Vector2 vPos = Vector2(finalLT.x, finalLT.y);
			Vector2 vSize = Vector2(finalRB.x - finalLT.x, finalRB.y - finalLT.y);
			GetOwner()->CreateSprite(vPos, vSize);
		}
		else
		{
			GetOwner()->SelectClear();
		}
	}
}


void SE_AtlasView::Render_Sprite()
{
	bool isMouseHovering = false;
	bool isMoving = false;


	auto& vecSprite = GetOwner()->GetSprites();
	const auto& selected = GetOwner()->GetSelected();
	Vector2 AtlasSize = GetOwner()->GetAtlas()->GetResolution();

	for (auto iter = vecSprite.begin(); iter != vecSprite.end(); ++iter)
	{
		CSprite& Sprite = *iter;
		Vector2 vPixelLT = Sprite.GetLeftTop() * AtlasSize;
		Vector2 vPixelRB = (Sprite.GetLeftTop() + Sprite.GetSlice()) * AtlasSize;
		ImVec2 vSpriteLT = ImVec2(vPixelLT.x * m_Ratio + m_AtlasPos.x, vPixelLT.y * m_Ratio + m_AtlasPos.y);
		ImVec2 vSpriteRB = ImVec2(vPixelRB.x * m_Ratio + m_AtlasPos.x, vPixelRB.y * m_Ratio + m_AtlasPos.y);


		bool isSelected = (find(selected.begin(), selected.end(), &Sprite) != selected.end());

		if (ImGui::IsWindowFocused())
		{
			bool isSpriteHover = vSpriteLT.x < m_MousePos.x && m_MousePos.x < vSpriteRB.x &&
				vSpriteLT.y < m_MousePos.y && m_MousePos.y < vSpriteRB.y;
			isMouseHovering |= isSpriteHover;

			// 스프라이트 선택
			if (KEY_TAP(KEY::LBTN) && isSpriteHover && !m_OnCreate)
			{
				ImVec2 vDiff = ImVec2(m_MousePos.x - m_AtlasPos.x, m_MousePos.y - m_AtlasPos.y);
				m_MouseLT = ImVec2(vDiff.x / m_Ratio, vDiff.y / m_Ratio);
				m_MouseRB = m_MouseLT;
			}

			if (KEY_RELEASED(KEY::LBTN) && isSpriteHover && !m_OnCreate)
			{
				ImVec2 vDiff = ImVec2(m_MousePos.x - m_AtlasPos.x, m_MousePos.y - m_AtlasPos.y);
				m_MouseRB = ImVec2(vDiff.x / m_Ratio, vDiff.y / m_Ratio);

				isMoving = Vector2(m_MouseRB.x - m_MouseLT.x, m_MouseRB.y - m_MouseLT.y).Length() > 0.1f;
				if (!isMoving)
				{
					// 단일 선택
					if (!(KEY_PRESSED(KEY::CTRL)))
					{
						GetOwner()->SelectClear();
					}

					GetOwner()->SelectSprite(&Sprite);
				}
			}

			// 스프라이트 이동
			if (KEY_PRESSED(KEY::LBTN) && isSelected && !m_OnCreate)
			{
				// 이동량의 픽셀상 좌표 구하기
				Vector2 vMovePx = PosToPixel(PixelToPos(Sprite.GetLeftTopPixel()) + CKeyMgr::GetInst()->GetMouseDir());
				Sprite.SetLeftTop(vMovePx);

			}
		}


		// 스프라이트 렌더링
		ImU32 color = isSelected ? IM_COL32(255, 0, 0, 255) : IM_COL32(0, 255, 0, 255);
		ImGui::GetWindowDrawList()->AddRect(vSpriteLT, vSpriteRB, color, 0.f, 0.f, 1.f);

		// Pivot 렌더링
		// offset (0, 0) -> pivot ratio (0.5, 0.5)
		// offset (0, sprite_size.y / 2) -> pivot ratio (0.5, 1)
		// offset (sprite_size.x / 2, -sprite_size.y / 2) -> (0, 0)
		// offset (-sprite_size.x / 2, +sprite_size.y / 2) -> (1, 1)
		// pivot ratio : size.x/2 - offset, size.x/2 + offset
		Vector2 vPixelSize = vPixelRB - vPixelLT;
		Vector2 vPivotRatio = Vector2(vPixelSize.x / 2.f - Sprite.GetOffset().x, vPixelSize.y / 2.f - Sprite.GetOffset().y) / vPixelSize;
		Vector2 vPivotPos = (vPixelLT + vPivotRatio * vPixelSize) * m_Ratio + Vector2(m_AtlasPos.x, m_AtlasPos.y);
		ImGui::GetWindowDrawList()->AddCircle(ImVec2(vPivotPos.x, vPivotPos.y), 2.f, IM_COL32(255, 255, 255, 255), 0.0f, 1.5f);
	}

	//ImGui::GetWindowDrawList()->AddRect(m_MousePos, ImVec2(m_MousePos.x + 100.f, m_MousePos.y + 100.f), IM_COL32(255, 255, 0, 255), 0.f, 0.f, 1.f);
	if (KEY_RELEASED(KEY::LBTN) && !isMoving && !isMouseHovering && !m_OnCreate)
	{
		GetOwner()->AutoSliceOnce(PosToPixel(Vector2(m_MousePos.x, m_MousePos.y)));
	}

	if (!isMouseHovering)
		CreateDrag();

	if (m_OnCreate)
		OnDrag();


	// 선택된 모든 스프라이트 삭제
	if (KEY_TAP(KEY::DEL) && !selected.empty())
	{
		for (auto idx : selected)
		{
			GetOwner()->DeleteSprite(idx);
		}
		GetOwner()->SelectClear();
	}
}

bool SE_AtlasView::CalcSpriteSize(Vector2 _PixelPos)
{
	float left = GetOwner()->GetAtlas()->GetWidth() - 1.f;
	float top = GetOwner()->GetAtlas()->GetHeight() - 1.f;
	float right = 0.f;
	float bot = 0.f;

	// Quene 에 클릭한 최초의 픽셀좌표 입력
	list<Vector2>	queue;
	queue.push_back(_PixelPos);

	if (IsPixelOk(_PixelPos))
	{
		queue.push_back(_PixelPos);
		m_PixelID.insert(_PixelPos);
	}
	else
	{
		return false;
	}

	int isPresent = 0;

	while (!queue.empty())
	{
		Vector2 vPixelPos = queue.front();
		queue.pop_front();

		if (vPixelPos.x < left)
			left = vPixelPos.x;
		if (vPixelPos.x > right)
			right = vPixelPos.x;
		if (vPixelPos.y < top)
			top = vPixelPos.y;
		if (vPixelPos.y > bot)
			bot = vPixelPos.y;


		// 주변 픽셀을 queue 에 넣는다.
		Vector2 vUp = vPixelPos + Vector2(0.f, 1.f);	  // 위		
		Vector2 vDown = vPixelPos + Vector2(0.f, -1.f); // 아래		
		Vector2 vLeft = vPixelPos + Vector2(-1.f, 0.f); // 좌		
		Vector2 vRight = vPixelPos + Vector2(1.f, 0.f); // 우

		if (IsPixelOk(vUp))
		{
			queue.push_back(vUp);
			m_PixelID.insert(vUp);
			++isPresent;
		}

		if (IsPixelOk(vDown))
		{
			queue.push_back(vDown);
			m_PixelID.insert(vDown);
			++isPresent;
		}

		if (IsPixelOk(vLeft))
		{
			queue.push_back(vLeft);
			m_PixelID.insert(vLeft);
			++isPresent;
		}

		if (IsPixelOk(vRight))
		{
			queue.push_back(vRight);
			m_PixelID.insert(vRight);
			++isPresent;
		}
	}

	if (isPresent > 0)
	{
		m_MouseLT = ImVec2(left, top);
		m_MouseRB = ImVec2(right, bot);
		return true;
	}

	return false;
}

bool SE_AtlasView::IsPixelOk(Vector2 _PixelPos)
{
	// 해당 픽셀이 아틀라스 해상도 영역을 벗어난 좌표라면
	Ptr<CTexture> pAtlas = GetOwner()->GetAtlas();
	if (_PixelPos.x < 0 || pAtlas->GetWidth() <= _PixelPos.x
		|| _PixelPos.y < 0 || pAtlas->GetHeight() <= _PixelPos.y)
	{
		return false;
	}

	// 이미 등록된 적이 있는 Pixel 이라면
	if (m_PixelID.end() != m_PixelID.find(_PixelPos))
		return false;

	// 픽셀의 알파값으 0 이라면
	int PixelIdx = pAtlas->GetWidth() * (int)_PixelPos.y + (int)_PixelPos.x;
	if (0.f == pAtlas->GetPixels()[PixelIdx].a)
		return false;

	return true;
}
