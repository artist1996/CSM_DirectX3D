#include "pch.h"
#include "CComponent.h"

CComponent::CComponent(COMPONENT_TYPE _Type)
	: m_Owner(nullptr)
	, m_Type(_Type)
{
}

CComponent::CComponent(const CComponent& _Origin)
	: CEntity(_Origin)
	, m_Owner(nullptr)
	, m_Type(_Origin.m_Type)
{
}

CComponent::~CComponent()
{
}

void CComponent::Begin()
{
}

void CComponent::Tick()
{
}
