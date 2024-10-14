#include "pch.h"
#include "CState.h"

CState::CState(UINT _Type)
	: m_Owner(nullptr)
	, m_StateType(_Type)
{
}

CState::~CState()
{
}