#pragma once

#include "singleton.h"

class CDistanceMgr
	: public CSingleton<CDistanceMgr>
{
	SINGLE(CDistanceMgr);
private:
	class CGameObject* m_Player;
	bool			   m_Range;

private:
	bool CalculateDistance();

public:
	void Init();
	void Tick();

	bool IsInRange() { return m_Range; }
};

