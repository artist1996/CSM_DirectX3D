#pragma once
class CEntity
{
private:
	static UINT g_ID;

private:
	wstring m_Name;
	UINT	m_ID;

public:
	UINT GetID() { return m_ID; }
	void SetName(const wstring& _Name) { m_Name = _Name; }
	wstring& GetName()				   { return m_Name; }

public:
	virtual CEntity* Clone() PURE;

public:
	CEntity();
	CEntity(const CEntity& _Other);
	virtual ~CEntity();
};

