#pragma once

#include "ImGui/imgui.h"

class EditorUI
{
private:
	static UINT	      m_GlobalID;

	const UINT		  m_ID;
	string			  m_Name;
	string			  m_FullName;
	
	EditorUI*		  m_Parent;
	vector<EditorUI*> m_vecChildUI;

	bool			  m_Active;
	bool			  m_ChildBorder;
	bool			  m_Modal;

	bool			  m_UseMenuBar;
	bool		      m_Move;
	bool			  m_HorizontalScrollbar;
	
	ImVec2			  m_ChildSize;

public:
	void AddChild(EditorUI* _UI);
	
	void SetActive(bool _Active);
	bool IsActive()					{ return m_Active; }

	void SetName(const string& _strName);
	void SetChildBorder(bool _Set)	{ m_ChildBorder = _Set; }
	void SetChildSize(ImVec2 _Size)	{ m_ChildSize = _Size; }

	void SetModal(bool _Modal)			   { m_Modal = _Modal; }
	bool IsModal()						   { return m_Modal; }
	void UseMenuBar(bool _Use)			   { m_UseMenuBar = _Use; }
	void SetMove(bool _Move)			   { m_Move = _Move; }
	void SetHorizontalScrollBar(bool _Use) { m_HorizontalScrollbar = _Use; }

	const string& GetName()		{ return m_Name; }
	const string& GetFullName() { return m_FullName; }

private:
	virtual void Activate() {}
	virtual void Deactivate() {}

public:
	virtual void Init() {}
	virtual void Tick();
	virtual void Update() PURE;

public:
	EditorUI();
	virtual ~EditorUI();
};

typedef void(EditorUI::* DELEGATE_0)(void);
typedef void(EditorUI::* DELEGATE_1)(DWORD_PTR);
typedef void(EditorUI::* DELEGATE_2)(DWORD_PTR, DWORD_PTR);
typedef void(EditorUI::* DELEGATE_3)(DWORD_PTR, DWORD_PTR, DWORD_PTR);