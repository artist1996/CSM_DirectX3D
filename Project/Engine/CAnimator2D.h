#pragma once
#include "CComponent.h"

#include "CAnimation.h"

class CAnimator2D :
    public CComponent
{
private:
    vector<Ptr<CAnimation>>   m_vecAnimation;  // Animator2D 컴포넌트가 보유한 모든 FlipBook 목록
    Ptr<CAnimation>           m_CurAnimation;  // 현재 재생중인 FlipBook
    Ptr<CSprite>              m_CurFrmSprite; // 현재 재생중인 FlipBook 에서 현재 프레임 인덱스에 해당하는 스프라이트

    int                       m_CurFrmIdx;    // 현재 재생중인 FlibBook 에서 몇번 째 Sprite 가 재생중인지 인덱스 기록
    int                       m_MaxFrm;       // 현재 재생중인 FlipBook 의 최대 프레임

    float                     m_FPS;          // 현재 재생중인 FlipBook 의 초당 프레임 진행 수
    float                     m_AccTime;      // 누적 시간값 체크

    bool                      m_Repeat;       // 반복 재생 여부
    bool                      m_Finish;       // FlipBook 재생이 끝에 도달했는지 여부

public:
    void AddAnimation(int _Idx, Ptr<CAnimation> _FlipBook);
    Ptr<CAnimation> FindAnimation(const wstring& _Key);
    void Play(int _FlipBookIdx, float _FPS, bool _Repeat);
    void Stop();
    void Reset();
    void Binding();
    
    void erase(int _Idx);
    
    void SetCurAnimation(Ptr<CAnimation> _FlipBook) { m_CurAnimation = _FlipBook; }
    void SetFPS(float _FPS) { m_FPS = _FPS; }
    float GetFPS()          { return m_FPS; }
    bool IsFinish()         { return m_Finish; }

    static void Clear();

public:
    Ptr<CSprite>   GetCurSprite()                   { return m_CurFrmSprite; }
    Ptr<CAnimation> GetAnimationByIndex(int _Idx)   { return m_vecAnimation[_Idx]; }
    int            GetAnimationsSize()              { return (int)m_vecAnimation.size(); }
    Ptr<CAnimation> GetCurAnimation()               { return m_CurAnimation; }
    const vector<Ptr<CAnimation>> GetAnimations()   { return m_vecAnimation; }

    int GetCurFrameIndex() { return m_CurFrmIdx; }

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

public:
    virtual void FinalTick() override;

public:
    CLONE(CAnimator2D);
    CAnimator2D();
    CAnimator2D(const CAnimator2D& _Origin);
    virtual ~CAnimator2D();
};

