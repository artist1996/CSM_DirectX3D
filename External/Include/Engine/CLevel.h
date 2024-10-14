#pragma once
#include "CEntity.h"

class CLayer;
class CGameObject;

class CLevel :
    public CEntity
{
private:
    CLayer*      m_Layer[MAX_LAYER];
    UINT         m_Matrix[MAX_LAYER];
    LEVEL_STATE  m_State;

    Ptr<CSound>  m_BGM;

public:
    void AddObject(int _LayerIdx, CGameObject* _Object, bool _bMoveChild = false);
    CLayer* GetLayer(int _LayderIdx) { return m_Layer[_LayderIdx]; }
    LEVEL_STATE GetState()           { return m_State; }

    CGameObject* FindObjectByName(const wstring& _strName);

    void RegisterAsParent(int _LayerIdx, CGameObject* _Object);
    void ClearObject();

    void SetCollisionMatrix(UINT* _Matrix);
    UINT GetCollisionMatrix(UINT _Idx) { return m_Matrix[_Idx]; }

    void CollisionCheck(UINT _Layer1, UINT _Layer2);
    void CollisionUnCheck(UINT _Layer1, UINT _Layer2);

    void SetBGM(Ptr<CSound> _BGM)    { m_BGM = _BGM; }
    Ptr<CSound> GetBGM()             { return m_BGM; }
    
private:
    void ChangeLevelState(LEVEL_STATE _NextState);

public:
    virtual void Enter() {}
    virtual void Begin();
    virtual void Tick();
    virtual void FinalTick();
    virtual void Exit();
    
public:
    virtual CLevel* Clone() { return new CLevel(*this); }
    CLevel();
    CLevel(const CLevel& _Origin);
    virtual ~CLevel();

    friend class CTaskMgr;
};