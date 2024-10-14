#pragma once
#include "EditorUI.h"
class ListUI :
    public EditorUI
{
private:
    vector<string>  m_vecList;
    int             m_SelectedIdx;
    string          m_SelectedName;
    
    EditorUI*       m_UI;
    DELEGATE_1      m_MemFunc;

public:
    virtual void Update() override;

public:
    void AddItem(const string& _Item) { m_vecList.push_back(_Item); }
    void AddList(const vector<string>& _List);

    void AddDelegate(EditorUI* _UI, DELEGATE_1 _Func)
    { 
        m_UI = _UI; 
        m_MemFunc = _Func;
    }

    virtual void Activate() override;
    virtual void Deactivate() override;

    string GetSelectName() { return m_SelectedName; }

public:
    ListUI();
    virtual ~ListUI();
};

