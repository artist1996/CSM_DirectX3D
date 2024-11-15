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
	// �귯�� �ٲٱ�
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
			// ���̸� ����
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

	// ������ �� ����
	GetMaterial()->SetScalarParam(INT_0, m_FaceX);
	GetMaterial()->SetScalarParam(INT_1, m_FaceZ);

	// ������ �����ų ���̸�
	GetMaterial()->SetTexParam(TEX_0, m_HeightMap);

	// ���� ���ε�
	GetMaterial()->Binding();

	// ������
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
