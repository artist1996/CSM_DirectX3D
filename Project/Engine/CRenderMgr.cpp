#include "pch.h"
#include "CRenderMgr.h"
#include "CCamera.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CAssetMgr.h"

#include "CTransform.h"
#include "CLight2D.h"
#include "CLight3D.h"
#include "CMeshRender.h"
#include "CGameObject.h"

#include "CLevelMgr.h"
#include "CLevel.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CStructuredBuffer.h"

#include "CFontMgr.h"

#include "CMRT.h"

CRenderMgr::CRenderMgr()
	: m_DebugObject(nullptr)
	, m_EditorCamera(nullptr)
	, m_Light2DBuffer(nullptr)
	, m_Light3DBuffer(nullptr)
	, m_arrMRT{}
{
	m_Light2DBuffer = new CStructuredBuffer;
	m_Light3DBuffer = new CStructuredBuffer;
}

CRenderMgr::~CRenderMgr()
{
	SAFE_DELETE(m_DebugObject);
	SAFE_DELETE(m_Light2DBuffer);
	SAFE_DELETE(m_Light3DBuffer);

	Delete_Array(m_arrMRT);
}

void CRenderMgr::Tick()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (nullptr == pCurLevel)
		return;

	RenderStart();

	// Level 이 Player 상태인 경우, Level 내에 있는 카메라 시점으로 렌더링하기
	if (PLAY == pCurLevel->GetState())
	{
		if (nullptr != m_vecCam[0])
			Render(m_vecCam[0]);

		for (size_t i = 1; i < m_vecCam.size(); ++i)
		{
			if (nullptr == m_vecCam[i])
				continue;

			Render_Sub(m_vecCam[i]);
		}
	}

	// Level 이 Stop 이나 Pause 인 경우, Editor 용 카메라 시점으로 렌더링 하기
	else
	{
		if (nullptr != m_EditorCamera)
		{
			Render(m_EditorCamera);
		}
	}

	// Debug Render
	RenderDebugShape();

	CopyTexture();
	// Clear
	Clear();
}

void CRenderMgr::RegisterCamera(CCamera* _Cam, int _CamPriority)
{
	// 카메라 우선순위에 따라서 벡터의 공간이 마련되어야 한다.
	if (m_vecCam.size() <= _CamPriority + 1)
		m_vecCam.resize(_CamPriority + 1);

	// 카메라 우선순위에 맞는 위치에 넣는다.
	m_vecCam[_CamPriority] = _Cam;
}

void CRenderMgr::PostProcessCopy()
{
	Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	CONTEXT->CopyResource(m_PostProcessTex->GetTex2D().Get(), pRTTex->GetTex2D().Get());
}

void CRenderMgr::CopyTexture()
{
	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->GetRT(0)->GetTex2D().Get();
	CONTEXT->CopyResource(m_CopyTex->GetTex2D().Get(), m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->GetRT(0)->GetTex2D().Get());
}

void CRenderMgr::RenderDebugShape()
{
	list<tDebugShapeInfo>::iterator iter = m_DebugShapeList.begin();

	Ptr<CGraphicShader> pDebugShape = CAssetMgr::GetInst()->FindAsset<CGraphicShader>(L"DebugShapeShader");
	Ptr<CGraphicShader> pDebugLine = CAssetMgr::GetInst()->FindAsset<CGraphicShader>(L"DebugLineShader");

	for (; iter != m_DebugShapeList.end();)
	{
		Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DebugShapeMtrl");
		pMtrl->GetShader()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		pMtrl->GetShader()->SetRSType(RS_TYPE::CULL_NONE);
		pMtrl->SetShader(pDebugShape);

		switch ((*iter).Shape)
		{
		case DEBUG_SHAPE::RECT:
			m_DebugObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh_Debug"));
			m_DebugObject->MeshRender()->SetMaterial(pMtrl, 0);
			break;
		case DEBUG_SHAPE::CIRCLE:
			m_DebugObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh_Debug"));
			m_DebugObject->MeshRender()->SetMaterial(pMtrl, 0);
			break;
		case DEBUG_SHAPE::LINE:
			m_DebugObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"PointMesh"));
			m_DebugObject->MeshRender()->SetMaterial(pMtrl, 0);
			pMtrl->SetShader(pDebugLine);
			pMtrl->GetShader()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
			break;
		case DEBUG_SHAPE::CUBE:
			m_DebugObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CubeMesh_Debug"));
			m_DebugObject->MeshRender()->SetMaterial(pMtrl, 0);
			break;
		case DEBUG_SHAPE::SPHERE:
			m_DebugObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
			m_DebugObject->MeshRender()->SetMaterial(pMtrl, 0);
			pMtrl->GetShader()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			pMtrl->GetShader()->SetRSType(RS_TYPE::CULL_FRONT);
			break;
		case DEBUG_SHAPE::CONE:
			m_DebugObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"ConeMesh"));
			m_DebugObject->MeshRender()->SetMaterial(pMtrl, 0);
			break;
		case DEBUG_SHAPE::TRIANGLE:
			m_DebugObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"TriangleMesh_Debug"));
			m_DebugObject->MeshRender()->SetMaterial(pMtrl, 0);
			break;
		}

		// 위치 세팅
		m_DebugObject->Transform()->SetWorldMatrix((*iter).matWorld);

		// 재질 세팅
		m_DebugObject->MeshRender()->GetMaterial(0)->SetScalarParam(INT_0, (int)(*iter).Shape);
		m_DebugObject->MeshRender()->GetMaterial(0)->SetScalarParam(VEC4_0, (*iter).vColor);

		if ((*iter).Shape == DEBUG_SHAPE::LINE)
		{
			// 시작점, 끝점
			m_DebugObject->MeshRender()->GetMaterial(0)->SetScalarParam(VEC4_1, Vec4((*iter).vPos, 1.f));
			m_DebugObject->MeshRender()->GetMaterial(0)->SetScalarParam(VEC4_2, Vec4((*iter).vScale, 1.f));
		}
		
		// 깊이 판정 여부에 따라서, 쉐이더의 깊이판정 방식을 결정한다.
		if((*iter).DepthTest)
			m_DebugObject->MeshRender()->GetMaterial(0)->GetShader()->SetDSType(DS_TYPE::NO_WRITE);
		else
			m_DebugObject->MeshRender()->GetMaterial(0)->GetShader()->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

		// 렌더링
		m_DebugObject->MeshRender()->Render();

		// 수명이 다한 디버그 정보 삭제
		(*iter).Age += EngineDT;
		if ((*iter).LifeTime < (*iter).Age)
		{
			iter = m_DebugShapeList.erase(iter);
		}

		else
		{
			++iter;
		}
	}
}

CCamera* CRenderMgr::GetPOVCam()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	if (nullptr == pCurLevel)
		return nullptr;

	if (LEVEL_STATE::PLAY == pCurLevel->GetState())
	{
		if (m_vecCam.empty())
			return nullptr;

		return m_vecCam[0];
	}
	else
		return m_EditorCamera;
}

void CRenderMgr::RenderStart()
{
	// Output Merge State (출력 병합 단계)
	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->ClearRT();
	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->ClearDS();
	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->OMSet();

	// GlobalData 설정
	g_GlobalData.g_Resolution   = CDevice::GetInst()->GetResolution();
	g_GlobalData.g_Light2DCount = (int)m_vecLight2D.size();
	g_GlobalData.g_Light3DCount = (int)m_vecLight3D.size();
	
	vector<tLightInfo> vecLight2DInfo;

	for (size_t i = 0; i < m_vecLight2D.size(); ++i)
	{
		vecLight2DInfo.push_back(m_vecLight2D[i]->GetLightInfo());
	}

	if (m_Light2DBuffer->GetElementCount() < m_vecLight2D.size())
	{
		m_Light2DBuffer->Create(sizeof(tLightInfo), (UINT)vecLight2DInfo.size(), SB_TYPE::SRV_ONLY, true);
	}

	if (!m_vecLight2D.empty())
	{
		m_Light2DBuffer->SetData(vecLight2DInfo.data());
		m_Light2DBuffer->Binding(11);
	}

	// Light3D 정보 업데이트 및 바인딩
	vector<tLightInfo> vecLight3DInfo;
	for (size_t i = 0; i < m_vecLight3D.size(); ++i)
	{
		vecLight3DInfo.push_back(m_vecLight3D[i]->GetLightInfo());
	}

	if (m_Light3DBuffer->GetElementCount() < vecLight3DInfo.size())
	{
		m_Light3DBuffer->Create(sizeof(tLightInfo), (UINT)vecLight3DInfo.size(), SB_TYPE::SRV_ONLY, true);
	}

	if (!vecLight3DInfo.empty())
	{
		m_Light3DBuffer->SetData(vecLight3DInfo.data());
		m_Light3DBuffer->Binding(12);
	}

	// 현재 화면을 렌더링하는 카메라의 월드포즈를 Global 데이터에 전달
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CCamera* pCam = nullptr;
	if (PLAY == pCurLevel->GetState())
		pCam = m_vecCam[0];
	else
		pCam = m_EditorCamera;

	if (pCam == nullptr)
		g_GlobalData.g_CamWorldPos = Vec3(0.f, 0.f, 0.f);
	else
		g_GlobalData.g_CamWorldPos = pCam->Transform()->GetWorldPos();

	static CConstBuffer* pGlobalCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);
	pGlobalCB->SetData(&g_GlobalData);
	pGlobalCB->Binding();
}

void CRenderMgr::Render(CCamera* _Cam)
{
	// ================
	// Create ShadowMap
	// ================
	// 광원 시점에서 물체들의 깊이를 기록
	for (size_t i = 0; i < m_vecLight3D.size(); ++i)
	{
		m_vecLight3D[i]->CreateShadowMap();
	}

	// 오브젝트 분류
	_Cam->SortGameObject();

	// 카메라 변환행렬 설정
	// 물체가 렌더링될 때 사용할 View, Proj 행렬
	g_Trans.matView		= _Cam->GetViewMat();
	g_Trans.matProj		= _Cam->GetProjMat();
	g_Trans.matViewInv	= _Cam->GetViewMatInv();
	g_Trans.matProjInv	= _Cam->GetProjMatInv();

	// MRT 모두 클리어
	ClearMRT();

	// ==================
	// DEFERRED RENDERING
	// ==================
	m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->OMSet();
	_Cam->render_deferred();

	// ===============
	// DECAL RENDERING
	// ===============
	m_arrMRT[(UINT)MRT_TYPE::DECAL]->OMSet();
	_Cam->render_decal();

	// ===============
	// LIGHT RENDERING
	// ===============
	m_arrMRT[(UINT)MRT_TYPE::LIGHT]->OMSet();

	for (size_t i = 0; i < m_vecLight3D.size(); ++i)
	{
		m_vecLight3D[i]->Render();
	}

	// Shadow Blur
	_Cam->render_shadowblur();

	// ===================================
	// MERGE ALBEDO + LIGHTS ==> SwapChain
	// ===================================
	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->OMSet();
	m_MergeMtrl->Binding();
	m_RectMesh->Render(0);


	// =================
	// FORWARD RENDERING
	// =================
	// 분류된 물체들 렌더링
	//_Cam->render_opaque();
	//_Cam->render_masked();
	//_Cam->render_effect();
	//_Cam->render_transparent();
	//_Cam->render_particle();
	//_Cam->render_postprocess();
	//_Cam->render_ui();

	_Cam->render_forward();

	// 정리
	_Cam->clear();
}

void CRenderMgr::Render_Sub(CCamera* _Cam)
{

}

void CRenderMgr::Clear()
{
	m_vecLight2D.clear();
	m_vecLight3D.clear();
}

void CRenderMgr::ClearMRT()
{
	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->Clear();
	m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->ClearRT();
	m_arrMRT[(UINT)MRT_TYPE::LIGHT]->ClearRT();
}