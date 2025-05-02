#include "pch.h"
#include "CVolumetricLightCS.h"
#include "CLevelMgr.h"
#include "CGameObject.h"
#include "components.h"
#include "CStructuredBuffer.h"
#include "CTransform.h"
#include "CLight3D.h"
#include "CCamera.h"

CVolumetricLightCS::CVolumetricLightCS()
	: CComputeShader(8,8,8,L"shader\\volumetric.fx", "CS_VolumetricLight")
	, m_ModuleBuffer(nullptr)
{
	m_ModuleBuffer = new CStructuredBuffer;
	m_OutputBuffer = new CStructuredBuffer;
}

CVolumetricLightCS::~CVolumetricLightCS()
{
	SAFE_DELETE(m_ModuleBuffer);
	SAFE_DELETE(m_OutputBuffer);
}

int CVolumetricLightCS::Binding()
{
	if (nullptr == m_OutputTex)
		return E_FAIL;

	if (nullptr == m_OutputTex->GetSRV() || nullptr == m_OutputTex->GetUAV())
		return E_FAIL;

	CGameObject* pLight = CLevelMgr::GetInst()->FindObjectByName(L"SpotLight");

	if (nullptr == pLight || nullptr == pLight->Light3D())
		return E_FAIL;

	CLight3D* pLight3D = pLight->Light3D();
	
	tLightInfo tInfo = pLight3D->GetLightInformation();
		
	m_Module.fRadius			  = tInfo.Radius;				// 최대 거리
	m_Module.fAngle				  = cos(tInfo.Angle);			// 앵글
	m_Module.fScatteringIntensity = 1.f;						// 산란강도
	
	m_Module.Width	 = m_OutputTex->Width3D();
	m_Module.Height  = m_OutputTex->Height3D();
	m_Module.Depth	 = m_OutputTex->Depth();

	m_Module.vLightWorldPos		= tInfo.WorldPos;
	m_Module.vLightWorldDir		= tInfo.WorldPos.Normalize();
	m_Module.vLightCamWorldPos	= pLight3D->GetLightCam()->Transform()->GetWorldPos();
	m_Module.vAABBMin			= Vec3(-5.f, -5.f, -5.f);
	m_Module.vAABBMax			= Vec3(5.f, 5.f, 5.f);

	m_ModuleBuffer->Create(sizeof(tVolumetricModule), 1, SB_TYPE::SRV_UAV, true, &m_Module);
	m_OutputBuffer->Create(sizeof(tTestOutput), 1, SB_TYPE::SRV_UAV, true, &m_Buffer);
	m_OutputBuffer->SetData(&m_Buffer);
	
	m_ModuleBuffer->Binding_CS_SRV(21);
	m_OutputTex->Binding_CS_UAV(0);
	m_OutputBuffer->Binding_CS_UAV(1);

	

	return S_OK;
}

void CVolumetricLightCS::CalcGroupNum()
{
	if (nullptr == m_OutputTex)
		return;
	
	UINT Width  = m_OutputTex->Width3D();
	UINT Height = m_OutputTex->Height3D();
	UINT Depth  = m_OutputTex->Depth();

	m_GroupX = (Width + m_ThreadPerGroupX - 1) / m_ThreadPerGroupX;
	m_GroupY = (Height + m_ThreadPerGroupY - 1) / m_ThreadPerGroupY;
	m_GroupZ = (Depth + m_ThreadPerGroupZ - 1) / m_ThreadPerGroupZ;
}

void CVolumetricLightCS::Clear()
{
	m_OutputBuffer->GetData(&m_Buffer);

	m_OutputTex->Clear_CS_UAV();
	m_ModuleBuffer->Clear(21);
	m_OutputBuffer->Clear_CS_UAV();
}