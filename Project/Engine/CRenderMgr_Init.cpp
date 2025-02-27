#include "pch.h"
#include "CRenderMgr.h"
#include "CGameObject.h"
#include "assets.h"
#include "components.h"

#include "CDevice.h"
#include "CMRT.h"

void CRenderMgr::Init()
{
	m_PostProcessTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex");
	m_CopyTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"CopyTexture");

	m_DebugObject = new CGameObject;
	m_DebugObject->AddComponent(new CTransform);
	m_DebugObject->AddComponent(new CMeshRender);

	CreateMRT();

	CreateMaterial();
}

void CRenderMgr::CreateMRT()
{
	// =============
	// SwapChain MRT
	// =============
	{
		Ptr<CTexture> arrRT[8] = { CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex"), };
		Ptr<CTexture> pDSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DepthStencilTex");
		Vec4		  arrClearColor[8] = { Vec4(0.f, 0.f, 0.f, 0.f), };

		m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->SetName(L"SwapChain");
		m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->Create(1, arrRT, pDSTex);
		m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->SetClearColor(arrClearColor, false);
	}

	// =============
	// Effect MRT
	// =============
	{
		Ptr<CTexture> arrRT[8] = { CAssetMgr::GetInst()->FindAsset<CTexture>(L"EffectTargetTex"), };
		Ptr<CTexture> pDSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"EffectDepthStencilTex");
		Vec4		  arrClearColor[8] = { Vec4(0.f, 0.f, 0.f, 0.f), };

		m_arrMRT[(UINT)MRT_TYPE::EFFECT] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::EFFECT]->SetName(L"Effect");
		m_arrMRT[(UINT)MRT_TYPE::EFFECT]->Create(1, arrRT, pDSTex);
		m_arrMRT[(UINT)MRT_TYPE::EFFECT]->SetClearColor(arrClearColor, false);
	}

	// ===============
	// EffectBlur MRT
	// ===============
	{
		Ptr<CTexture> arrRT[8] = { CAssetMgr::GetInst()->FindAsset<CTexture>(L"EffectBlurTargetTex"), };
		Ptr<CTexture> pDSTex = nullptr;
		Vec4		  arrClearColor[8] = { Vec4(0.f, 0.f, 0.f, 0.f), };

		m_arrMRT[(UINT)MRT_TYPE::EFFECT_BLUR] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::EFFECT_BLUR]->SetName(L"EffectBlur");
		m_arrMRT[(UINT)MRT_TYPE::EFFECT_BLUR]->Create(1, arrRT, nullptr);
		m_arrMRT[(UINT)MRT_TYPE::EFFECT_BLUR]->SetClearColor(arrClearColor, false);
	}


	// ========
	// Deferred
	// ========
	{
		Vec2 vResolution = CDevice::GetInst()->GetResolution();

		Ptr<CTexture> arrRT[8] =
		{
			CAssetMgr::GetInst()->CreateTexture(L"AlbedoTargetTex"
											, (UINT)vResolution.x, (UINT)vResolution.y
											, DXGI_FORMAT_R8G8B8A8_UNORM
											, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE),
			CAssetMgr::GetInst()->CreateTexture(L"NormalTargetTex"
											, (UINT)vResolution.x, (UINT)vResolution.y
											, DXGI_FORMAT_R32G32B32A32_FLOAT
											, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE),
			CAssetMgr::GetInst()->CreateTexture(L"PositionTargetTex"
											, (UINT)vResolution.x, (UINT)vResolution.y
											, DXGI_FORMAT_R32G32B32A32_FLOAT
											, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE),
			CAssetMgr::GetInst()->CreateTexture(L"EmissiveTargetTex"
											, (UINT)vResolution.x, (UINT)vResolution.y
											, DXGI_FORMAT_R32G32B32A32_FLOAT
											, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE),
			CAssetMgr::GetInst()->CreateTexture(L"DataTargetTex"
											, (UINT)vResolution.x, (UINT)vResolution.y
											, DXGI_FORMAT_R32G32B32A32_FLOAT
											, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE),
		};
		Ptr<CTexture> pDSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DepthStencilTex");
		Vec4		  arrClearColor[8] = { Vec4(0.f, 0.f, 0.f, 0.f), };

		m_arrMRT[(UINT)MRT_TYPE::DEFERRED] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->SetName(L"Deferred");
		m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->Create(5, arrRT, pDSTex);
		m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->SetClearColor(arrClearColor, false);
	}

	// =====
	// LIGHT
	// =====
	{
		Vec2 vResolution = CDevice::GetInst()->GetResolution();

		Ptr<CTexture> arrRT[8] =
		{
			CAssetMgr::GetInst()->CreateTexture(L"DiffuseTargetTex"
											, (UINT)vResolution.x, (UINT)vResolution.y
											, DXGI_FORMAT_R32G32B32A32_FLOAT
											, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE),
			CAssetMgr::GetInst()->CreateTexture(L"SpecularTargetTex"
											, (UINT)vResolution.x, (UINT)vResolution.y
											, DXGI_FORMAT_R32G32B32A32_FLOAT
											, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE),
			CAssetMgr::GetInst()->CreateTexture(L"ShadowTargetTex"
											, (UINT)vResolution.x, (UINT)vResolution.y
											, DXGI_FORMAT_R32G32B32A32_FLOAT
											, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE),
		};
		Ptr<CTexture> pDSTex = nullptr;
		Vec4		  arrClearColor[8] = { Vec4(0.f, 0.f, 0.f, 0.f), };

		m_arrMRT[(UINT)MRT_TYPE::LIGHT] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::LIGHT]->SetName(L"Light");
		m_arrMRT[(UINT)MRT_TYPE::LIGHT]->Create(3, arrRT, pDSTex);
		m_arrMRT[(UINT)MRT_TYPE::LIGHT]->SetClearColor(arrClearColor, false);
	}

	// ===============
	// ShadowBlur MRT
	// ===============
	{
		Vec2 vResolution = CDevice::GetInst()->GetResolution();

		Ptr<CTexture> arrRT[8] = { CAssetMgr::GetInst()->CreateTexture(L"ShadowBlurTex"
											, (UINT)vResolution.x, (UINT)vResolution.y
											, DXGI_FORMAT_R32G32B32A32_FLOAT
											, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE), };
		Ptr<CTexture> pDSTex = nullptr;
		Vec4		  arrClearColor[8] = { Vec4(0.f, 0.f, 0.f, 0.f), };

		m_arrMRT[(UINT)MRT_TYPE::SHADOWBLUR] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::SHADOWBLUR]->SetName(L"ShadowBlur");
		m_arrMRT[(UINT)MRT_TYPE::SHADOWBLUR]->Create(1, arrRT, nullptr);
		m_arrMRT[(UINT)MRT_TYPE::SHADOWBLUR]->SetClearColor(arrClearColor, false);
	}

	// =====
	// DECAL
	// =====
	{
		Vec2 vResolution = CDevice::GetInst()->GetResolution();

		Ptr<CTexture> arrRT[8] =
		{
			CAssetMgr::GetInst()->FindAsset<CTexture>(L"AlbedoTargetTex"),
			CAssetMgr::GetInst()->FindAsset<CTexture>(L"EmissiveTargetTex"),
		};

		Ptr<CTexture> pDSTex = nullptr;
		Vec4		  arrClearColor[8] = { Vec4(0.f, 0.f, 0.f, 0.f), };

		m_arrMRT[(UINT)MRT_TYPE::DECAL] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::DECAL]->SetName(L"Decal");
		m_arrMRT[(UINT)MRT_TYPE::DECAL]->Create(2, arrRT, pDSTex);
		m_arrMRT[(UINT)MRT_TYPE::DECAL]->SetClearColor(arrClearColor, false);
	}
}

void CRenderMgr::CreateMaterial()
{
	// DirLightShader
	Ptr<CGraphicShader> pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\light.fx", "VS_DirLight");
	pShader->CreatePixelShader(L"shader\\light.fx", "PS_DirLight");
	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetBSType(BS_TYPE::ONE_ONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_LIGHT);
	CAssetMgr::GetInst()->AddAsset(L"DirLightShader", pShader);

	// DirLightMtrl
	Ptr<CMaterial> pMtrl = new CMaterial(true);
	pMtrl->SetShader(pShader);
	pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PositionTargetTex"));
	pMtrl->SetTexParam(TEX_1, CAssetMgr::GetInst()->FindAsset<CTexture>(L"NormalTargetTex"));
	CAssetMgr::GetInst()->AddAsset(L"DirLightMtrl", pMtrl);

	// PointLight Shader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\light.fx", "VS_PointLight");
	pShader->CreatePixelShader(L"shader\\light.fx", "PS_PointLight");
	pShader->SetRSType(RS_TYPE::CULL_FRONT);
	pShader->SetBSType(BS_TYPE::ONE_ONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_LIGHT);
	CAssetMgr::GetInst()->AddAsset(L"PointLightShader", pShader);

	// PointLight Mtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(pShader);
	pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PositionTargetTex"));
	pMtrl->SetTexParam(TEX_1, CAssetMgr::GetInst()->FindAsset<CTexture>(L"NormalTargetTex"));
	CAssetMgr::GetInst()->AddAsset(L"PointLightMtrl", pMtrl);

	// SpotLight Shader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\light.fx", "VS_SpotLight");
	pShader->CreatePixelShader(L"shader\\light.fx", "PS_SpotLight");
	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetBSType(BS_TYPE::ONE_ONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_LIGHT);
	CAssetMgr::GetInst()->AddAsset(L"SpotLightShader", pShader);

	// SpotLight Mtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(pShader);
	pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PositionTargetTex"));
	pMtrl->SetTexParam(TEX_1, CAssetMgr::GetInst()->FindAsset<CTexture>(L"NormalTargetTex"));
	CAssetMgr::GetInst()->AddAsset(L"SpotLightMtrl", pMtrl);

	// MergeShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\merge.fx", "VS_Merge");
	pShader->CreatePixelShader(L"shader\\merge.fx", "PS_Merge");
	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_NONE);

	// MergeMtrl
	m_MergeMtrl = new CMaterial(true);
	m_MergeMtrl->SetShader(pShader);
	m_MergeMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"AlbedoTargetTex"));
	m_MergeMtrl->SetTexParam(TEX_1, CAssetMgr::GetInst()->FindAsset<CTexture>(L"DiffuseTargetTex"));
	m_MergeMtrl->SetTexParam(TEX_2, CAssetMgr::GetInst()->FindAsset<CTexture>(L"SpecularTargetTex"));
	m_MergeMtrl->SetTexParam(TEX_3, CAssetMgr::GetInst()->FindAsset<CTexture>(L"EmissiveTargetTex"));
	m_MergeMtrl->SetTexParam(TEX_4, CAssetMgr::GetInst()->FindAsset<CTexture>(L"ShadowBlurTex"));

	// RectMesh
	m_RectMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh");

	// DecalMtrl
	Ptr<CMaterial> pDeaclMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DecalMtrl");
	pDeaclMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PositionTargetTex"));
}