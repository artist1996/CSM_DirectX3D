#include "pch.h"
#include "CFrustum.h"

#include "CCamera.h"

CFrustum::CFrustum(CCamera* _Owner)
	: m_Owner(_Owner)
{
	// �������������� �þ� �ִ� ��ġ
	//   4 -- 5
	//  /|   /|  <-- Far
	// 0 -- 1 6
	// | /  |/ <-- Near
	// 3 -- 2   

	m_arrProj[0] = Vec3(-1.f, 1.f, 0.f);
	m_arrProj[1] = Vec3(1.f, 1.f, 0.f);
	m_arrProj[2] = Vec3(1.f, -1.f, 0.f);
	m_arrProj[3] = Vec3(-1.f, -1.f, 0.f);

	m_arrProj[4] = Vec3(-1.f, 1.f, 1.f);
	m_arrProj[5] = Vec3(1.f, 1.f, 1.f);
	m_arrProj[6] = Vec3(1.f, -1.f, 1.f);
	m_arrProj[7] = Vec3(-1.f, -1.f, 1.f);
}

CFrustum::~CFrustum()
{
}

void CFrustum::FinalTick()
{
	// Camera �� Proj, View �����
	const Matrix& matProjInv = m_Owner->GetProjMatInv();
	const Matrix& matViewInv = m_Owner->GetViewMatInv();

	Matrix matInv = matProjInv * matViewInv;

	// ���� ��ǥ�迡 �ִ� �ʱ� ��ǥ�� ProjInv, ViewInv �� ���ؼ� World �������� �̵� ��Ų��.
	Vec3 arrWorld[8] = {};
	for (int i = 0; i < 8; ++i)
	{
		arrWorld[i] = XMVector3TransformCoord(m_arrProj[i], matInv);
	}

	// �������������� �þ� �ִ� ��ġ
	//   4 -- 5
	//  /|   /|  <-- Far
	// 0 -- 1 6
	// | /  |/ <-- Near
	// 3 -- 2

	// ���忡 �ִ� ����ü�� 8�� �������� �̿��ؼ� 6���� ����� ������.
	m_arrFace[(UINT)FACE_TYPE::FT_NEAR]  = XMPlaneFromPoints(arrWorld[0], arrWorld[1], arrWorld[2]);
	m_arrFace[(UINT)FACE_TYPE::FT_FAR]   = XMPlaneFromPoints(arrWorld[6], arrWorld[5], arrWorld[4]);
	m_arrFace[(UINT)FACE_TYPE::FT_LEFT]  = XMPlaneFromPoints(arrWorld[7], arrWorld[4], arrWorld[0]);
	m_arrFace[(UINT)FACE_TYPE::FT_RIGHT] = XMPlaneFromPoints(arrWorld[1], arrWorld[5], arrWorld[6]);
	m_arrFace[(UINT)FACE_TYPE::FT_TOP]   = XMPlaneFromPoints(arrWorld[4], arrWorld[5], arrWorld[1]);
	m_arrFace[(UINT)FACE_TYPE::FT_BOT]   = XMPlaneFromPoints(arrWorld[6], arrWorld[7], arrWorld[3]);
}

bool CFrustum::FrustumCheck(Vec3 _WorldPos, float _Radius)
{
	for (int i = 0; i < 6; ++i)
	{
		// w = -d
		Vec3 vNormal = m_arrFace[i];

		// |A| o |B| == D
		// |A| o |B| + (-D) = 0;
		// 0�� �� ũ�� ����ü ����, 0�� �� ������ ����ü �ٱ�
		if (vNormal.Dot(_WorldPos) + m_arrFace[i].w > _Radius)
			return false;
	}

	return true;
}