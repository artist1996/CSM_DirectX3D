#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_IndependentScale(false)
{
}

CTransform::~CTransform()
{
}

void CTransform::FinalTick()
{
	// ������Ʈ�� ���� ��� ���
	// ũ�� ���
	Matrix matScale = XMMatrixScaling(m_RelativeScale.x, m_RelativeScale.y, m_RelativeScale.z);
	
	// �̵� ���
	Matrix matTranslation = XMMatrixTranslation(m_RelativePos.x, m_RelativePos.y, m_RelativePos.z);

	// ȸ�� ���
	Matrix matRot = XMMatrixRotationX(m_RelativeRotation.x)
				  * XMMatrixRotationY(m_RelativeRotation.y)
				  * XMMatrixRotationZ(m_RelativeRotation.z);

	m_matWorld = matScale * matRot * matTranslation;

	static Vec3 vDefaultAxis[3] = 	
	{
		Vec3(1.f, 0.f, 0.f),
		Vec3(0.f, 1.f, 0.f),
		Vec3(0.f, 0.f, 1.f),
	};

	// HLSL mul
	// w�� 1�� Ȯ��
	
	for (int i = 0; i < 3; ++i)
	{
		m_RelativeDir[i] = XMVector3TransformNormal(vDefaultAxis[i], matRot);
		m_RelativeDir[i].Normalize();
	}

	// �θ� ������Ʈ�� �ִ��� Ȯ��
	if (GetOwner()->GetParent())
	{
		// �θ��� ��������� ���ؼ� ���� ��������� �����
		const Matrix& matParentWorldMat = GetOwner()->GetParent()->Transform()->GetWorldMatrix();
		
		if (m_IndependentScale)
		{ 
			// �θ��� ũ�� ����� ������� ���ؼ� �θ��� ũ�⿡ ������� ũ�⸦ ���� �ʰ� �������� ũ�⸦ ���� ���ش�.
			Vec3 vParentScale = GetOwner()->GetParent()->Transform()->GetRelativeScale();
			Matrix matParentScale = XMMatrixScaling(vParentScale.x, vParentScale.y, vParentScale.z);
			Matrix matParentScaleInv = XMMatrixInverse(nullptr, matParentScale);

			m_matWorld = m_matWorld * matParentScaleInv * matParentWorldMat;
			//m_matWorld = m_matWorld * matParentWorldMat * matParentScaleInv;
		}

		else
		{
			m_matWorld *= matParentWorldMat;
		}
		// ���� ������� ������Ʈ�� ���⺤�͸� ����
		for (int i = 0; i < 3; ++i)
		{
			m_WorldDir[i] = XMVector3TransformNormal(vDefaultAxis[i], m_matWorld);
			m_WorldDir[i].Normalize();

			if (L"Player" == GetOwner()->GetName())
				int a = 0;
		}
	}

	// �θ� ������ RelativeDir �� �� World Dir
	else
	{
		for (int i = 0; i < 3; ++i)
		{
			m_WorldDir[i] = m_RelativeDir[i];
		}
	}
}

Vec3 CTransform::GetWorldScale()
{
	Vec3 vWorldScale = Vec3(1.f, 1.f, 1.f);

	CGameObject* pObject = GetOwner();

	while (pObject)
	{
		vWorldScale *= pObject->Transform()->GetRelativeScale();

		// ���� �������� ũ�⸦ ���� Object ��� break ������Ѵ�.
		if (pObject->Transform()->m_IndependentScale)
			break;

		pObject = GetOwner()->GetParent();
	}

	return vWorldScale;
}

void CTransform::Binding()
{
	g_Trans.matWorld = m_matWorld;
	g_Trans.matWV = g_Trans.matWorld * g_Trans.matView;
	g_Trans.matWVP = g_Trans.matWV * g_Trans.matProj;

	CConstBuffer* pTransformCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	pTransformCB->SetData(&g_Trans);
	pTransformCB->Binding();
}

void CTransform::SaveToFile(FILE* _pFile)
{	
	fwrite(&m_RelativePos, sizeof(Vec3), 1, _pFile);
	fwrite(&m_RelativeScale, sizeof(Vec3), 1, _pFile);
	fwrite(&m_RelativeRotation, sizeof(Vec3), 1, _pFile);
	fwrite(&m_IndependentScale, sizeof(bool), 1, _pFile);
}

void CTransform::LoadFromFile(FILE* _pFile)
{
	fread(&m_RelativePos, sizeof(Vec3), 1, _pFile);
	fread(&m_RelativeScale, sizeof(Vec3), 1, _pFile);
	fread(&m_RelativeRotation, sizeof(Vec3), 1, _pFile);
	fread(&m_IndependentScale, sizeof(bool), 1, _pFile);
}