#pragma once
#include "ComponentUI.h"
class CameraUI :
    public ComponentUI
{
private:
    float m_fHeight;
    bool  m_ShowLayerCheck;

public:
    virtual void Update() override;

private:
    void LayerCheck();
    void Projection();
    

public:
    CameraUI();
    virtual ~CameraUI();
};

