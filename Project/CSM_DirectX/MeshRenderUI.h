#pragma once
#include "ComponentUI.h"
class MeshRenderUI :
    public ComponentUI
{
private:

public:
    virtual void Update() override;

    void SelectMesh(DWORD_PTR _ListUI);
    void SelectMaterial(DWORD_PTR _ListUI);

public:
    MeshRenderUI();
    virtual ~MeshRenderUI();
};

