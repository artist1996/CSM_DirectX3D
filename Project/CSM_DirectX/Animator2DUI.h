#pragma once
#include "ComponentUI.h"
class Animator2DUI :
    public ComponentUI
{
private:
    UINT m_UIHeight;
    int  m_SelectedIdx;
    int  m_Idx;

public:
    virtual void Update() override;

private:
    void SelectFlipBook(DWORD_PTR _ListUI);
    

public:
    Animator2DUI();
    virtual ~Animator2DUI();
};

