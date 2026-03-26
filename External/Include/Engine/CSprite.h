#pragma once
#include "CAsset.h"

#include "CTexture.h"

class CSprite :
	public CAsset
{
private:
	Ptr<CTexture>	m_Atlas;
	Vector2			m_LeftTopUV;
	Vector2			m_SliceUV;

	Vector2			m_BackgroundUV;
	Vector2			m_OffsetUV;

	float			m_Alpha;

public:
	void SetAtlasTexture(Ptr<CTexture> _Tex);
	void SetLeftTop(Vector2 _LeftTopPixel);
	void SetSlice(Vector2 _SlicePixel);
	void SetBackground(Vector2 _BackgroundPixel);
	void SetOffset(Vector2 _OffsetPixel);
	void SetAlpha(float _Alpha) { m_Alpha = _Alpha; }

	Ptr<CTexture> GetAtlasTexture() { return m_Atlas; }
	Vector2 GetLeftTop() { return m_LeftTopUV; }
	Vector2 GetSlice() { return m_SliceUV; }
	Vector2 GetBackground() { return m_BackgroundUV; }
	Vector2 GetOffset() { return m_OffsetUV; }
	Vector2 GetLeftTopPixel() { return m_LeftTopUV * m_Atlas->GetResolution(); }
	Vector2 GetSlicePixel() { return m_SliceUV * m_Atlas->GetResolution(); }
	Vector2 GetBackgroundPixel() { return m_BackgroundUV * m_Atlas->GetResolution(); }
	Vector2 GetOffsetPixel() { return m_OffsetUV * m_Atlas->GetResolution(); }
	float GetAlpha() { return m_Alpha; }

	void Binding();
	static void Clear();

private:
	virtual int Load(const wstring& _FilePath) override;

public:
	virtual int Save(const wstring& _strFilePath) override;
	int SaveContinue(FILE* _File);

public:
	CLONE(CSprite);
	CSprite(bool _bEngineRes = false);
	~CSprite();

public:
	static Vector2 GetUVFromPixel(Vector2 _Pixel, Vector2 _Resolution);
	static Vector2 GetPixelFromUV(Vector2 _Pixel, Vector2 _Resolution);
};
