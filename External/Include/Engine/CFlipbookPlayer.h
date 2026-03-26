#pragma once
#include "CComponent.h"

#include "CFlipbook.h"

class CFlipbookPlayer :
    public CComponent
{
private:
    vector<Ptr<CFlipbook>>  m_vecFlipbook;      // 재생 목록 Flipbook
    Ptr<CFlipbook>          m_CurFlipbook;      // 재생중인 Flipbook
    int                     m_SpriteIdx;        // 재생중인 Sprite 인덱스
    bool                    m_Repeat;           // 반복모드
    float                   m_FPS;              // 재생속도
    float                   m_Time;             // 누적 시간
    bool                    m_Finish;           // 재생 회차가 끝났을 때
    bool                    m_Stop;             // 재생 회차가 끝났을 때

    // 렌더링
    float                    m_Alpha;

public:
    void AddFlipbook(int _idx, Ptr<CFlipbook> _Flipbook);
    void Play(int _Idx, float _FPS, bool _Repeat);
    void PlayOnce(Ptr<CFlipbook> _Flipbook, float _FPS);
    void PlayForDuration(int _Idx, float _Duration, bool _Repeat);
    void PlayForDuration(Ptr<CFlipbook> _Flipbook, float _Duration, bool _Repeat);
    void Stop()
    {
        m_CurFlipbook = nullptr;
        m_FPS = 0.f;
        m_Repeat = false;
        m_Time = 0.f;
        m_SpriteIdx = 0;
    }
    void Pause()
    {
        m_Stop = true;
    }

    Ptr<CSprite> GetCurrentSprite() { return m_CurFlipbook->GetSprite(m_SpriteIdx); }
    Ptr<CFlipbook> GetCurFlipbook() { return m_CurFlipbook; }
    Ptr<CFlipbook> GetFlipbook(int _idx) { return _idx < m_vecFlipbook.size()? m_vecFlipbook[_idx] : nullptr; }
    vector<Ptr<CFlipbook>>& GetFlipbookList() { return m_vecFlipbook; }
    float GetFPS() { return m_FPS; }
    bool IsLoop() { return m_Repeat; }
    bool IsPlaying() { return m_Repeat || (!m_Repeat && !m_Finish); }
    bool IsFinishedOnce() { return m_Finish; }

    void SetFlipbookAlpha(float _Alpha) { m_Alpha = _Alpha; }
    float GetFlipbookAlpha() { return m_Alpha; }

    void SetLoop(bool _Loop) { m_Repeat = _Loop; }
    int GetCurSpriteIdx() { return m_SpriteIdx; }
    void SetCurSpriteIdx(int _Idx) { m_SpriteIdx = _Idx; }

    void Binding();
    void Clear();

public:
    virtual void FinalTick() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _FILE) override;

public:
    CLONE(CFlipbookPlayer);
    CFlipbookPlayer();
    ~CFlipbookPlayer();
};

