#include "pch.h"
#include "CDecal.h"

#include "CAssetMgr.h"
#include "CTransform.h"

CDecal::CDecal()
	: CRenderComponent(COMPONENT_TYPE::DECAL)
{
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CubeMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DecalMtrl"));
}

CDecal::~CDecal()
{
}

void CDecal::FinalTick()
{
	DrawDebugCube(Transform()->GetWorldMatrix(), Vec4(0.f, 1.f, 0.f, 1.f), 0, true);
}

void CDecal::Render()
{
	Transform()->Binding();

	GetMaterial()->SetTexParam(TEX_1, m_DecalTex);
	GetMaterial()->Binding();

	GetMesh()->Render();
}

void CDecal::SaveToFile(FILE* _File)
{
}

void CDecal::LoadFromFile(FILE* _File)
{
}