#include "pch.h"
#include "CRenderComponent.h"

#include "CAssetMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"

#include "CTransform.h"

CRenderComponent::CRenderComponent(COMPONENT_TYPE _Type)
	: CComponent(_Type)
	, m_Mesh(nullptr)
	, m_Mtrl(nullptr)
	, m_DynamicMtrl(nullptr)
	, m_FrustumCheck(true)
{
}

CRenderComponent::CRenderComponent(const CRenderComponent& _Origin)
	: CComponent(_Origin)
	, m_Mesh(_Origin.m_Mesh)
	, m_Mtrl(_Origin.m_Mtrl)
	, m_SharedMtrl(_Origin.m_SharedMtrl)
	, m_DynamicMtrl(nullptr)
	, m_FrustumCheck(_Origin.m_FrustumCheck)
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	if (nullptr != pCurLevel)
	{
		assert(!(pCurLevel->GetState() != PLAY
			  && nullptr != _Origin.m_DynamicMtrl));
	}

	if (nullptr != _Origin.m_DynamicMtrl)
	{
		GetDynamicMaterial();
	}
}

CRenderComponent::~CRenderComponent()
{
}

Ptr<CMaterial> CRenderComponent::GetSharedMaterial()
{
	m_Mtrl = m_SharedMtrl;
	return m_Mtrl;
}

void CRenderComponent::SetMaterial(Ptr<CMaterial> _Mtrl)
{
	m_Mtrl = m_SharedMtrl = _Mtrl;
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial()
{
	if (nullptr != m_DynamicMtrl)
		return m_DynamicMtrl;

	m_Mtrl = m_DynamicMtrl = m_SharedMtrl->Clone();

	return m_Mtrl;
}

void CRenderComponent::render_shadowmap()
{
	// ������ ShadowMapMtrl �� �̹� Binding �Ǿ��ִ°� ��� �� ��
	// �ڽ��� ������ Mesh �� ������ ��û�� �ϸ� �ȴ�.
	Transform()->Binding();
	GetMesh()->Render();
}

void CRenderComponent::render_shadowblur()
{
	// ������ ShadowMapMtrl �� �̹� Binding �Ǿ��ִ°� ��� �� ��
	// �ڽ��� ������ Mesh �� ������ ��û�� �ϸ� �ȴ�.
	Transform()->Binding();
	GetMesh()->Render();
}

void CRenderComponent::SaveDataToFile(FILE* _pFile)
{
	SaveAssetRef(m_Mesh, _pFile);
	SaveAssetRef(m_Mtrl, _pFile);
	SaveAssetRef(m_SharedMtrl, _pFile);
}

void CRenderComponent::LoadDataFromFile(FILE* _pFile)
{
	LoadAssetRef(m_Mesh, _pFile);
	LoadAssetRef(m_Mtrl, _pFile);
	LoadAssetRef(m_SharedMtrl, _pFile);
}