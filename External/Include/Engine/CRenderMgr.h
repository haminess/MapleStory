#pragma once
#include "singleton.h"


#include "CTexture.h"

class CCamera;
class CLight2D;
class CStructuredBuffer;
class CFlipbook;

class CRenderMgr :
    public singleton<CRenderMgr>
{
    SINGLE(CRenderMgr);
private:
    int m_CamNum;
    bool m_bGrid;

    // 현재 레벨로 지정된 레벨 안에 있는 카메라들
    vector<CCamera*>        m_vecCam;
    CCamera*                m_RenderCam;    // 현재 렌더링 중인 카메라

    // 레벨에 속해있지 않은 외부 카메라
    CCamera*                m_EditorCam;   

    vector<CLight2D*>       m_vecLight2D;
    CStructuredBuffer*      m_Light2DBuffer;

    list<tTextInfo>         m_TextList;  // 텍스트 랜더링 정보

    list<tDebugShapeInfo>   m_DbgList;  // 디버그 랜더링 정보
    class CGameObject*      m_DbgObj;   // 디버그 렌더링 오브젝트

    Ptr<CTexture>           m_PostProcessTex; // 후처리용 텍스쳐(렌더타겟 복사용도)

    bool                    m_IsEditor; // 에디터모드 뷰 or 인게임 뷰


    // Fade
    CGameObject* m_FadeObj;
    float       m_FadeRatio;      // 0 ~ 1
    bool        m_FadeIn;         // true: FadeIn, false: FadeOut
    bool        m_Fade;           // Fade 진행중인지

    
    // Eff Obj
    list<CGameObject*> m_EffObjList;

public:
    void StartFade(bool _bFadeIn)
    {
        m_FadeIn = _bFadeIn;
        m_Fade = true;
        m_FadeRatio = m_FadeIn ? 0.f : 1.f;
    }


public:
    void RegisterCamera(CCamera* _Cam, UINT _Priority);
    void RegisterEditorCamera(CCamera* _EditorCam) { m_EditorCam = _EditorCam; }
    void DeregisterCamera() { m_vecCam.clear(); }

    void RegisterLight2D(CLight2D* _Light2D) { m_vecLight2D.push_back(_Light2D); }
    void AddDebugShape(const tDebugShapeInfo& _info) { m_DbgList.push_back(_info); }
    void AddText(const tTextInfo& _info) { m_TextList.push_back(_info); }
    void AddText(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color)
    { 
        tTextInfo info = {};
        info.Text = _pStr;
        info.Position.x = _fPosX;
        info.Position.y = _fPosY;
        info.FontSize = _fFontSize;
        info.Color = _Color;
        m_TextList.push_back(info);
    }
    void CopyRenderTarget();
    void SetEditorMode(bool _IsEditor) { m_IsEditor = _IsEditor; m_bGrid = true; }

    CCamera* GetRenderCamera() { return m_RenderCam; }
    void SetRenderCamera(CCamera* _Cam) {  m_RenderCam = _Cam; }


    const vector<CCamera*>& GetCameraList() { return m_vecCam; }
    int GetRenderCamNum() { return m_CamNum; }
    void SetRenderCamNum(int _PriorityNum) { m_CamNum = _PriorityNum; }

    void ShowGrid(bool _Show) { m_bGrid = _Show; }
    bool IsGrid() { return m_bGrid; }

public:
    void PlayAnimEff(Ptr<CFlipbook> _Flip, CGameObject* _Parent, Vector3 _Pos, Vector3 _Rot = Vector3(0.f, 0.f, 0.f), int _Layer = (int)LAYER_INDEX::DEFAULT);
    void ClearAnimEffObjects() { m_EffObjList.clear(); }


public:
    void Init();
    void Tick();
    void Render();

private:
    void RenderStart();
    void Binding();
    void Render_Text();
    void Render_Debug();

    void Render_Play();
    void Render_Editor();
};

