#pragma once

class CGameObject;

class CTaskMgr
	: public CSingleton<CTaskMgr>
{
	SINGLE(CTaskMgr)
private:
	vector<tTask>			m_vecTask;
	vector<CGameObject*>	m_GC; // Garbage Collector;

public:
	void Tick();
	void AddTask(const tTask& _Task) { m_vecTask.push_back(_Task); }

private:
	void ClearGC();
	void ExecuteTask();	
};

