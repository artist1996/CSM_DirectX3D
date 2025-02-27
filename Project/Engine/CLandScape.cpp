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
	, m_MinLevel(0)
	, m_MaxLevel(4)
	, m_MaxLevelRange(2000.f)
	, m_MinLevelRange(6000.f)
	, m_BrushIdx(-1)
	, m_BrushScale(Vec2(0.2f, 0.2f))
	, m_IsHeightMapCreated(false)
	, m_WeightMap(nullptr)
	, m_WeightWidth(0)
	, m_WeightHeight(0)
	, m_WeightIdx(0)
	, m_Mode(LANDSCAPE_MODE::SPLATING)
{
	SetFace(m_FaceX, m_FaceZ);

	Init();
}

CLandScape::~CLandScape()
{
	SAFE_DELETE(m_RaycastOut);
	SAFE_DELETE(m_WeightMap);
}

void CLandScape::FinalTick()
{
	// 모드 전환
	if (KEY_TAP(KEY::NUM6))
	{
		if (HEIGHTMAP == m_Mode)
			m_Mode = SPLATING;
		else if (SPLATING == m_Mode)
			m_Mode = NONE;
		else
			m_Mode = HEIGHTMAP;
	}

	// 브러쉬 바꾸기
	if (KEY_TAP(KEY::NUM7))
	{
		++m_BrushIdx;
		if (m_vecBrush.size() <= m_BrushIdx)
			m_BrushIdx = 0;
	}

	// 가중치 인덱스 바꾸기
	if (KEY_TAP(KEY::NUM8))
	{
		++m_WeightIdx;
		if (m_ColorTex->GetArraySize() <= (UINT)m_WeightIdx)
			m_WeightIdx = 0;
	}

	if (NONE == m_Mode)
		return;

	Raycasting();

	if (HEIGHTMAP == m_Mode)
	{
		if (m_IsHeightMapCreated && KEY_PRESSED(KEY::LBTN))
		{
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

	else if (SPLATING == m_Mode)
	{
		if (KEY_PRESSED(KEY::LBTN) && m_WeightWidth != 0 && m_WeightHeight != 0)
		{
			if (m_Out.Success)
			{
				m_WeightMapCS->SetBrushPos(m_RaycastOut);
				m_WeightMapCS->SetBrushScale(m_BrushScale);
				m_WeightMapCS->SetBrushTex(m_vecBrush[m_BrushIdx]);
				m_WeightMapCS->SetWeightMap(m_WeightMap);
				m_WeightMapCS->SetWeightIdx(m_WeightIdx);
				m_WeightMapCS->SetWeightMapWidthHeight(m_WeightWidth, m_WeightHeight);

				m_WeightMapCS->Execute();
			}
		}
	}
}

void CLandScape::Render()
{
	Transform()->Binding();

	//GetMaterial()->GetShader()->SetRSType(RS_TYPE::WIRE_FRAME);

	// 지형의 면 개수
	GetMaterial(0)->SetScalarParam(INT_0, m_FaceX);
	GetMaterial(0)->SetScalarParam(INT_1, m_FaceZ);

	// 지형 모드
	GetMaterial(0)->SetScalarParam(INT_2, (int)m_Mode);

	// 텍스쳐 배열 개수
	GetMaterial(0)->SetScalarParam(INT_3, (int)m_ColorTex->GetArraySize());

	// 테셀레이션 레벨
	GetMaterial(0)->SetScalarParam(VEC4_0, Vec4(m_MinLevel, m_MaxLevel, m_MinLevelRange, m_MaxLevelRange));

	// 카메라 월드 위치
	CCamera* pCam = CRenderMgr::GetInst()->GetPOVCam();
	GetMaterial(0)->SetScalarParam(VEC4_1, pCam->Transform()->GetWorldPos());

	// 지형에 적용시킬 높이맵
	GetMaterial(0)->SetTexParam(TEX_0, m_HeightMap);

	// 지형 색상 및 노말 텍스쳐
	GetMaterial(0)->SetTexParam(TEXARR_0, m_ColorTex);
	GetMaterial(0)->SetTexParam(TEXARR_1, m_NormalTex);

	// Brush 정보
	GetMaterial(0)->SetTexParam(TEX_1, m_vecBrush[m_BrushIdx]);
	GetMaterial(0)->SetScalarParam(VEC2_0, m_BrushScale);
	GetMaterial(0)->SetScalarParam(VEC2_1, m_Out.Location);
	GetMaterial(0)->SetScalarParam(FLOAT_0, (float)m_Out.Success);

	// 가중치 해상도
	GetMaterial(0)->SetScalarParam(VEC2_2, Vec2(m_WeightWidth, m_WeightHeight));

	// WeightMap t20 바인딩
	m_WeightMap->Binding(20);

	// 재질 바인딩
	GetMaterial(0)->Binding();

	// 렌더링
	GetMesh()->Render(0);

	// WeightMap 버퍼 바인딩 클리어
	m_WeightMap->Clear(20);
}

void CLandScape::SetFace(int _X, int _Z)
{
	m_FaceX = _X;
	m_FaceZ = _Z;

	CreateMesh();
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LandScapeMtrl"), 0);
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

void CLandScape::SetBurshIndex(Ptr<CTexture> _BrushTex)
{
	for (size_t i = 0; i < m_vecBrush.size(); ++i)
	{
		if (_BrushTex == m_vecBrush[i])
		{
			m_BrushIdx = (UINT)i;
			break;
		}
	}
}

void CLandScape::SetWeightIndex(int _Idx)
{
	m_WeightIdx = _Idx;

	if (m_ColorTex->GetArraySize() <= (UINT)m_WeightIdx || 0 > m_WeightIdx)
		m_WeightIdx = 0;
}

void CLandScape::SaveToFile(FILE* _File)
{
}

void CLandScape::LoadFromFile(FILE* _File)
{
}

