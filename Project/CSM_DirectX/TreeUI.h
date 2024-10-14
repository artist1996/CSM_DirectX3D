#pragma once
#include "EditorUI.h"

class TreeNode
{
private:
    class TreeUI*     m_Owner;
    string            m_strName;
    TreeNode*         m_ParentNode;
    vector<TreeNode*> m_vecChildNode;

    const UINT        m_ID;
    DWORD_PTR         m_Data;

    bool              m_Frame;
    bool              m_Selected;

public:
    void SetName(const string& _strName) { m_strName = _strName; }
    const string& GetName()              { return m_strName; }

    void AddChildNode(TreeNode* _Node);
    void Update();

    TreeNode* GetParentNode() { return m_ParentNode; }

    void SetFrame(bool _Frame) { m_Frame = _Frame; }
    void EraseChild(TreeNode* _Node);
    bool IsFrame()             { return m_Frame; }

    void DragCheck();
    void DropCheck();

    DWORD_PTR GetData()        { return m_Data; }

public:
    TreeNode(UINT _ID);
    ~TreeNode();

    friend class TreeUI;
};

class TreeUI :
    public EditorUI
{
private:
    vector<TreeNode*> m_vecNodes;

    TreeNode*         m_Root;
    TreeNode*         m_SelectedNode;
    TreeNode*         m_DuplicateNode;
    TreeNode*         m_DragedNode;
    TreeNode*         m_DroppedNode;
                     
    UINT              m_NodeID;
    bool              m_ShowRoot;
                     
    bool              m_UseDrag;
    bool              m_UseDrop;
    bool              m_ShowNameOnly;
    bool              m_Duplicate;
                     
    EditorUI*         m_ClickedInst;
    DELEGATE_1        m_ClickedFunc;
                     
    EditorUI*         m_PopupInst;
    DELEGATE_1        m_PopupFunc;
                     
    EditorUI*         m_SelfDragDropInst;
    DELEGATE_2        m_SelfDragDropFunc;

    EditorUI*         m_DropInst;
    DELEGATE_2        m_DropFunc;
    string            m_DropPayLoadName;

public:
    virtual void Update() override;

    TreeNode* AddNode(TreeNode* _Parent, const string& _strName, DWORD_PTR _Data = 0);    
    TreeNode* GetSelectedNode()                     { return m_SelectedNode; };
    const vector<TreeNode*>& GetDuplicateNodes()    { return m_vecNodes; }

    void SetDragedNode(TreeNode* _Node);
    void SetDuplicateNode(TreeNode* _Node);
    void SetDuplicateNodes();

    void SetDroppedNode(TreeNode* _Node);
    void SetDropPayLoadName(const string& _strName) { m_DropPayLoadName = _strName; }
    void PopupMenu(TreeNode* _Node);

    void SetSelectedNode(TreeNode* _Node);
    void SetShowRoot(bool _Show)       { m_ShowRoot = _Show; }
    bool IsShowRoot()                  { return m_ShowRoot; }

    const string& GetDropPayLoadName() { return m_DropPayLoadName; }

    void UseDrag(bool _Drag)         { m_UseDrag = _Drag; }
    void UseDrop(bool _Drop)         { m_UseDrop = _Drop; }
    void SetShowNameOnly(bool _Name) { m_ShowNameOnly = _Name; }

    bool IsDrag()                { return m_UseDrag; }
    bool IsDrop()                { return m_UseDrop; }
    bool IsShowNameOnly()        { return m_ShowNameOnly; }
    bool IsEmptyDuplicateNodes() { return m_vecNodes.empty(); }

    void AddClickedDelegate(EditorUI* _Inst, DELEGATE_1 _Func)      { m_ClickedInst = _Inst; m_ClickedFunc = _Func; }
    void AddSelfDragDropDelegate(EditorUI* _Inst, DELEGATE_2 _Func) { m_SelfDragDropInst = _Inst; m_SelfDragDropFunc = _Func; }
    void AddDropDelegate(EditorUI* _Inst, DELEGATE_2 _Func)         { m_DropInst = _Inst; m_DropFunc = _Func; }
    void AddPopupDelegate(EditorUI* _Inst, DELEGATE_1 _Func)        { m_PopupInst = _Inst; m_PopupFunc = _Func; }
    void Clear();
    void ClearDuplicateNodes()                                      { m_vecNodes.clear(); }

public:
    TreeUI();
    virtual ~TreeUI();
};