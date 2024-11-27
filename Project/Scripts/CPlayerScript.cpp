#include "pch.h"
#include "CPlayerScript.h"

CPlayerScript::CPlayerScript()
	: CScript(SCRIPT_TYPE::PLAYERSCRIPT)
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Begin()
{
	GetOwner()->GetRenderComponent()->GetDynamicMaterial(0);

}

void CPlayerScript::Tick()
{
	MouseCheck();
}

void CPlayerScript::SaveToFile(FILE* _pFile)
{
}

void CPlayerScript::LoadFromFile(FILE* _pFile)
{
}

void CPlayerScript::MouseCheck()
{
	Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
	Vec3 vPos = Transform()->GetWorldPos();
	Vec3 vScale = Transform()->GetRelativeScale();

	if (vPos.x - vScale.x * 0.5f < vMousePos.x && vPos.x + vScale.x * 0.5f > vMousePos.x
		&& vPos.y + vScale.y * 0.5f > vMousePos.y && vPos.y - vScale.y * 0.5f < vMousePos.y)
	{
		MeshRender()->GetMaterial(0)->SetScalarParam(INT_3, 1);
		MeshRender()->GetMaterial(0)->SetScalarParam(VEC4_1, Vec4(1.f, 0.f, 0.f, 1.f));
	}
	else
	{
		MeshRender()->GetMaterial(0)->SetScalarParam(INT_3, 0);
	}
}
