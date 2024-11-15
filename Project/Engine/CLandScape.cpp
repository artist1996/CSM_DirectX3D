#include "pch.h"
#include "CLandScape.h"

#include "CAssetMgr.h"
#include "CKeyMgr.h"
#include "CTransform.h"
#include "CCamera.h"
#include "CRenderMgr.h"
#include "CStructuredBuffer.h"

CLandScape::CLandScape()
	: CRenderComponent(COMPONENT_TYPE::LANDSCAPE)
	, m_FaceX(1)
	, m_FaceZ(1)
	, m_BrushIdx(-1)
	, m_BrushScale(Vec2(0.2f, 0.2f))
	, m_IsHeightMapCreated(false)
{
	SetFace(m_FaceX, m_FaceZ);

	Init();
}

CLandScape::~CLandScape()
{
	if (nullptr != m_RaycastOut)
		delete m_RaycastOut;
}

void CLandScape::FinalTick()
{
	// 브러쉬 바꾸기
	if (KEY_TAP(KEY::NUM7))
	{
		++m_BrushIdx;
		if (m_vecBrush.size() <= m_BrushIdx)
			m_BrushIdx = 0;
	}

	if (m_IsHeightMapCreated && KEY_PRESSED(KEY::LBTN))
	{
		Raycasting();

		if (m_Out.Success)
		{
			// 높이맵 설정
			m_HeightMapCS->SetBrushPos(m_RaycastOut);
			m_HeightMapCS->SetBrushScale(m_BrushScale);
			m_HeightMapCS->SetHeightMap(m_HeightMap);
			m_HeightMapCS->SetBrushTex(m_vecBrush[m_BrushIdx]);
			m_HeightMapCS->Execute();
		}
	}
}

void CLandScape::Render()
{
	Transform()->Binding();

	//GetMaterial()->GetShader()->SetRSType(RS_TYPE::WIRE_FRAME);

	// 지형의 면 개수
	GetMaterial()->SetScalarParam(INT_0, m_FaceX);
	GetMaterial()->SetScalarParam(INT_1, m_FaceZ);

	// 지형에 적용시킬 높이맵
	GetMaterial()->SetTexParam(TEX_0, m_HeightMap);

	// 재질 바인딩
	GetMaterial()->Binding();

	// 렌더링
	GetMesh()->Render();
}

void CLandScape::SetFace(int _X, int _Z)
{
	m_FaceX = _X;
	m_FaceZ = _Z;

	CreateMesh();
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LandScapeMtrl"));
}

int CLandScape::Raycasting()
{
	// 현재 시점 카메라 가져오기
	CCamera* pCam = CRenderMgr::GetInst()->GetPOVCam();
	if (nullptr == pCam)
		return false;

	// 구조화버퍼 클리어
	m_Out = {};
	m_Out.Distance = 0xffffffff;
	m_RaycastOut->SetData(&m_Out, sizeof(tRaycastOut));

	// 카메라가 시점에서 마우스를 향하는 Ray 정보를 가져옴
	tRay ray = pCam->GetRay();

	// LandScape 의 WorldInv 행렬 가져옴
	const Matrix& matWorldInv = Transform()->GetWorldMatInv();

	// 월드 기준 Ray 정보를 LandScape 의 Local 공간으로 데려감
	ray.vStart = XMVector3TransformCoord(ray.vStart, matWorldInv);
	ray.vDir = XMVector3TransformNormal(ray.vDir, matWorldInv);
	ray.vDir.Normalize();

	// Raycast 컴퓨트 쉐이더에 필요한 데이터 전달
	m_RaycastCS->SetRayInfo(ray);
	m_RaycastCS->SetFace(m_FaceX, m_FaceZ);
	m_RaycastCS->SetOutBuffer(m_RaycastOut);
	m_RaycastCS->SetHeightMap(m_HeightMap);

	// 컴퓨트쉐이더 실행
	m_RaycastCS->Execute();

	// 결과 확인
	m_RaycastOut->GetData(&m_Out);

	return m_Out.Success;
}

void CLandScape::SaveToFile(FILE* _File)
{
}

void CLandScape::LoadFromFile(FILE* _File)
{
}
