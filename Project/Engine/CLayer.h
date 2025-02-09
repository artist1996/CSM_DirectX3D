#pragma once
#include "CEntity.h"

class CGameObject;

class CLayer :
    public CEntity
{
private:
    vector<CGameObject*> m_Parents;     // 최상위 부모 Object 만 넣어줄 vector
    vector<CGameObject*> m_Objects;     // 부모 자식 Object 만 넣어줄 vector
    const int            m_LayerIdx;
    
public:
    int GetLayerIdx()                              { return m_LayerIdx; }
    void AddObject(CGameObject* _Object, bool _bMoveChild);
    const vector<CGameObject*>& GetParentObjects() { return m_Parents; }
    const vector<CGameObject*>& GetObjects()       { return m_Objects; }

    void RegisterGameObject(CGameObject* _Object) { m_Objects.push_back(_Object); }
    void ClearObject()                            { m_Objects.clear(); }

    void RegisterAsParent(CGameObject* _Object);

    void DisconnectWithObject(CGameObject* _Object);

    void DeregisterObjectAsParent(CGameObject* _Object);
    void DeregisterObject(CGameObject* _Object);

public:
    void Begin();
    void Tick();
    void FinalTick();

public:
    virtual CLayer* Clone() { return new CLayer(*this); }
    CLayer(int _LayerIdx);
    CLayer(const CLayer& _Origin);
    virtual ~CLayer();
};