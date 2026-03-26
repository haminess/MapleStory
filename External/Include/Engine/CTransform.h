#pragma once
#include "CComponent.h"
class CTransform :
    public CComponent
{
private:
    Vector3 m_RelativePos;
    Vector3 m_RelativeScale;
    Vector3 m_RelativeRotation;
    Vector3 m_LocalDir[(UINT)DIR_TYPE::END];
    Vector3 m_WorldDir[(UINT)DIR_TYPE::END];

    float m_Depth;

    Matrix m_matWorld;
    Vector3 m_PrevPos;

    bool m_IndependentPosition;
    bool m_IndependentScale;

public:
    Vector3 GetRelativePos() { return m_RelativePos; }
    Vector3 GetRelativeScale() { return m_RelativeScale; }
    Vector3 GetRelativeRotation() { return m_RelativeRotation; }
    Vector3 GetLocalDir(DIR_TYPE _Type) { return m_LocalDir[(UINT)_Type]; }
    Vector3 GetWorldDir(DIR_TYPE _Type) { return m_WorldDir[(UINT)_Type]; }
    const Matrix& GetWorldMat() { return m_matWorld; }
    void SetWorldMat(const Matrix& _mat) { m_matWorld = _mat; }
    Vector3 GetWorldScale();

    Vector3 GetWorldPos() 
    { 
        // Translation
        // 행렬의 4행 1, 2, 3열을 반환하는 함수
        return m_matWorld.Translation(); 
    }
    Vector3 GetPrevPos() { return m_PrevPos; }

    float GetDepth() { return m_Depth; }
    void SetDepth(float _Depth) { m_Depth = _Depth; }

    bool IsIndependentScale() { return m_IndependentScale; }

    void SetWorldPos(Vector3 _pos)
    {
        // 1. 월드 위치를 받아 월드 행렬에 반영
        // 2. 월드 행렬과 부모 행렬을 통해 상대 위치 구하기
         m_matWorld.Translation(_pos);

        // 부모가 있으면 월드 좌표를 로컬 좌표에 반영시켜줌
        if (GetOwner()->GetParent() && false == m_IndependentScale)
        {
            Matrix matParentWorld = GetOwner()->GetParent()->Transform()->GetWorldMat();

            Matrix matLocal = m_matWorld * matParentWorld.Invert();
            m_RelativePos = matLocal.Translation();
        }
        else
        {
            m_RelativePos = _pos;
        }
    }

    void SetRelativePos(Vector3 _Pos) { m_RelativePos = _Pos; }
    void SetRelativePos(float _x, float _y, float _z) { m_RelativePos = Vector3(_x, _y, _z); }
    void SetRelativeScale(Vector3 _Scale) { m_RelativeScale = _Scale; }
    void SetRelativeScale(float _x, float _y, float _z) { m_RelativeScale = Vector3(_x, _y, _z); }
    void SetRelativeRotation(Vector3 _Rotation) { m_RelativeRotation = _Rotation; }
    void SetRelativeRotation(float _x, float _y, float _z) { m_RelativeRotation = Vector3(_x, _y, _z); }
    void UseIndependentScale(bool _Scale) { m_IndependentScale = _Scale; }


    void Binding();

public:
    virtual void FinalTick() override;
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _FILE) override;


public:
    CLONE(CTransform);
    CTransform();
    ~CTransform();
};

