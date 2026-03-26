#include "pch.h"
#include "SpriteEditor.h"

#include "CImGuiMgr.h"
#include "SE_AtlasView.h"
#include "SE_Detail.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CTexture.h>
#include <Engine/CSprite.h>
#include <Engine/CGameObject.h>
#include <Engine/CMeshRender.h>
#include <Engine/CFlipbookPlayer.h>

SpriteEditor::SpriteEditor()
	: EditorUI("SpriteEditor_T")
	, m_AtlasView(nullptr)
	, m_Detail(nullptr)
{
	UseMenuBar(false);
}

SpriteEditor::~SpriteEditor()
{
}


void SpriteEditor::Init()
{
	m_AtlasView = (SE_AtlasView*)CImGuiMgr::GetInst()->FindUI("SE_AtlasView");
	m_Detail = (SE_Detail*)CImGuiMgr::GetInst()->FindUI("SE_Detail");

	m_AtlasView->SetMove(false);
	m_Detail->SetMove(false);

	m_AtlasView->m_Owner = this;
	m_Detail->m_Owner = this;

	Load(CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\link.png", L"Texture\\link.png"));
}

void SpriteEditor::Render_Update()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Window"))
		{
			bool AtlasView = m_AtlasView->IsActive();
			bool Detail = m_Detail->IsActive();

			if (ImGui::MenuItem("AtlasView", nullptr, &AtlasView))
			{
				m_AtlasView->SetActive(AtlasView);
			}

			if (ImGui::MenuItem("Detail", nullptr, &Detail))
			{
				m_Detail->SetActive(Detail);
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

}

void SpriteEditor::Activate()
{
	m_AtlasView->SetActive(true);
	m_Detail->SetActive(true);
}

void SpriteEditor::Deactivate()
{
	m_AtlasView->SetActive(false);
	m_Detail->SetActive(false);
}

void SpriteEditor::LoadFromDirectory()
{
	// 아틀라스 정보 로드
	path filepath = OpenFileExplorer(L"Texture\\");
	if (filepath != L"")
	{
		path Ext = filepath.extension();
		wstring relpath = CPathMgr::GetInst()->GetRelativePath(filepath);
		if (".sprite" == Ext)
		{
			Ptr<CSprite> pSprite = CAssetMgr::GetInst()->Load<CSprite>(relpath, relpath);
			Load(pSprite->GetAtlasTexture());
		}
		else
		{
			Load(CAssetMgr::GetInst()->Load<CTexture>(relpath, relpath));
		}
	}
}


void SpriteEditor::SliceByCount(Vector2 _Count)
{
	Vector2 vAtlasScale = m_AtlasTex->GetResolution();

	if (_Count.x < 1 || _Count.y < 1 || _Count.x > vAtlasScale.x / 3 || _Count.y > vAtlasScale.y / 3)
		return;

	// 기존 스프라이트 삭제
	m_Sprite.clear();

	// 새로운 스프라이트 생성
	Vector2 vSpriteScale = vAtlasScale / _Count;
	for (int cy = 0; cy < _Count.y; ++cy)
	{
		for (int cx = 0; cx < _Count.x; ++cx)
		{
			CreateSprite(Vector2(vSpriteScale.x * cx, vSpriteScale.y * cy), vSpriteScale, vSpriteScale);
		}
	}
}

void SpriteEditor::SliceBySize(Vector2 _Size)
{
	Vector2 vAtlasScale = m_AtlasTex->GetResolution();

	if (_Size.x < 1 || _Size.y < 1 || _Size.x > vAtlasScale.x || _Size.y > vAtlasScale.y)
		return;

	// 기존 스프라이트 삭제
	m_Sprite.clear();

	// 새로운 스프라이트 생성
	for (int cy = 0; cy < (int)vAtlasScale.y / (int)_Size.y; ++cy)
	{
		for (int cx = 0; cx < (int)vAtlasScale.x / (int)_Size.x; ++cx)
		{
			CreateSprite(Vector2(_Size.x * cx, _Size.y * cy), _Size, _Size);
		}
	}
}


void SpriteEditor::AutoSliceOnce(Vector2 _PixelPos)
{
	if (m_AutoSlice != nullptr)
		return;

	// 등록아이디 검사용 Set Clear
	m_PixelID.clear();

	if (CalcSpriteSize(_PixelPos))
	{
		CreateSprite(m_AutoSlice->GetLeftTopPixel(), m_AutoSlice->GetSlicePixel());
	}

	delete m_AutoSlice;
	m_AutoSlice = nullptr;
}

void SpriteEditor::AutoSliceAll()
{
	// 등록아이디 검사용 Set Clear
	m_PixelID.clear();

	for (int y = 0; y < m_AtlasTex->GetHeight(); ++y)
	{
		for (int x = 0; x < m_AtlasTex->GetWidth(); ++x)
		{
			if (CalcSpriteSize(Vector2(x, y)))
			{
				CreateSprite(m_AutoSlice->GetLeftTopPixel(), m_AutoSlice->GetSlicePixel());
				delete m_AutoSlice;
			}
		}
	}

	m_AutoSlice = nullptr;
}


bool SpriteEditor::CalcSpriteSize(Vector2 _PixelPos)
{
	float left = m_AtlasTex->GetWidth() - 1.f;
	float top = m_AtlasTex->GetHeight() - 1.f;
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
		m_AutoSlice = new CSprite;
		m_AutoSlice->SetAtlasTexture(m_AtlasTex);
		m_AutoSlice->SetLeftTop(Vector2(left, top));
		m_AutoSlice->SetSlice(Vector2(right - left, bot - top));

		return true;
	}

	return false;
}

bool SpriteEditor::IsPixelOk(Vector2 _PixelPos)
{
	// 해당 픽셀이 아틀라스 해상도 영역을 벗어난 좌표라면
	if (_PixelPos.x < 0 || m_AtlasTex->GetWidth() <= _PixelPos.x
		|| _PixelPos.y < 0 || m_AtlasTex->GetHeight() <= _PixelPos.y)
	{
		return false;
	}

	// 이미 등록된 적이 있는 Pixel 이라면
	if (m_PixelID.end() != m_PixelID.find(_PixelPos))
		return false;

	// 픽셀의 알파값으 0 이라면
	int PixelIdx = m_AtlasTex->GetWidth() * (int)_PixelPos.y + (int)_PixelPos.x;
	if (0.f == m_AtlasTex->GetPixels()[PixelIdx].a)
		return false;

	return true;
}



CGameObject* SpriteEditor::GetPreviewObject()
{
	if (nullptr == m_MapPreviewObject)
	{
		m_MapPreviewObject = new CGameObject;
		m_MapPreviewObject->AddComponent(new CMeshRender);
		m_MapPreviewObject->AddComponent(new CFlipbookPlayer);

		Ptr<CFlipbook> PreviewFlipbook = new CFlipbook;
		PreviewFlipbook->AddSprite(m_Selected[0]);
		m_MapPreviewObject->FlipbookPlayer()->AddFlipbook(0, PreviewFlipbook);
		CreateObject(m_MapPreviewObject, 0, false);
	}

	return m_MapPreviewObject;
}

void SpriteEditor::CreateSprite(Vector2 _Pos, Vector2 _Scale, Vector2 _Back, Vector2 _Offset)
{
	// 스프라이트 생성
	CSprite newSprite;

	// 텍스처에 등록
	newSprite.SetAtlasTexture(m_AtlasTex);
	newSprite.SetLeftTop(_Pos);
	newSprite.SetSlice(_Scale);
	if(_Back.x == 0.f || _Back.y == 0.f)
		newSprite.SetBackground(Vector2(m_AtlasTex->GetWidth(), m_AtlasTex->GetHeight()));
	else
		newSprite.SetBackground(_Back);
	newSprite.SetOffset(_Offset);

	m_Sprite.push_back(newSprite);
	
	SelectClear();
}

void SpriteEditor::Load(Ptr<CTexture> _Tex)
{
	// 아틀라스 로드
	m_AtlasTex = _Tex;

	// 스프라이트 정보 로드
	m_Sprite.clear();

	const vector<CSprite*>& vSprite = m_AtlasTex->GetSprites();
	for (int i = 0;i < vSprite.size(); ++i)
	{
		m_Sprite.push_back(*vSprite[i]);
	}

	m_Selected.clear();
	m_AtlasView->m_CursorPos = Vector2(0.f, 0.f);
}


void SpriteEditor::Save()
{
	if (nullptr == m_AtlasTex)
		return;

	// 기존 데이터 삭제
	// - 텍스처의 스프라이트 정보를 토대로 AssetMgr의 기존 스프라이트 삭제
	// - 텍스처의 스프라이트도 클리어
	// - 기존 sprite 파일은 삭제되지는 않을 것임
	vector<CSprite*>& vecSprites = m_AtlasTex->GetSpritesForEdit();
	for (auto pSprite : vecSprites)
	{
		CAssetMgr::GetInst()->DeleteAsset<CSprite>(pSprite->GetKey());
	}
	vecSprites.clear();

	// 새로운 데이터 등록
	// - 에디터의 스프라이트 정보를 토대로 AssetMgr에 생성 후 등록
	// - 위 과정에서 텍스처 vector에 추가 반영
	// - 위 과정에서 파일 작성
	wstring atlasName = CPathMgr::GetInst()->GetNameFromPath(m_AtlasTex->GetRelativePath());
	wstring path = CPathMgr::GetInst()->GetContentPath() + L"Sprite\\" + atlasName;
	int num = 0;
	for (auto iter = m_Sprite.begin(); iter != m_Sprite.end(); ++iter)
	{
		// 파일 저장
		wstring relPath = L"Sprite\\" + atlasName + L"_" + std::to_wstring(num) + L".sprite";
		wstring fullPath = CPathMgr::GetInst()->GetContentPath() + relPath;
		iter->SetAtlasTexture(m_AtlasTex);
		iter->Save(fullPath);

		// 에셋매니저 로드
		Ptr<CSprite> pSprite = CAssetMgr::GetInst()->Load<CSprite>(relPath, relPath);
		
		++num;
	}
}

void SpriteEditor::CreateFlipbook(bool _Loop)
{
	Ptr<CFlipbook> pNew = new CFlipbook;
	for (auto pSprite : m_Selected)
	{
		pNew->AddSprite(CAssetMgr::GetInst()->Load<CSprite>(pSprite->GetRelativePath(), pSprite->GetRelativePath()));
	}
	pNew->SetLoop(_Loop);

	wstring atlasName = CPathMgr::GetInst()->GetNameFromPath(m_AtlasTex->GetRelativePath());
	wstring relPath = L"Flipbook\\" + atlasName + L".flip";
	pNew->Save(CPathMgr::GetInst()->GetContentPath() + relPath);
	
	Ptr<CFlipbook> pSprite = CAssetMgr::GetInst()->Load<CFlipbook>(relPath, relPath);
}

