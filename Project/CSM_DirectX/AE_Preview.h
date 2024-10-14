#pragma once
#include "AE_Sub.h"
class AE_Preview :
    public AE_Sub
{
private:
    Ptr<CSprite>    m_Sprite;

    int             m_CurFrmIdx;

    float           m_FPS;
    float           m_AccTime;

    bool            m_Play;
    bool            m_Finish;

private:
    void Reset();
    void Preview();

public:
    void SetFPS(float _FPS) { m_FPS = _FPS; }
    void Play()             { m_Play = true; }
    void Stop()             { m_Play = false; }

public:
    virtual void Init() override;
    virtual void Update() override;

    virtual void Deactivate() override;

public:
    AE_Preview();
    virtual ~AE_Preview();
};

