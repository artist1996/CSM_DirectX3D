#pragma once
#include "CComponent.h"
class CLight3D :
    public CComponent
{
private:
    tLightInfo      m_Info;
    int             m_LightIdx;

    Ptr<CMesh>      m_VolumeMesh;
    Ptr<CMaterial>  m_LightMtrl;

    Ptr<CMaterial>  m_ShadowMapMtrl;
    class CMRT*     m_ShadowMapMRT;

    CGameObject*    m_Cam;           // 광원이 보유한 카메라

    Vec3            m_TargetPos;

public:
    void SetLightType(LIGHT_TYPE _Type);
    void SetRadius(float _Radius)            { m_Info.Radius = _Radius; }
    void SetAngle(float _Angle)              { m_Info.Angle = _Angle; }
    void SetLightColor(Vec3 _Color)          { m_Info.light.Color = _Color; }
    void SetLightAmbient(Vec3 _Amb)          { m_Info.light.Ambient = _Amb; }
    void SetSpecularCoefficient(float _Coef) { m_Info.light.SpecCoef = _Coef; }

    LIGHT_TYPE GetLightType() { return m_Info.Type; }
    float GetRadius()         { return m_Info.Radius; }
    float GetAngle()          { return m_Info.Angle; }
    Vec3 GetLightColor()      { return m_Info.light.Color; }
    Vec3 GetLightAmbient()    { return m_Info.light.Ambient; }


    void CreateShadowMap();

    const tLightInfo& GetLightInfo() { return m_Info; }

public:
    virtual void FinalTick() override;
    void Render();

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CLight3D);
    CLight3D();
    CLight3D(const CLight3D& _Origin);
    ~CLight3D();
};

