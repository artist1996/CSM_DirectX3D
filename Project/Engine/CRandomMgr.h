#pragma once

#include "singleton.h"

class CRandomMgr
	: public CSingleton<CRandomMgr>
{
	SINGLE(CRandomMgr);

public:
	int GetRandom(int _Random);

public:
};

