#include "pch.h"
#include "TE_Sub.h"

TE_Sub::TE_Sub()
	: m_Owner(nullptr)
{
}

TE_Sub::~TE_Sub()
{
}

void TE_Sub::Init()
{
}

void TE_Sub::Update()
{
}

void TE_Sub::Deactivate()
{
	m_TargetObject = nullptr;
}
