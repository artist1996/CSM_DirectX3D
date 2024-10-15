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

CRenderMgr::CRenderMgr()
	: m_DebugObject(nullptr)
	, m_EditorCamera(nullptr)
	, m_Light2DBuffer(nullptr)
	, m_Light3DBuffer(nullptr)
{
	m_Light2DBuffer = new CStructuredBuffer;
	m_Light3DBuffer = new CStructuredBuffer;
}

CRenderMgr::~CRenderMgr()
{
	if (nullptr != m_DebugObject)
		delete m_DebugObject;

	SAFE_DELETE(m_Light2DBuffer);
	SAFE_DELETE(m_Light3DBuffer);
}

void CRenderMgr::Init()
{
	m_PostProcessTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex");
	m_CopyTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"CopyTexture");

	m_DebugObject = new CGameObject;
	m_DebugObject->AddComponent(new CTransform);
	m_DebugObject->AddComponent(new CMeshRender);
	m_DebugObject->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DebugShapeMtrl"));
}

void CRenderMgr::Tick()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	if (nullptr == pCurLevel)
		return;

	RenderStart();

	if (PLAY == pCurLevel->GetState())
	{
		for (size_t i = 0; i < m_vecCam.size(); ++i)
		{
			if (nullptr == m_vecCam[i])
				continue;

			m_vecCam[i]->Render();

			if(i == 0)
				RenderDebugShape();
		}
	}

	else
	{
		if (nullptr != m_EditorCamera)
		{
			m_EditorCamera->Render();
			RenderDebugShape();
		}
	}
	//CTimeMgr::GetInst()->Render();
	//CKeyMgr::GetInst()->Render();
	CopyTexture();

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
	Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	CONTEXT->CopyResource(m_CopyTex->GetTex2D().Get(), pRTTex->GetTex2D().Get());
}

void CRenderMgr::RenderDebugShape()
{
	list<tDebugShapeInfo>::iterator iter = m_DebugShapeList.begin();

	for (; iter != m_DebugShapeList.end();)
	{
		switch ((*iter).Shape)
		{
		case DEBUG_SHAPE::RECT:
			m_DebugObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh_Debug"));
			break;
		case DEBUG_SHAPE::CIRCLE:
			m_DebugObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh_Debug"));
			break;
		case DEBUG_SHAPE::LINE:
			break;
		case DEBUG_SHAPE::CUBE:
			break;
		case DEBUG_SHAPE::SPHERE:
			break;
		}

		// 위치 세팅
		m_DebugObject->Transform()->SetWorldMatrix((*iter).matWorld);

		// 재질 세팅
		m_DebugObject->MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, (*iter).vColor);
		
		// 깊이 판정 여부에 따라서, 쉐이더의 깊이판정 방식을 결정한다.
		if((*iter).DepthTest)
			m_DebugObject->MeshRender()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::LESS);
		else
			m_DebugObject->MeshRender()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

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

void CRenderMgr::RenderStart()
{
	// Output Merge State (출력 병합 단계)
	Ptr<CTexture> RTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	Ptr<CTexture> DSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DepthStencilTex");
	CONTEXT->OMSetRenderTargets(1, RTTex->GetRTV().GetAddressOf(), DSTex->GetDSV().Get());

	float color[4] = { 0.7f, 0.7f, 0.7f, 1.f };
	CONTEXT->ClearRenderTargetView(RTTex->GetRTV().Get(), color);
	CONTEXT->ClearDepthStencilView(DSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	g_GlobalData.g_Resolution = Vec2((float)RTTex->Width(), (float)RTTex->Height());
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

void CRenderMgr::Clear()
{
	m_vecLight2D.clear();
	m_vecLight3D.clear();
}
