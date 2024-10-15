#pragma once
#include "CRenderComponent.h"

enum class SKYBOX_TYPE
{
    SPHERE,
    CUBE,

    END,
};

class CSkyBox :
    public CRenderComponent
{
private:
    SKYBOX_TYPE   m_Type;
    Ptr<CTexture> m_SkyBoxTex;

public:
    void SetSkyBoxType(SKYBOX_TYPE _Type);
    void SetSkyBoxTexture(Ptr<CTexture> _Tex) { m_SkyBoxTex = _Tex; }

public:
    SKYBOX_TYPE GetSkyBoxType()      { return m_Type; }
    Ptr<CTexture> GetSkyBoxTexture() { return m_SkyBoxTex; }

public:
    virtual void FinalTick() override;
    virtual void Render()    override;

public:
    virtual void SaveToFile(FILE* _pFile)   override;
    virtual void LoadFromFile(FILE* _pFile) override;

public:
    CLONE(CSkyBox);
    CSkyBox();
    virtual ~CSkyBox();
};

