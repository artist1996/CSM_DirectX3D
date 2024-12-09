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
	// ��� ��ȯ
	if (KEY_TAP(KEY::NUM6))
	{
		if (HEIGHTMAP == m_Mode)
			m_Mode = SPLATING;
		else if (SPLATING == m_Mode)
			m_Mode = NONE;
		else
			m_Mode = HEIGHTMAP;
	}

	// �귯�� �ٲٱ�
	if (KEY_TAP(KEY::NUM7))
	{
		++m_BrushIdx;
		if (m_vecBrush.size() <= m_BrushIdx)
			m_BrushIdx = 0;
	}

	// ����ġ �ε��� �ٲٱ�
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
				// ���̸� ����
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

	// ������ �� ����
	GetMaterial(0)->SetScalarParam(INT_0, m_FaceX);
	GetMaterial(0)->SetScalarParam(INT_1, m_FaceZ);

	// ���� ���
	GetMaterial(0)->SetScalarParam(INT_2, (int)m_Mode);

	// �ؽ��� �迭 ����
	GetMaterial(0)->SetScalarParam(INT_3, (int)m_ColorTex->GetArraySize());

	// �׼����̼� ����
	GetMaterial(0)->SetScalarParam(VEC4_0, Vec4(m_MinLevel, m_MaxLevel, m_MinLevelRange, m_MaxLevelRange));

	// ī�޶� ���� ��ġ
	CCamera* pCam = CRenderMgr::GetInst()->GetPOVCam();
	GetMaterial(0)->SetScalarParam(VEC4_1, pCam->Transform()->GetWorldPos());

	// ������ �����ų ���̸�
	GetMaterial(0)->SetTexParam(TEX_0, m_HeightMap);

	// ���� ���� �� �븻 �ؽ���
	GetMaterial(0)->SetTexParam(TEXARR_0, m_ColorTex);
	GetMaterial(0)->SetTexParam(TEXARR_1, m_NormalTex);

	// Brush ����
	GetMaterial(0)->SetTexParam(TEX_1, m_vecBrush[m_BrushIdx]);
	GetMaterial(0)->SetScalarParam(VEC2_0, m_BrushScale);
	GetMaterial(0)->SetScalarParam(VEC2_1, m_Out.Location);
	GetMaterial(0)->SetScalarParam(FLOAT_0, (float)m_Out.Success);

	// ����ġ �ػ�
	GetMaterial(0)->SetScalarParam(VEC2_2, Vec2(m_WeightWidth, m_WeightHeight));

	// WeightMap t20 ���ε�
	m_WeightMap->Binding(20);

	// ���� ���ε�
	GetMaterial(0)->Binding();

	// ������
	GetMesh()->Render(0);

	// WeightMap ���� ���ε� Ŭ����
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
	// ���� ���� ī�޶� ��������
	CCamera* pCam = CRenderMgr::GetInst()->GetPOVCam();
	if (nullptr == pCam)
		return false;

	// ����ȭ���� Ŭ����
	m_Out = {};
	m_Out.Distance = 0xffffffff;
	m_RaycastOut->SetData(&m_Out, sizeof(tRaycastOut));

	// ī�޶� �������� ���콺�� ���ϴ� Ray ������ ������
	tRay ray = pCam->GetRay();

	// LandScape �� WorldInv ��� ������
	const Matrix& matWorldInv = Transform()->GetWorldMatInv();

	// ���� ���� Ray ������ LandScape �� Local �������� ������
	ray.vStart = XMVector3TransformCoord(ray.vStart, matWorldInv);
	ray.vDir = XMVector3TransformNormal(ray.vDir, matWorldInv);
	ray.vDir.Normalize();

	// Raycast ��ǻƮ ���̴��� �ʿ��� ������ ����
	m_RaycastCS->SetRayInfo(ray);
	m_RaycastCS->SetFace(m_FaceX, m_FaceZ);
	m_RaycastCS->SetOutBuffer(m_RaycastOut);
	m_RaycastCS->SetHeightMap(m_HeightMap);

	// ��ǻƮ���̴� ����
	m_RaycastCS->Execute();

	// ��� Ȯ��
	m_RaycastOut->GetData(&m_Out);

	return m_Out.Success;
}

void CLandScape::SaveToFile(FILE* _File)
{
}

void CLandScape::LoadFromFile(FILE* _File)
{
}
