#pragma once
#include "CComponent.h"

typedef void(*CALL_BACK)(void);
typedef void(CGameObject::*DELEGATE)(void);

class CGameObject;

class CRigidbody :
    public CComponent
{
private:
    Vec3    m_Velocity;             // 속도 ( 방향, 크기 )
    Vec3    m_VelocityByGravity;    // 중력에 의해서 증가하는 속도
    Vec3    m_AddVelocity;          // 최종 속도에 누적될 속도

    float   m_Mass;                 // 질량
    Vec3    m_Force;                // 힘

    float   m_InitWalkSpeed;        // 최소 보장 속력
    float   m_MaxWalkSpeed;         // 최대 제한 속력
    float   m_MaxGravitySpeed;      // 중력으로 발생한 낙하속도 제한

    float   m_Friction;             // 마찰력    

    float   m_GravityAccel;         // 중력 가속도
    bool    m_UseGravity;
    bool    m_Ground;               // 땅 위에 서있는지 체크
    float   m_JumpSpeed;            // 점프 속력


    // Ground On / Off 호출시킬 함수포인터
    // CallBack
    CALL_BACK   m_GroundFunc;
    CALL_BACK   m_AirFunc;

    // Delegate
    CGameObject* m_GroundInst;
    DELEGATE    m_GroundDelegate;

    CGameObject* m_AirInst;
    DELEGATE    m_AirDelegate;

public:
    void AddForce(Vec3 _vForce) { m_Force += _vForce; }

    void SetMass(float _Mass) { m_Mass = _Mass; }
    void SetInitialWalkSpeed(float _Speed) { m_InitWalkSpeed = _Speed; }
    void SetMaxWalkSpeed(float _Speed) { m_MaxWalkSpeed = _Speed; }
    void SetMaxGravitySpeed(float _Speed) { m_MaxGravitySpeed = _Speed; }
    void SetFriction(float _Friction) { m_Friction = _Friction; }
    void SetJumpSpeed(float _Speed) { m_JumpSpeed = _Speed; }

    void Jump();
    void SetGravityVelocity(Vec3 _Velocity) { m_VelocityByGravity = _Velocity; }

    float GetMass() { return m_Mass; }
    float GetInitialWalkSpeed() { return m_InitWalkSpeed; }
    float GetMaxWalkSpeed() { return m_MaxWalkSpeed; }
    float GetFriction() { return m_Friction; }
    Vec3 GetGravityVelocity() { return m_VelocityByGravity; }

    void SetGroundFunc(void (*_pFunc)(void)) { m_GroundFunc = _pFunc; }
    void SetAirFunc(void(*_pFunc)(void)) { m_AirFunc = _pFunc; }

    void SetGroundDelegate(CGameObject* _Inst, DELEGATE _MemFunc)
    {
        m_GroundInst = _Inst;
        m_GroundDelegate = _MemFunc;
    }

    void SetAirDelegate(CGameObject* _Inst, DELEGATE _MemFunc)
    {
        m_AirInst = _Inst;
        m_AirDelegate = _MemFunc;
    }

    void UseGravity(bool _Use)
    {
        m_UseGravity = _Use;
        if (!m_UseGravity)
            m_VelocityByGravity = Vec3(0.f, 0.f, 0.f);
    }

    void SetGround(bool _Ground)
    {
        if (m_Ground == _Ground)
            return;

        m_Ground = _Ground;

        if (m_Ground)
        {
            m_VelocityByGravity = Vec3(0.f, 0.f, 0.f);

            if (nullptr != m_GroundFunc)
                m_GroundFunc();

            if (m_GroundInst && m_GroundDelegate)
                (m_GroundInst->*m_GroundDelegate)();
        }
        else
        {
            if (nullptr != m_AirFunc)
                m_AirFunc();

            if (m_AirInst && m_AirDelegate)
                (m_AirInst->*m_AirDelegate)();
        }
    }

    bool IsGround()            { return m_Ground; }
    bool UseGravity()          { return m_UseGravity; }
    bool IsFall()              { return m_Velocity.Length() < 0.f; }
    float GetJumpSpeed()       { return m_JumpSpeed; }
    float GetMaxGravitySpeed() { return m_MaxGravitySpeed; }

 public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

public:
    virtual void FinalTick() override;

public:
    CLONE(CRigidbody);
    CRigidbody();
    CRigidbody(const CRigidbody& _Origin);
    ~CRigidbody();
};

