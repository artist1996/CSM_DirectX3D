#include "pch.h"
#include "CRenderComponent.h"

#include "CAssetMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"

#include "CTransform.h"

CRenderComponent::CRenderComponent(COMPONENT_TYPE _Type)
	: CComponent(_Type)
	, m_Mesh(nullptr)
	, m_FrustumCheck(true)
{
}

CRenderComponent::CRenderComponent(const CRenderComponent& _Origin)
	: CComponent(_Origin)
	, m_Mesh(_Origin.m_Mesh)
	, m_FrustumCheck(_Origin.m_FrustumCheck)
{
	m_vecMtrls.resize(_Origin.m_vecMtrls.size());

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	for (size_t i = 0; i < _Origin.m_vecMtrls.size(); ++i)
	{
		m_vecMtrls[i].pCurMtrl	  = _Origin.m_vecMtrls[i].pCurMtrl;
		m_vecMtrls[i].pSharedMtrl = _Origin.m_vecMtrls[i].pSharedMtrl;

		// ���� ������Ʈ�� ���������� �����ϰ� �ְ�, ���� ��������� ���������� �ƴѰ��

		if (_Origin.m_vecMtrls[i].pSharedMtrl != _Origin.m_vecMtrls[i].pCurMtrl)
		{
			assert(_Origin.m_vecMtrls[i].pDynamicMtrl.Get());

			// ���� ���� ������Ʈ�� ������ ���� ������ �����Ѵ�.
			GetDynamicMaterial(i);

			*m_vecMtrls[i].pDynamicMtrl.Get() = *_Origin.m_vecMtrls[i].pDynamicMtrl.Get();
		}
		else
		{
			m_vecMtrls[i].pCurMtrl = m_vecMtrls[i].pSharedMtrl;
		}
	}
}

CRenderComponent::~CRenderComponent()
{
}

void CRenderComponent::render_shadowmap()
{
	// ������ ShadowMapMtrl �� �̹� Binding �Ǿ��ִ°� ��� �� ��
	// �ڽ��� ������ Mesh �� ������ ��û�� �ϸ� �ȴ�.
	Transform()->Binding();

	// ��� ����� �޽��� ���̸� �����Ѵ�.
	for(int i = 0; i < GetMesh()->GetSubsetCount(); ++i)
		GetMesh()->Render(i);
}

void CRenderComponent::render_shadowblur()
{
	// ������ ShadowMapMtrl �� �̹� Binding �Ǿ��ִ°� ��� �� ��
	// �ڽ��� ������ Mesh �� ������ ��û�� �ϸ� �ȴ�.
	Transform()->Binding();
	for(int i = 0; i < GetMesh()->GetSubsetCount(); ++i)
		GetMesh()->Render(i);
}

void CRenderComponent::SetMesh(Ptr<CMesh> _Mesh)
{
	m_Mesh = _Mesh;

	if (m_vecMtrls.empty())
	{
		m_vecMtrls.clear();
		vector<tMtrlSet> vecMtrls;
		m_vecMtrls.swap(vecMtrls);
	}

	if (nullptr != m_Mesh)
		m_vecMtrls.resize(m_Mesh->GetSubsetCount());
}

void CRenderComponent::SetMaterial(Ptr<CMaterial> _Mtrl, UINT _Idx)
{
	// ������ ����Ǹ� ������ ���纻 �޾Ƶ� DynamicMaterial �� �����Ѵ�.
	m_vecMtrls[_Idx].pSharedMtrl  = _Mtrl;
	m_vecMtrls[_Idx].pCurMtrl     = _Mtrl;
	m_vecMtrls[_Idx].pDynamicMtrl = nullptr;
}

Ptr<CMaterial> CRenderComponent::GetMaterial(UINT _Idx)
{
	return m_vecMtrls[_Idx].pCurMtrl;
}

Ptr<CMaterial> CRenderComponent::GetSharedMaterial(UINT _Idx)
{
	// ���� ������ �������� ������ ���� ��������� ������������ ȸ���ϵ��� �Ѵ�.

	m_vecMtrls[_Idx].pCurMtrl = m_vecMtrls[_Idx].pSharedMtrl;

	if (m_vecMtrls[_Idx].pDynamicMtrl.Get())
		m_vecMtrls[_Idx].pDynamicMtrl = nullptr;

	return m_vecMtrls[_Idx].pSharedMtrl;
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial(UINT _Idx)
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	if (LEVEL_STATE::PLAY != pCurLevel->GetState())
		return nullptr;

	if (nullptr == m_vecMtrls[_Idx].pSharedMtrl)
	{
		m_vecMtrls[_Idx].pCurMtrl	  = nullptr;
		m_vecMtrls[_Idx].pDynamicMtrl = nullptr;
		return m_vecMtrls[_Idx].pCurMtrl;
	}

	if (nullptr == m_vecMtrls[_Idx].pDynamicMtrl)
	{
		m_vecMtrls[_Idx].pDynamicMtrl = m_vecMtrls[_Idx].pSharedMtrl->Clone();
		m_vecMtrls[_Idx].pDynamicMtrl->SetName(m_vecMtrls[_Idx].pSharedMtrl->GetName() + L"_Clone");
		m_vecMtrls[_Idx].pCurMtrl = m_vecMtrls[_Idx].pDynamicMtrl;
	}

	return m_vecMtrls[_Idx].pCurMtrl;
}

void CRenderComponent::SaveDataToFile(FILE* _pFile)
{
	// �޽� �������� ����
	SaveAssetRef(m_Mesh, _pFile);

	// ���� �������� ����
	UINT iMtrlCount = GetMaterialCount();
	fwrite(&iMtrlCount, sizeof(UINT), 1, _pFile);
	
	for (UINT i = 0; i < iMtrlCount; ++i)
		SaveAssetRef(m_vecMtrls[i].pSharedMtrl, _pFile);
}

void CRenderComponent::LoadDataFromFile(FILE* _pFile)
{
	// �޽� �������� �ҷ�����
	LoadAssetRef(m_Mesh, _pFile);

	UINT iMtrlCount = GetMaterialCount();
	fread(&iMtrlCount, sizeof(UINT), 1, _pFile);
	
	for (UINT i = 0; i < iMtrlCount; ++i)
		LoadAssetRef(m_vecMtrls[i].pSharedMtrl, _pFile);
}