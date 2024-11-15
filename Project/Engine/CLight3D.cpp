#include "pch.h"
#include "CLight3D.h"

#include "CAssetMgr.h"
#include "CRenderMgr.h"
#include "CTransform.h"
#include "CCamera.h"
#include "CMRT.h"

CLight3D::CLight3D()
	: CComponent(COMPONENT_TYPE::LIGHT3D)
	, m_LightIdx(-1)
	, m_ShadowMapMRT(nullptr)
{
	m_Cam = new CGameObject;
	m_Cam->AddComponent(new CTransform);
	m_Cam->AddComponent(new CCamera);

	SetLightType(LIGHT_TYPE::DIRECTIONAL);
}

CLight3D::CLight3D(const CLight3D& _Origin)
	: CComponent(_Origin)
	, m_LightIdx(-1)
	, m_Info(_Origin.m_Info)
	, m_Cam(nullptr)
	, m_ShadowMapMRT(nullptr)
{
	m_Cam = _Origin.m_Cam->Clone();
	m_ShadowMapMRT = _Origin.m_ShadowMapMRT->Clone();
	SetLightType(m_Info.Type);
}

CLight3D::~CLight3D()
{
	SAFE_DELETE(m_Cam);
	SAFE_DELETE(m_ShadowMapMRT);
}

void CLight3D::FinalTick()
{
	m_Info.WorldPos = Transform()->GetWorldPos();
	m_Info.WorldDir = Transform()->GetWorldDir(DIR::FRONT);

	if (LIGHT_TYPE::POINT == m_Info.Type)
		Transform()->SetRelativeScale(m_Info.Radius * 2.f, m_Info.Radius * 2.f, m_Info.Radius * 2.f);
	else if(LIGHT_TYPE::SPOT == m_Info.Type)
		Transform()->SetRelativeScale(m_Info.Radius, m_Info.Radius, m_Info.Radius);
	else
		Transform()->SetRelativePos(m_TargetPos + -m_Info.WorldDir * 10000.f);

	// 자신을 RenderMgr 에 등록시킴
	m_LightIdx = CRenderMgr::GetInst()->RegisterLight3D(this);

#ifdef _DEBUG
	if (LIGHT_TYPE::POINT == m_Info.Type)
		DrawDebugSphere(Transform()->GetWorldMat(), Vec4(0.f, 1.f, 0.f, 1.f), 0.f, true);
	else if(LIGHT_TYPE::SPOT == m_Info.Type)
		DrawDebugCone(Transform()->GetWorldMat(), Vec4(0.f, 1.f, 0.f, 1.f), 0.f, true);
#endif
}

void CLight3D::Render()
{
	Transform()->Binding();

	// 광원 인덱스
	m_LightMtrl->SetScalarParam(INT_0, m_LightIdx);

	// 광원 카메라로 투영 시킬 때 사용할 ViewProj 행렬
	m_LightMtrl->SetScalarParam(MAT_0, m_Cam->Camera()->GetViewMat() * m_Cam->Camera()->GetProjMat());

	// ShadowMap 정보
	m_LightMtrl->SetTexParam(TEX_2, m_ShadowMapMRT->GetRT(0));
	m_LightMtrl->Binding();

	m_VolumeMesh->Render();

	//m_Cam->Camera()->render_shadowblur();
}

void CLight3D::CreateShadowMap()
{
	// Camera 의 Transform 에 Light3D 의 Transform 정보를 복사
	*m_Cam->Transform() = *Transform();

	// 현재 카메라의 위치를 기준으로 ViewMat, ProjMat 를 계산한다.
	m_Cam->Camera()->FinalTick();

	// 광원 카메라의 View Proj 정보를 상수버퍼 전역변수에 세팅한다.
	g_Trans.matView	   = m_Cam->Camera()->GetViewMat();
	g_Trans.matProj    = m_Cam->Camera()->GetProjMat();
	g_Trans.matViewInv = m_Cam->Camera()->GetViewMatInv();
	g_Trans.matProjInv = m_Cam->Camera()->GetProjMatInv();

	// MRT OMSet
	m_ShadowMapMRT->Clear();
	m_ShadowMapMRT->OMSet();

	m_ShadowMapMtrl->Binding();
	
	m_Cam->Camera()->SortGameObject_ShadowMap();
	m_Cam->Camera()->render_shadowmap();
}

void CLight3D::SetLightType(LIGHT_TYPE _Type)
{
	m_Info.Type = _Type;

	if (LIGHT_TYPE::DIRECTIONAL == _Type)
	{
		m_VolumeMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh");
		m_LightMtrl  = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DirLightMtrl");

		m_ShadowMapMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DirLightShadowMapMtrl");

		// 광원 카메라 옵션 설정
		m_Cam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
		m_Cam->Camera()->SetWidth(4096);
		m_Cam->Camera()->SetHeight(4096);
		m_Cam->Camera()->SetLayerAll();
		m_Cam->Camera()->SetLayer(31, false);
		m_Cam->Camera()->SetProjScale(1.f);

		Ptr<CTexture> pShadowMap = new CTexture;
		pShadowMap->Create(8192, 8192, DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

		Ptr<CTexture> pShadowMapDepth = new CTexture;
		pShadowMapDepth->Create(8192, 8192, DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL);

		if(nullptr == m_ShadowMapMRT)
			m_ShadowMapMRT = new CMRT;

		m_ShadowMapMRT->Create(1, &pShadowMap, pShadowMapDepth);

		Vec4 vClearColor = Vec4(-1.f, 0.f, 0.f, 0.f);
		m_ShadowMapMRT->SetClearColor(&vClearColor, true);
	}
	else if (LIGHT_TYPE::POINT == _Type)
	{
		m_VolumeMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh");
		m_LightMtrl  = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"PointLightMtrl");
	}
	else if (LIGHT_TYPE::SPOT == _Type)
	{
		m_VolumeMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"ConeMesh");
		m_LightMtrl  = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SpotLightMtrl");

		m_ShadowMapMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SpotLightShadowMapMtrl");

		// 광원 카메라 옵션 설정
		m_Cam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
		m_Cam->Camera()->SetWidth(4096);
		m_Cam->Camera()->SetHeight(4096);
		m_Cam->Camera()->SetLayerAll();
		m_Cam->Camera()->SetLayer(31, false);
		m_Cam->Camera()->SetProjScale(1.f);

		Ptr<CTexture> pShadowMap = new CTexture;
		pShadowMap->Create(8192, 8192, DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

		Ptr<CTexture> pShadowMapDepth = new CTexture;
		pShadowMapDepth->Create(8192, 8192, DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL);

		if (nullptr == m_ShadowMapMRT)
			m_ShadowMapMRT = new CMRT;

		m_ShadowMapMRT->Create(1, &pShadowMap, pShadowMapDepth);

		Vec4 vClearColor = Vec4(-1.f, 0.f, 0.f, 0.f);
		m_ShadowMapMRT->SetClearColor(&vClearColor, true);
	}
}

void CLight3D::SaveToFile(FILE* _pFile)
{
	fwrite(&m_Info, sizeof(tLightInfo), 1, _pFile);
	SaveAssetRef(m_VolumeMesh, _pFile);
	SaveAssetRef(m_LightMtrl, _pFile);
}

void CLight3D::LoadFromFile(FILE* _pFile)
{
	fread(&m_Info, sizeof(tLightInfo), 1, _pFile);
	LoadAssetRef(m_VolumeMesh, _pFile);
	LoadAssetRef(m_LightMtrl, _pFile);
}