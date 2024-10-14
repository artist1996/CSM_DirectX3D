#include "pch.h"
#include "CRigidbody.h"

#include "CTimeMgr.h"

#include "CTransform.h"

CRigidbody::CRigidbody()
	: CComponent(COMPONENT_TYPE::RIGIDBODY)
	, m_Mass(1.f)
	, m_InitWalkSpeed(0.f)
	, m_MaxWalkSpeed(0.f)
	, m_MaxGravitySpeed(500.f)
	, m_Friction(500.f)
	, m_GravityAccel(980.f)
	, m_UseGravity(false)
	, m_Ground(false)
	, m_JumpSpeed(800.f)
	, m_GroundFunc(nullptr)
	, m_AirFunc(nullptr)
	, m_GroundInst(nullptr)
	, m_GroundDelegate(nullptr)
	, m_AirInst(nullptr)
	, m_AirDelegate(nullptr)
{
}

CRigidbody::CRigidbody(const CRigidbody& _Other)
	: CComponent(_Other)
	, m_Mass(_Other.m_Mass)
	, m_InitWalkSpeed(_Other.m_InitWalkSpeed)
	, m_MaxWalkSpeed(_Other.m_MaxWalkSpeed)
	, m_MaxGravitySpeed(_Other.m_MaxGravitySpeed)
	, m_Friction(_Other.m_Friction)
	, m_GravityAccel(_Other.m_GravityAccel)
	, m_UseGravity(_Other.m_UseGravity)
	, m_Ground(_Other.m_Ground)
	, m_JumpSpeed(_Other.m_JumpSpeed)
	, m_GroundFunc(nullptr)
	, m_AirFunc(nullptr)
	, m_GroundInst(nullptr)
	, m_GroundDelegate(nullptr)
	, m_AirInst(nullptr)
	, m_AirDelegate(nullptr)
{
}

CRigidbody::~CRigidbody()
{
}

void CRigidbody::Jump()
{
	m_VelocityByGravity += Vec3(0.f, 1.f, 0.f) * m_JumpSpeed;
	SetGround(false);
}

void CRigidbody::SaveToFile(FILE* _pFile)
{
	fwrite(&m_Mass, sizeof(float), 1, _pFile);
	fwrite(&m_InitWalkSpeed, sizeof(float), 1, _pFile);
	fwrite(&m_MaxWalkSpeed, sizeof(float), 1, _pFile);   
	fwrite(&m_MaxGravitySpeed, sizeof(float), 1, _pFile);
	fwrite(&m_Friction, sizeof(float), 1, _pFile);       
	fwrite(&m_GravityAccel, sizeof(float), 1, _pFile);
	fwrite(&m_UseGravity, sizeof(float), 1, _pFile);
	fwrite(&m_JumpSpeed, sizeof(float), 1, _pFile);
}

void CRigidbody::LoadFromFile(FILE* _pFile)
{
	fread(&m_Mass, sizeof(float), 1, _pFile);
	fread(&m_InitWalkSpeed, sizeof(float), 1, _pFile);
	fread(&m_MaxWalkSpeed, sizeof(float), 1, _pFile);
	fread(&m_MaxGravitySpeed, sizeof(float), 1, _pFile);
	fread(&m_Friction, sizeof(float), 1, _pFile);
	fread(&m_GravityAccel, sizeof(float), 1, _pFile);
	fread(&m_UseGravity, sizeof(float), 1, _pFile);
	fread(&m_JumpSpeed, sizeof(float), 1, _pFile);
}

void CRigidbody::FinalTick()
{
	Vec3 vObjPos = Transform()->GetRelativePos();

	// F = M x A, F / M = A
	Vec3 vAccel = m_Force / m_Mass;

	// 가속도
	// 중력을 사용할때, 공중에서 힘이 적용된 경우
	if (m_UseGravity && !m_Ground)
	{
		m_Velocity += vAccel * DT * 0.5f;
	}
	else
	{
		m_Velocity += vAccel * DT * 1.f;
	}


	// 최대 속도 제한
	if (0.f != m_MaxWalkSpeed && m_MaxWalkSpeed < m_Velocity.Length())
	{
		m_Velocity.Normalize();
		m_Velocity *= m_MaxWalkSpeed;
	}
	
	// 최소 속도 보정
	if (m_InitWalkSpeed != 0.f && m_Velocity.Length() < 10.f && !vAccel.IsZero())
	{
		Vec3 vForceDir = m_Force;
		vForceDir.Normalize();
		m_Velocity = vForceDir * m_InitWalkSpeed;
	}

	// 만약 RigidBody 에 적용된 힘이 없으면서, 속도는 있는경우
	// 마찰에 의해서 현재 속도를 줄인다
	if (m_Force.IsZero())
	{
		float Speed = m_Velocity.Length();

		// 중력기능을 켰고, 공중상태인 경우
		if (m_UseGravity && !m_Ground)
		{
			// 마찰을 더 적게 적용한다.
			Speed -= m_Friction * DT * 0.2f;
		}
		else
		{
			Speed -= m_Friction * DT;
		}

		// 마찰에 의해서 속도가 역으로 생길 수는 없기때문에, 
		// 감소된 속력의 크기가 음수가 되지 않게 보정한다.
		if (Speed < 0)
			Speed = 0.f;

		if (!m_Velocity.IsZero())
			m_Velocity.Normalize();

		m_Velocity *= Speed;
	}

	// 중력 가속도에 의한 속도 증가
	if (m_UseGravity && !m_Ground)
	{
		m_VelocityByGravity += Vec3(0.f, -1.f, 0.f) * m_GravityAccel * DT;
		//m_VelocityByGravity += Vec3(0.f, 0.f, -1.f) * m_GravityAccel * DT;
		if (0.f != m_MaxGravitySpeed && m_MaxGravitySpeed < m_VelocityByGravity.Length())
		{
			m_VelocityByGravity.Normalize();
			m_VelocityByGravity *= m_MaxGravitySpeed;
		}
	}

	// 최종 속도
	Vec3 vFinalVelocity = m_Velocity + m_VelocityByGravity;


	// 현재 속도에 따른 이동
	// 속도 = 거리 / 시간	
	vObjPos += vFinalVelocity * DT;
	Transform()->SetRelativePos(vObjPos);

	// 이번 프레임 힘 초기화
	m_Force = Vec3(0.f, 0.f,0.f);
	m_AddVelocity = Vec3(0.f, 0.f,0.f);

	// DebugRender
	//DrawDebugLine(PEN_TYPE::PEN_BLUE, vObjPos, vObjPos + vFinalVelocity, 0.f);
}