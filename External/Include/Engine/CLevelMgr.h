#pragma once

class CLevel;

class CLevelMgr
	: public CSingleton<CLevelMgr>
{
	SINGLE(CLevelMgr);
private:
	vector<CLevel*>			m_vecLevel;
	CLevel*					m_CurLevel;

	bool					m_LevelChanged;

public:
	CLevel* GetCurrentLevel() { return m_CurLevel; }
	CGameObject* FindObjectByName(const wstring& _strName);

	void AddLevel(CLevel* _pLevel) { m_vecLevel.push_back(_pLevel); }
	void LevelChanged();
	bool IsLevelChanged() { return m_LevelChanged; }

	bool CheckMonster();

	CGameObject* GetPlayer();
	CGameObject* GetPlayerJump();
	CGameObject* GetPlayerMove();

private:
	bool ChangeLevel(CLevel* _NextLevel);

public:
	void Init();
	void Progress();

	friend class CTaskMgr;
};

