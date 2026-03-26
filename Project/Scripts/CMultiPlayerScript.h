#pragma once
#include <Engine/CScript.h>
class CMultiPlayerScript :
	public CScript
{
private:
	vector<CFlipbookPlayer*> m_Players;

public:
	void AddFlipbook(int _idx, Ptr<CFlipbook> _Flip);

	void Play(int _Idx, float _FPS, bool _Repeat)
	{
		for (auto& p : m_Players)
		{
			if(nullptr != p->GetFlipbook(_Idx)) 
				p->Play(_Idx, _FPS, _Repeat);
		}
	}
	void Stop()
	{
		for (auto& p : m_Players)
			p->Stop();
	}
	void Pause()
	{
		for (auto& p : m_Players)
			p->Pause();
	}
	bool IsPlaying();


public:
	virtual void Tick() override;

public:
	CLONE(CMultiPlayerScript)
	CMultiPlayerScript();
	CMultiPlayerScript(const CMultiPlayerScript& _Other);
	~CMultiPlayerScript();
};

