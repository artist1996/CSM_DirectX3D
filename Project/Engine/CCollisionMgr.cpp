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

			// 등록된적이 없으면 등록시킨다.
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

			// 두 충돌체가 지금 충돌중이다.
			if (IsCollisionCube(pLeftCol, pRightCol))
			{
				// 이전에도 충돌중이었다.
				if (iter->second)
				{
					pLeftCol->Overlap(pRightCol);
					pRightCol->Overlap(pLeftCol);
				}

				// 이전에는 충돌중이 아니었다.
				else
				{
					pLeftCol->BeginOverlap(pRightCol);
					pRightCol->BeginOverlap(pLeftCol);
				}

				iter->second = true;

				// 두 충돌체중 하나라도 Dead 상태거나 비활성화 상태라면
				// 추가로 충돌 해제를 호출시켜준다.
				if (bDead) //|| bDeactive)
				{
					pLeftCol->EndOverlap(pRightCol);
					pRightCol->EndOverlap(pLeftCol);
					iter->second = false;
				}
			}

			// 두 충돌체가 지금 충돌중이 아니다.
			else
			{
				// 이전에는 충돌중이었다.
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
	// 두 구체의 반지름 합
	float radius = _Left->GetRadius() + _Right->GetRadius();

	// 두 구체의 중심 간 거리 계산
	Vec3 leftPos = _Left->GetOwner()->Transform()->GetWorldPos();
	Vec3 rightPos = _Right->GetOwner()->Transform()->GetWorldPos();

	// 두 점 간의 거리 계산
	float distance = (leftPos - rightPos).Length();

	// 충돌 여부 판단
	return distance <= radius;
}

bool CCollisionMgr::IsCollision(CCollider2D* _Left, CCollider2D* _Right)
{
	// 충돌체의 기본 원형 도형을 가져오고, Local Space 상의 정점 정보를 가져온다.
	Ptr<CMesh> pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh");
	Vtx* pVtx = (Vtx*)pMesh->GetVtxSysMem();
	
	// 각 충돌체의 월드 행렬을 가져온다.
	const Matrix& matLeft = _Left->GetWorldMatrix();
	const Matrix& matRight = _Right->GetWorldMatrix();
	
	// 기본 도형(Rect) 를 각 충돌체의 월드행렬을 곱해서, 충돌체의 각 모서리 위치로 옮긴 후,
	// 좌표끼리 위치값을 빼서 충돌체의 월드상에서의 위치에서 도형의 표면 방향벡터를 구한다.
	// 이 벡터는 충돌체들을 투영시킬 축이 될 예정
	// Local Space 에서 World Space 로 이동해야 하기 때문에 동차좌표를 1로 이동을 적용 받는 XMVector3Coord 함수를 사용 해서
	// 원래 Mesh의 LocalSpace 에 충돌체의 월드행렬을 곱해주고 빼줘서 방향 벡터를 계산한다. 
	Vec3 vProjAxis[4] = {};

	vProjAxis[0] = XMVector3TransformCoord(pVtx[3].vPos, matLeft) - XMVector3TransformCoord(pVtx[0].vPos, matLeft);
	vProjAxis[1] = XMVector3TransformCoord(pVtx[1].vPos, matLeft) - XMVector3TransformCoord(pVtx[0].vPos, matLeft);

	vProjAxis[2] = XMVector3TransformCoord(pVtx[3].vPos, matRight) - XMVector3TransformCoord(pVtx[0].vPos, matRight);
	vProjAxis[3] = XMVector3TransformCoord(pVtx[1].vPos, matRight) - XMVector3TransformCoord(pVtx[0].vPos, matRight);

	// 충돌체의 중심을 잇는 벡터
	Vec3 vCenter = XMVector3TransformCoord(Vec3(0.f,0.f,0.f), matLeft) - XMVector3TransformCoord(Vec3(0.f,0.f,0.f), matRight);

	// 투영
	for (int i = 0; i < 4; ++i)
	{
		Vec3 vProj = vProjAxis[i];
		vProj.Normalize();

		// 내적의 값은 Scalar
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
	// 충돌체의 월드 행렬
	const Matrix& matLeft = _Left->GetWorldMatrix();
	const Matrix& matRight = _Right->GetWorldMatrix();

	// 충돌체의 중심 좌표
	Vec3 centerLeft = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matLeft);
	Vec3 centerRight = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matRight);
	Vec3 vCenter = centerRight - centerLeft;

	// OBB 축 (Local X, Y, Z 축을 World 좌표계로 변환)
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

	// AABB 반경 (크기)
	Vec3 leftHalfSize = _Left->GetHalfSize();  // 각 축의 반지름
	Vec3 rightHalfSize = _Right->GetHalfSize();

	// SAT 검사
	for (int i = 0; i < 3; ++i) // Left X, Y, Z 축 검사
	{
		if (!TestAxis(leftAxis[i], vCenter, leftAxis, rightAxis, leftHalfSize, rightHalfSize))
			return false;
	}

	for (int i = 0; i < 3; ++i) // Right X, Y, Z 축 검사
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
	// 축의 길이가 0에 가까운 경우 검사 생략
	if (axis.LengthSquared() < 1e-6f)
		return true;

	// 축 정규화
	Vec3 normAxis = axis.Normalize();

	// 투영 길이 계산
	float leftProjection =
		fabs(leftHalfSize.x * normAxis.Dot(leftAxis[0])) +
		fabs(leftHalfSize.y * normAxis.Dot(leftAxis[1])) +
		fabs(leftHalfSize.z * normAxis.Dot(leftAxis[2]));

	float rightProjection =
		fabs(rightHalfSize.x * normAxis.Dot(rightAxis[0])) +
		fabs(rightHalfSize.y * normAxis.Dot(rightAxis[1])) +
		fabs(rightHalfSize.z * normAxis.Dot(rightAxis[2]));

	float centerProjection = fabs(normAxis.Dot(vCenter));

	// 투영 거리 비교
	return centerProjection <= (leftProjection + rightProjection);
}