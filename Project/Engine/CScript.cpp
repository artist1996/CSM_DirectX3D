#include "pch.h"
#include "CScript.h"

CScript::CScript(UINT _ScriptType)
	: CComponent(COMPONENT_TYPE::SCRIPT)
	, m_ScriptType(_ScriptType)
{
}

CScript::~CScript()
{
}

void CScript::Instantiate(Ptr<CPrefab> _Pref, int _LayerIdx, Vec3 _WorldPos, const wstring& _strName)
{
	CGameObject* pInstance = _Pref->Instantiate();

	pInstance->SetName(_strName);
	pInstance->Transform()->SetRelativePos(_WorldPos);

	CreateObject(pInstance, _LayerIdx);
}