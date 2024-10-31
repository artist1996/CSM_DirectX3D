#include "pch.h"
#include "CSkyBox.h"

#include "CAssetMgr.h"
#include "CTransform.h"

#include "CTimeMgr.h"

CSkyBox::CSkyBox()
	: CRenderComponent(COMPONENT_TYPE::SKYBOX)
	, m_Type(SKYBOX_TYPE::SPHERE)
{
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SkyBoxMtrl"));

	SetFrustumCheck(false);
}

CSkyBox::~CSkyBox()
{
}

void CSkyBox::FinalTick()
{
}

void CSkyBox::Render()
{
	Transform()->Binding();
	GetMaterial()->SetScalarParam(INT_0, (int)m_Type);

	if (SKYBOX_TYPE::SPHERE == m_Type)
	{
		if(!m_SkyBoxTex->IsCubeMap())
			GetMaterial()->SetTexParam(TEX_0, m_SkyBoxTex);
		else
			GetMaterial()->SetTexParam(TEX_0, nullptr);
	}
	else if(SKYBOX_TYPE::CUBE == m_Type)
	{
		if (m_SkyBoxTex->IsCubeMap())
			GetMaterial()->SetTexParam(TEXCUBE_0, m_SkyBoxTex);
		else
			GetMaterial()->SetTexParam(TEXCUBE_0, nullptr);
	}

	GetMaterial()->Binding();

	GetMesh()->Render();
}

void CSkyBox::SetSkyBoxType(SKYBOX_TYPE _Type)
{
	m_Type = _Type;

	if (SKYBOX_TYPE::SPHERE == m_Type)
		SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
	else if (SKYBOX_TYPE::CUBE == m_Type)
		SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CubeMesh"));
	
	// Mesh 가 변경되면 Material 을 다시 설정해준다.
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SkyBoxMtrl"));
}

void CSkyBox::SaveToFile(FILE* _pFile)
{
}

void CSkyBox::LoadFromFile(FILE* _pFile)
{
}