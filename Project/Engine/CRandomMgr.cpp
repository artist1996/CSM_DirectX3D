#include "pch.h"
#include "CRandomMgr.h"

CRandomMgr::CRandomMgr()
{}

CRandomMgr::~CRandomMgr()
{}

int CRandomMgr::GetRandom(int _Random)
{
	std::random_device rd;
	std::mt19937	   mt(rd());	

	std::uniform_int_distribution<int> dist(0, _Random);

	int randNum = dist(mt);

	return randNum;
}
