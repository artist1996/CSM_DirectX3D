#include "pch.h"
#include "CRenderMgr.h"
#include "CGameObject.h"
#include "assets.h"
#include "components.h"

#include "CDevice.h"
#include "CMRT.h"

#include "CDownScaleCS.h"
#include "CThresholdCS.h"
#include "CVerticalBlurCS.h"
#include "CHorizontalBlurCS.h"
#include "CUpScaleCS.h"

#include "CVolumetricLightCS.h"

void CRenderMgr::Init()
{
	m_PostProcessTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex");
	m_CopyTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"CopyTexture");

	m_DebugObject = new CGameObject;
	m_DebugObject->AddComponent(new CTransform);
	m_DebugObject->AddComponent(new CMeshRender);

	CreateMRT();

	CreateMaterial();

	CreateComputeShader();
}

void CRenderMgr::CreateMRT()
{
	// =============
	// SwapChain MRT
	// =============
	{
		Ptr<CTexture> arrRT[8] = { CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex"), CAssetMgr::GetInst()->FindAsset<CTexture>(L"ThresholdTex") };
		Ptr<CTexture> pDSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DepthStencilTex");
		Vec4		  arrClearColor[8] = { Vec4(0.f, 0.f, 0.f, 0.f), };

		m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->SetName(L"SwapChain");
		m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->Create(2, arrRT, pDSTex);
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
			CAssetMgr::GetInst()->CreateTexture(L"LightResultTex"
											, (UINT)vResolution.x, (UINT)vResolution.y
											, DXGI_FORMAT_R32G32B32A32_FLOAT
											, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE),
		};
		Ptr<CTexture> pDSTex = nullptr;
		Vec4		  arrClearColor[8] = { Vec4(0.f, 0.f, 0.f, 0.f), };

		m_arrMRT[(UINT)MRT_TYPE::LIGHT] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::LIGHT]->SetName(L"Light");
		m_arrMRT[(UINT)MRT_TYPE::LIGHT]->Create(4, arrRT, pDSTex);
		m_arrMRT[(UINT)MRT_TYPE::LIGHT]->SetClearColor(arrClearColor, false);
	}

	//===============
	//ShadowBlur MRT
	//===============
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


	// =========
	// Threshold
	// =========
	{
		Vec2 vResolution = CDevice::GetInst()->GetResolution();
	
		Ptr<CTexture> arrRT[8] = { CAssetMgr::GetInst()->CreateTexture(L"BloomThresholdTex"
											, (UINT)vResolution.x, (UINT)vResolution.y
											, DXGI_FORMAT_R32G32B32A32_FLOAT
											, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS) };
		Ptr<CTexture> pDSTex = nullptr;
		Vec4		  arrClearColor[8] = { Vec4(0.f, 0.f, 0.f, 0.f), };
	
		m_arrMRT[(UINT)MRT_TYPE::THRESHOLD] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::THRESHOLD]->SetName(L"Threshold");
		m_arrMRT[(UINT)MRT_TYPE::THRESHOLD]->Create(1, arrRT, nullptr);
		m_arrMRT[(UINT)MRT_TYPE::THRESHOLD]->SetClearColor(arrClearColor, false);
	}

	// ==========
	// DownScale
	// ==========
	{
		Vec2 vResolution = CDevice::GetInst()->GetResolution();

		Ptr<CTexture> arrRT[8] = { CAssetMgr::GetInst()->CreateTexture(L"DownScaleTex"
																	 , (UINT)vResolution.x * 0.25f, (UINT)vResolution.y * 0.25f
																	 , DXGI_FORMAT_R32G32B32A32_FLOAT
																	 , D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS), };
		Ptr<CTexture> pDSTex = nullptr;
		Vec4		  arrClearColor[8] = { Vec4(0.f, 0.f, 0.f, 0.f), };

		m_arrMRT[(UINT)MRT_TYPE::DOWNSCALE] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::DOWNSCALE]->SetName(L"DownScale");
		m_arrMRT[(UINT)MRT_TYPE::DOWNSCALE]->Create(1, arrRT, nullptr);
		m_arrMRT[(UINT)MRT_TYPE::DOWNSCALE]->SetClearColor(arrClearColor, false);

	}

	// VerticalBlur
	{
		Vec2 vResolution = CDevice::GetInst()->GetResolution();
		
		Ptr<CTexture> arrRT[8] = { CAssetMgr::GetInst()->CreateTexture(L"VerticalBlurTex"
																	 , (UINT)vResolution.x * 0.25f, (UINT)vResolution.y * 0.25f
																	 , DXGI_FORMAT_R32G32B32A32_FLOAT
																	 , D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS), };
		Ptr<CTexture> pDSTex = nullptr;
		Vec4		  arrClearColor[8] = { Vec4(0.f, 0.f, 0.f, 0.f), };

		m_arrMRT[(UINT)MRT_TYPE::VERTICAL_BLUR] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::VERTICAL_BLUR]->SetName(L"VerticalBlur");
		m_arrMRT[(UINT)MRT_TYPE::VERTICAL_BLUR]->Create(1, arrRT, nullptr);
		m_arrMRT[(UINT)MRT_TYPE::VERTICAL_BLUR]->SetClearColor(arrClearColor, false);
	}

	// HorizontalBlur
	{
		Vec2 vResolution = CDevice::GetInst()->GetResolution();

		Ptr<CTexture> arrRT[8] = { CAssetMgr::GetInst()->CreateTexture(L"HorizontalBlurTex"
																	 , (UINT)vResolution.x * 0.25f, (UINT)vResolution.y * 0.25f
																	 , DXGI_FORMAT_R32G32B32A32_FLOAT
																	 , D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS), };
		Ptr<CTexture> pDSTex = nullptr;
		Vec4		  arrClearColor[8] = { Vec4(0.f, 0.f, 0.f, 0.f), };

		m_arrMRT[(UINT)MRT_TYPE::HORIZONTAL_BLUR] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::HORIZONTAL_BLUR]->SetName(L"HorizontalBlur");
		m_arrMRT[(UINT)MRT_TYPE::HORIZONTAL_BLUR]->Create(1, arrRT, nullptr);
		m_arrMRT[(UINT)MRT_TYPE::HORIZONTAL_BLUR]->SetClearColor(arrClearColor, false);
	}

	// UpScale
	{
		Vec2 vResolution = CDevice::GetInst()->GetResolution();

		Ptr<CTexture> arrRT[8] = { CAssetMgr::GetInst()->CreateTexture(L"UpScaleTex"
																	 , (UINT)vResolution.x, (UINT)vResolution.y
																	 , DXGI_FORMAT_R32G32B32A32_FLOAT
																	 , D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS), };
		Ptr<CTexture> pDSTex = nullptr;
		Vec4		  arrClearColor[8] = { Vec4(0.f, 0.f, 0.f, 0.f), };

		m_arrMRT[(UINT)MRT_TYPE::UPSCALE] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::UPSCALE]->SetName(L"UpScale");
		m_arrMRT[(UINT)MRT_TYPE::UPSCALE]->Create(1, arrRT, nullptr);
		m_arrMRT[(UINT)MRT_TYPE::UPSCALE]->SetClearColor(arrClearColor, false);
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
	m_MergeMtrl->SetTexParam(TEX_5, CAssetMgr::GetInst()->FindAsset<CTexture>(L"UpScaleTex"));

	// RectMesh
	m_RectMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh");

	// DecalMtrl
	Ptr<CMaterial> pDeaclMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DecalMtrl");
	pDeaclMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PositionTargetTex"));

	// Threshold Shader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Threshold");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_Threshold");
	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_NONE);
	CAssetMgr::GetInst()->AddAsset(L"ThresholdShader", pMtrl);

	// Threshold Mtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(pShader);
	CAssetMgr::GetInst()->AddAsset(L"ThresholdMtrl", pMtrl);

	// Downscale Shader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_DownScale");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_DownScale");
	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_NONE);
	CAssetMgr::GetInst()->AddAsset(L"DownScaleShader", pMtrl);

	// DownScale Mtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(pShader);
	CAssetMgr::GetInst()->AddAsset(L"DownScaleMtrl", pMtrl);

	// Blur Shader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_BloomBlur");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_BloomBlur");
	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_NONE);
	CAssetMgr::GetInst()->AddAsset(L"BloomBlurShader", pMtrl);
	
	// Blur Mtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(pShader);
	CAssetMgr::GetInst()->AddAsset(L"BloomBlurMtrl", pMtrl);
	
	// Bloom Shader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Bloom");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_Bloom");
	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_NONE);
	CAssetMgr::GetInst()->AddAsset(L"BloomShader", pMtrl);
	
	// Bloom Mtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(pShader);
	CAssetMgr::GetInst()->AddAsset(L"BloomMtrl", pMtrl);
}



void CRenderMgr::CreateComputeShader()
{
	m_DownScaleCS = (CDownScaleCS*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"DownScaleCS").Get();

	if (nullptr == m_DownScaleCS)
	{
		m_DownScaleCS = new CDownScaleCS;
		CAssetMgr::GetInst()->AddAsset<CComputeShader>(L"DownScaleCS", m_DownScaleCS.Get());
	}
	
	m_ThresholdCS = (CThresholdCS*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"ThresholdCS").Get();

	if (nullptr == m_ThresholdCS)
	{
		m_ThresholdCS = new CThresholdCS;
		CAssetMgr::GetInst()->AddAsset<CComputeShader>(L"ThresholdCS", m_ThresholdCS.Get());
	}

	m_VerticalBlurCS = (CVerticalBlurCS*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"VerticalBlurCS").Get();

	if (nullptr == m_VerticalBlurCS)
	{
		m_VerticalBlurCS = new CVerticalBlurCS;
		CAssetMgr::GetInst()->AddAsset<CComputeShader>(L"VerticalBlurCS", m_VerticalBlurCS.Get());
	}


	m_HorizontalBlurCS = (CHorizontalBlurCS*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"HorizontalBlurCS").Get();

	if (nullptr == m_HorizontalBlurCS)
	{
		m_HorizontalBlurCS = new CHorizontalBlurCS;
		CAssetMgr::GetInst()->AddAsset<CComputeShader>(L"HorizontalBlurCS", m_HorizontalBlurCS.Get());
	}

	m_UpScaleCS = (CUpScaleCS*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"UpScaleCS").Get();

	if (nullptr == m_UpScaleCS)
	{
		m_UpScaleCS = new CUpScaleCS;
		CAssetMgr::GetInst()->AddAsset<CComputeShader>(L"UpScaleCS", m_UpScaleCS.Get());
	}

	m_VolumetricCS = (CVolumetricLightCS*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"VolumetricCS").Get();

	if (nullptr == m_VolumetricCS)
	{
		m_VolumetricCS = new CVolumetricLightCS;
		CAssetMgr::GetInst()->AddAsset<CComputeShader>(L"VolumetricCS", m_VolumetricCS.Get());
	}
}