#include "pch.h"
#include "CCollisionMgr.h"

#include "CLevelMgr.h"
#include "CAssetMgr.h"
#include "CLevel.h"
#include "CLayer.h"

#include "CGameObject.h"
#include "CCollider3D.h"
#include "CCollider2D.h"
#include "CBoundingSphere.h"

#include "CMesh.h"

CCollisionMgr::CCollisionMgr()
    : m_Matrix{}
{
}

CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::Tick()
{
	for (int Row = 0; Row < MAX_LAYER; ++Row)
	{
        for (int Col = Row; Col < MAX_LAYER; ++Col)
        {
            if (m_Matrix[Row] & (1 << Col))
            {
				CollisionBtwLayer(Row, Col);
            }
        }
	}
}

void CCollisionMgr::CollisionCheck(UINT _Layer1, UINT _Layer2)
{
    UINT Row = _Layer1;
    UINT Col = _Layer2;

    if (Row > Col)
    {
        Row = _Layer2;
        Col = _Layer1;
    }

    if (Row & (1 << Col))
    {
        m_Matrix[Row] &= ~(1 << Col);
    }

    else
    {
		m_Matrix[Row] |= (1 << Col);
    }
}

void CCollisionMgr::CollisionUnCheck(UINT _Layer1, UINT _Layer2)
{
	UINT Row = _Layer1;
	UINT Col = _Layer2;

	if (Row > Col)
	{
		Row = _Layer2;
		Col = _Layer1;
	}

	m_Matrix[Row] &= ~(1 << Col);	
}

void CCollisionMgr::SetCollisionMatrix(UINT* _Matrix)
{
	CollisionCheckClear();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_Matrix[i] = _Matrix[i];
	}
}

void CCollisionMgr::CollisionBtwLayer(UINT _Left, UINT _Right)
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	if (nullptr == pCurLevel)
		return;

	const vector<CGameObject*>& vecLeft = pCurLevel->GetLayer(_Left)->GetObjects();
	const vector<CGameObject*>& vecRight = pCurLevel->GetLayer(_Right)->GetObjects();

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		CCollider3D* pLeftCol = vecLeft[i]->Collider3D();
		CBoundingSphere* pLeftSphere = vecLeft[i]->BoundingSphere();

		if (nullptr == pLeftCol || nullptr == pLeftSphere)
		{		
			continue;
		}

		for (size_t j = 0; j < vecRight.size(); ++j)
		{
			CCollider3D* pRightCol = vecRight[j]->Collider3D();
			CBoundingSphere* pRightSphere = vecRight[j]->BoundingSphere();

			if (nullptr == pRightCol || nullptr == pRightSphere)
				continue;

			COLLIDER_ID id = {};

			id.LeftID = pLeftCol->GetID();
			id.RightID = pRightCol->GetID();

			map<ULONGLONG, bool>::iterator iter = m_mapCollisionInfo.find(id.ID);

			// ��ϵ����� ������ ��Ͻ�Ų��.
			if (iter == m_mapCollisionInfo.end())
			{
				m_mapCollisionInfo.insert(make_pair(id.ID, false));
				iter = m_mapCollisionInfo.find(id.ID);
			}

			bool bDead = pLeftCol->GetOwner()->IsDead() || pRightCol->GetOwner()->IsDead();
			//bool bDeactive = !vecLeft[i]->IsActive() || !vecRight[j]->IsActive();

			//if (!CheckBoundingSphere(pLeftSphere, pRightSphere))
			//{
			//	if (iter->second)
			//	{
			//		pLeftCol->EndOverlap(pRightCol);
			//		pRightCol->EndOverlap(pLeftCol);
			//		iter->second = false;
			//	}
			//
			//	continue;
			//}

			// �� �浹ü�� ���� �浹���̴�.
			if (IsCollisionCube(pLeftCol, pRightCol))
			{
				// �������� �浹���̾���.
				if (iter->second)
				{
					pLeftCol->Overlap(pRightCol);
					pRightCol->Overlap(pLeftCol);
				}

				// �������� �浹���� �ƴϾ���.
				else
				{
					pLeftCol->BeginOverlap(pRightCol);
					pRightCol->BeginOverlap(pLeftCol);
				}

				iter->second = true;

				// �� �浹ü�� �ϳ��� Dead ���°ų� ��Ȱ��ȭ ���¶��
				// �߰��� �浹 ������ ȣ������ش�.
				if (bDead) //|| bDeactive)
				{
					pLeftCol->EndOverlap(pRightCol);
					pRightCol->EndOverlap(pLeftCol);
					iter->second = false;
				}
			}

			// �� �浹ü�� ���� �浹���� �ƴϴ�.
			else
			{
				// �������� �浹���̾���.
				if (iter->second)
				{
					pLeftCol->EndOverlap(pRightCol);
					pRightCol->EndOverlap(pLeftCol);
				}

				iter->second = false;
			}
		}
	}
}

#include "CTransform.h"

bool CCollisionMgr::CheckBoundingSphere(CBoundingSphere* _Left, CBoundingSphere* _Right)
{
	// �� ��ü�� ������ ��
	float radius = _Left->GetRadius() + _Right->GetRadius();

	// �� ��ü�� �߽� �� �Ÿ� ���
	Vec3 leftPos = _Left->GetOwner()->Transform()->GetWorldPos();
	Vec3 rightPos = _Right->GetOwner()->Transform()->GetWorldPos();

	// �� �� ���� �Ÿ� ���
	float distance = (leftPos - rightPos).Length();

	// �浹 ���� �Ǵ�
	return distance <= radius;
}

bool CCollisionMgr::IsCollision(CCollider2D* _Left, CCollider2D* _Right)
{
	// �浹ü�� �⺻ ���� ������ ��������, Local Space ���� ���� ������ �����´�.
	Ptr<CMesh> pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh");
	Vtx* pVtx = (Vtx*)pMesh->GetVtxSysMem();
	
	// �� �浹ü�� ���� ����� �����´�.
	const Matrix& matLeft = _Left->GetWorldMatrix();
	const Matrix& matRight = _Right->GetWorldMatrix();
	
	// �⺻ ����(Rect) �� �� �浹ü�� ��������� ���ؼ�, �浹ü�� �� �𼭸� ��ġ�� �ű� ��,
	// ��ǥ���� ��ġ���� ���� �浹ü�� ����󿡼��� ��ġ���� ������ ǥ�� ���⺤�͸� ���Ѵ�.
	// �� ���ʹ� �浹ü���� ������ų ���� �� ����
	// Local Space ���� World Space �� �̵��ؾ� �ϱ� ������ ������ǥ�� 1�� �̵��� ���� �޴� XMVector3Coord �Լ��� ��� �ؼ�
	// ���� Mesh�� LocalSpace �� �浹ü�� ��������� �����ְ� ���༭ ���� ���͸� ����Ѵ�. 
	Vec3 vProjAxis[4] = {};

	vProjAxis[0] = XMVector3TransformCoord(pVtx[3].vPos, matLeft) - XMVector3TransformCoord(pVtx[0].vPos, matLeft);
	vProjAxis[1] = XMVector3TransformCoord(pVtx[1].vPos, matLeft) - XMVector3TransformCoord(pVtx[0].vPos, matLeft);

	vProjAxis[2] = XMVector3TransformCoord(pVtx[3].vPos, matRight) - XMVector3TransformCoord(pVtx[0].vPos, matRight);
	vProjAxis[3] = XMVector3TransformCoord(pVtx[1].vPos, matRight) - XMVector3TransformCoord(pVtx[0].vPos, matRight);

	// �浹ü�� �߽��� �մ� ����
	Vec3 vCenter = XMVector3TransformCoord(Vec3(0.f,0.f,0.f), matLeft) - XMVector3TransformCoord(Vec3(0.f,0.f,0.f), matRight);

	// ����
	for (int i = 0; i < 4; ++i)
	{
		Vec3 vProj = vProjAxis[i];
		vProj.Normalize();

		// ������ ���� Scalar
		float Dot = fabs(vProjAxis[0].Dot(vProj));
		Dot += fabs(vProjAxis[1].Dot(vProj));
		Dot += fabs(vProjAxis[2].Dot(vProj));
		Dot += fabs(vProjAxis[3].Dot(vProj));
		Dot /= 2.f;
		
		float fCenter = fabs(vCenter.Dot(vProj));
		
		if (Dot < fCenter)
		{
			return false;
		}
	}
	
    return true;
}

bool CCollisionMgr::IsCollisionCube(CCollider3D* _Left, CCollider3D* _Right)
{
	// �浹ü�� ���� ���
	const Matrix& matLeft = _Left->GetWorldMatrix();
	const Matrix& matRight = _Right->GetWorldMatrix();

	// �浹ü�� �߽� ��ǥ
	Vec3 centerLeft = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matLeft);
	Vec3 centerRight = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matRight);
	Vec3 vCenter = centerRight - centerLeft;

	// OBB �� (Local X, Y, Z ���� World ��ǥ��� ��ȯ)
	Vec3 leftAxis[3] = {
		XMVector3TransformNormal(Vec3(1.f, 0.f, 0.f), matLeft),
		XMVector3TransformNormal(Vec3(0.f, 1.f, 0.f), matLeft),
		XMVector3TransformNormal(Vec3(0.f, 0.f, 1.f), matLeft)
	};
	Vec3 rightAxis[3] = {
		XMVector3TransformNormal(Vec3(1.f, 0.f, 0.f), matRight),
		XMVector3TransformNormal(Vec3(0.f, 1.f, 0.f), matRight),
		XMVector3TransformNormal(Vec3(0.f, 0.f, 1.f), matRight)
	};

	// AABB �ݰ� (ũ��)
	Vec3 leftHalfSize = _Left->GetHalfSize();  // �� ���� ������
	Vec3 rightHalfSize = _Right->GetHalfSize();

	// SAT �˻�
	for (int i = 0; i < 3; ++i) // Left X, Y, Z �� �˻�
	{
		if (!TestAxis(leftAxis[i], vCenter, leftAxis, rightAxis, leftHalfSize, rightHalfSize))
			return false;
	}

	for (int i = 0; i < 3; ++i) // Right X, Y, Z �� �˻�
	{
		if (!TestAxis(rightAxis[i], vCenter, leftAxis, rightAxis, leftHalfSize, rightHalfSize))
			return false;
	}

	for (int i = 0; i < 3; ++i) // Left X Right X, Left X Right Y
	{
		for (int j = 0; j < 3; ++j)
		{
			Vec3 crossAxis = leftAxis[i].Cross(rightAxis[j]);
			if (!TestAxis(crossAxis, vCenter, leftAxis, rightAxis, leftHalfSize, rightHalfSize))
				return false;
		}
	}

	return true;
}

bool CCollisionMgr::TestAxis(Vec3& axis, const Vec3& vCenter, const Vec3 leftAxis[3], const Vec3 rightAxis[3], const Vec3& leftHalfSize, const Vec3& rightHalfSize)
{
	// ���� ���̰� 0�� ����� ��� �˻� ����
	if (axis.LengthSquared() < 1e-6f)
		return true;

	// �� ����ȭ
	Vec3 normAxis = axis.Normalize();

	// ���� ���� ���
	float leftProjection =
		fabs(leftHalfSize.x * normAxis.Dot(leftAxis[0])) +
		fabs(leftHalfSize.y * normAxis.Dot(leftAxis[1])) +
		fabs(leftHalfSize.z * normAxis.Dot(leftAxis[2]));

	float rightProjection =
		fabs(rightHalfSize.x * normAxis.Dot(rightAxis[0])) +
		fabs(rightHalfSize.y * normAxis.Dot(rightAxis[1])) +
		fabs(rightHalfSize.z * normAxis.Dot(rightAxis[2]));

	float centerProjection = fabs(normAxis.Dot(vCenter));

	// ���� �Ÿ� ��
	return centerProjection <= (leftProjection + rightProjection);
}