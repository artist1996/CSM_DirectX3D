#pragma once
#include "CRenderComponent.h"

class CDecal :
    public CRenderComponent
{
private:
    Ptr<CTexture>   m_DecalTex;
    Ptr<CTexture>   m_EmissiveTex;

public:
    void SetDecalTexture(Ptr<CTexture> _Tex)    { m_DecalTex = _Tex; }
    void SetEmissiveTexture(Ptr<CTexture> _Tex) { m_EmissiveTex = _Tex; }

public:
    Ptr<CTexture> GetDecalTexture()     { return m_DecalTex; }
    Ptr<CTexture> GetEmisiiveTexture()  { return m_EmissiveTex; }

public:
    virtual void FinalTick() override;
    virtual void Render() override;

public:
    virtual void SaveToFile(FILE* _File) override;   // ���Ͽ� ������Ʈ�� ������ ����
    virtual void LoadFromFile(FILE* _File) override; // ���Ͽ� ������Ʈ�� ������ �ε�

public:
    CLONE(CDecal);
    CDecal();
    ~CDecal();
};