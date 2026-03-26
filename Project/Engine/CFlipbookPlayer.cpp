#include "pch.h"
#include "CFlipbookPlayer.h"

#include "CTimeMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"

CFlipbookPlayer::CFlipbookPlayer()
	: CComponent(COMPONENT_TYPE::FLIPBOOKPLAYER)
	, m_SpriteIdx(0)
	, m_Repeat(false)
	, m_FPS(24)
	, m_Time(0.f)
	, m_Alpha(1.f)
{
}

CFlipbookPlayer::~CFlipbookPlayer()
{
}



void CFlipbookPlayer::SaveComponent(FILE* _File)
{
	fwrite(&m_SpriteIdx, sizeof(int), 1, _File);
	fwrite(&m_Repeat, sizeof(bool), 1, _File);
	fwrite(&m_FPS, sizeof(float), 1, _File);

	size_t count = m_vecFlipbook.size();
	fwrite(&count, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < m_vecFlipbook.size(); ++i)
	{
		SaveAssetRef(m_vecFlipbook[i], _File);
	}

	SaveAssetRef(m_CurFlipbook, _File);
}

void CFlipbookPlayer::LoadComponent(FILE* _File)
{
	fread(&m_SpriteIdx, sizeof(int), 1, _File);
	fread(&m_Repeat, sizeof(bool), 1, _File);
	fread(&m_FPS, sizeof(float), 1, _File);

	size_t count = 0;
	fread(&count, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < count; ++i)
	{
		Ptr<CFlipbook> pFlipBook;
		LoadAssetRef(pFlipBook, _File);
		m_vecFlipbook.push_back(pFlipBook);
	}

	LoadAssetRef(m_CurFlipbook, _File);
}



void CFlipbookPlayer::FinalTick()
{
	if (nullptr == m_CurFlipbook || (m_Finish && !m_Repeat) || m_Stop)
		return;

	if (m_Finish && m_Repeat)
	{
		m_SpriteIdx = 0;
		m_Finish = false;
	}

	// 스프라이트 프레임 간격 시간
	float Term = (1.f / m_FPS);

	if (Term < m_Time)
	{
		++m_SpriteIdx;

		// Sprite 인덱스 초과
		if (m_CurFlipbook->GetMaxSprite() <= m_SpriteIdx)
		{
			m_SpriteIdx -= 1;
			m_Finish = true;
		}

		m_Time -= Term;
	}

	if(CLevelMgr::GetInst()->GetCurrentLevel()->GetState() == LEVEL_STATE::PLAY)
		m_Time += DT;
	else
		m_Time += EngineDT;
}

void CFlipbookPlayer::AddFlipbook(int _idx, Ptr<CFlipbook> _Flipbook)
{
	if (m_vecFlipbook.size() <= _idx)
	{
		m_vecFlipbook.resize(_idx + 1);
	}

	m_vecFlipbook[_idx] = _Flipbook;
}

void CFlipbookPlayer::Play(int _Idx, float _FPS, bool _Repeat)
{
	if (m_vecFlipbook.size() <= _Idx || m_vecFlipbook[_Idx] == nullptr)
		return;

	m_CurFlipbook = m_vecFlipbook[_Idx];
	m_FPS = _FPS;
	m_Repeat = _Repeat;
	m_Time = 0.f;
	m_SpriteIdx = 0;
	m_Finish = false;
	m_Stop = false;

	if (m_CurFlipbook->GetMaxSprite() > 0)
	{
		Vector2 vecSize = m_CurFlipbook->GetSprite(0)->GetBackgroundPixel();
		if (GetOwner())
			GetOwner()->Transform()->SetRelativeScale(vecSize.x, vecSize.y, 1.f);
	}
}
void CFlipbookPlayer::PlayOnce(Ptr<CFlipbook> _Flipbook, float _FPS)
{
	m_CurFlipbook = _Flipbook;
	m_FPS = _FPS;
	m_Repeat = false;
	m_Time = 0.f;
	m_SpriteIdx = 0;
	m_Finish = false;
	m_Stop = false;

	if (_Flipbook == nullptr)
		return; 

	if (m_CurFlipbook->GetMaxSprite() > 0)
	{
		Vector2 vecSize = m_CurFlipbook->GetSprite(0)->GetBackgroundPixel();
		if (GetOwner())
			GetOwner()->Transform()->SetRelativeScale(vecSize.x, vecSize.y, 1.f);
	}
}

void CFlipbookPlayer::PlayForDuration(int _Idx, float _Duration, bool _Repeat)
{
	Play(_Idx, GetFlipbook(_Idx)->GetMaxSprite() / _Duration, _Repeat);
}

void CFlipbookPlayer::PlayForDuration(Ptr<CFlipbook> _Flipbook, float _Duration, bool _Repeat)
{
	PlayOnce(_Flipbook, _Flipbook->GetMaxSprite() / _Duration);
}

void CFlipbookPlayer::Binding()
{
	if (nullptr == m_CurFlipbook || (m_Finish && !m_Repeat))
		return;

	Ptr<CSprite> Sprite = m_CurFlipbook->GetSprite(m_SpriteIdx);

	if (nullptr != Sprite)
	{
		Sprite->SetAlpha(m_Alpha);
		Sprite->SetOffset(m_CurFlipbook->GetAnimOffset());
		Sprite->Binding();
	}
}

void CFlipbookPlayer::Clear()
{
	if (nullptr == m_CurFlipbook)
		return;

	m_CurFlipbook->GetSprite(m_SpriteIdx)->SetAlpha(1.f);
	CSprite::Clear();
}