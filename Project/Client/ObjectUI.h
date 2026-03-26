#pragma once
#include "ListUI.h"

class CLevel;
class ObjectUI :
    public ListUI
{
private:
    CLevel* m_CurLevel;
    CGameObject* m_Target;

public:
    virtual void Tick() override;
    virtual void Render_Update() override;
    void Render_Child(class CGameObject* _Child);
    void Selected();
    void Clicked(int _itemNum);


public:
    virtual void Activate() override;
    virtual void Deactivate() override;

public:
    ObjectUI();
    ~ObjectUI();
};

