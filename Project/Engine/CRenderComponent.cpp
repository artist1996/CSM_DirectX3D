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

		// 원본 오브젝트가 공유재질을 참조하고 있고, 현재 사용재질은 공유재질이 아닌경우

		if (_Origin.m_vecMtrls[i].pSharedMtrl != _Origin.m_vecMtrls[i].pCurMtrl)
		{
			assert(_Origin.m_vecMtrls[i].pDynamicMtrl.Get());

			// 복사 렌더 컴포넌트도 별도의 동적 재질을 생성한다.
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
	// 재질은 ShadowMapMtrl 로 이미 Binding 되어있는걸 사용 할 것
	// 자신이 선택한 Mesh 로 렌더링 요청을 하면 된다.
	Transform()->Binding();

	// 모든 서브셋 메쉬의 깊이를 저장한다.
	for(int i = 0; i < GetMesh()->GetSubsetCount(); ++i)
		GetMesh()->Render(i);
}

void CRenderComponent::render_shadowblur()
{
	// 재질은 ShadowMapMtrl 로 이미 Binding 되어있는걸 사용 할 것
	// 자신이 선택한 Mesh 로 렌더링 요청을 하면 된다.
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
	// 재질이 변경되면 기존에 복사본 받아둔 DynamicMaterial 을 삭제한다.
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
	// 공유 재질을 가져오는 것으로 현재 사용재질을 동적재질에서 회복하도록 한다.

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
	// 메쉬 참조정보 저장
	SaveAssetRef(m_Mesh, _pFile);

	// 재질 참조정보 저장
	UINT iMtrlCount = GetMaterialCount();
	fwrite(&iMtrlCount, sizeof(UINT), 1, _pFile);
	
	for (UINT i = 0; i < iMtrlCount; ++i)
		SaveAssetRef(m_vecMtrls[i].pSharedMtrl, _pFile);
}

void CRenderComponent::LoadDataFromFile(FILE* _pFile)
{
	// 메쉬 참조정보 불러오기
	LoadAssetRef(m_Mesh, _pFile);

	UINT iMtrlCount = GetMaterialCount();
	fread(&iMtrlCount, sizeof(UINT), 1, _pFile);
	
	for (UINT i = 0; i < iMtrlCount; ++i)
		LoadAssetRef(m_vecMtrls[i].pSharedMtrl, _pFile);
}