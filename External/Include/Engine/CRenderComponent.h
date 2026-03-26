#pragma once
#include "CComponent.h"

#include "CMesh.h"
#include "CMaterial.h"


class CRenderComponent :
    public CComponent
{
private:
    Ptr<CMesh>              m_Mesh;

    Ptr<CMaterial>          m_SharedMtrl;   // 원본 재질
    Ptr<CMaterial>          m_CurMtrl;      // 현재 사용 중인 재질
    Ptr<CMaterial>          m_DynamicMtrl;   // 동적 재질

public:
    void SetMesh(Ptr<CMesh> _Mesh) { m_Mesh = _Mesh; }

    // 원본 재질을 등록하는 함수
    void SetMaterial(Ptr<CMaterial> _Material);     

    Ptr<CMesh> GetMesh() { return m_Mesh; }
    Ptr<CMaterial> GetMaterial() { return m_CurMtrl; }
    Ptr<CMaterial> GetMaterial() const { return m_CurMtrl; }
    Ptr<CMaterial> GetSharedMaterial();
    Ptr<CMaterial> GetDynamicMaterial();
    void CreateDynamicMaterial();


public:
    virtual void Render() = 0;

public:
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _FILE) override;

public:
    virtual CRenderComponent* Clone() = 0;
    CRenderComponent(COMPONENT_TYPE _Type);
    CRenderComponent(const CRenderComponent& _Origin);
    ~CRenderComponent();
};

