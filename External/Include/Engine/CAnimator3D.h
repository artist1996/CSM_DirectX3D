#pragma once
#include "CComponent.h"

class CStructuredBuffer;

class CAnimator3D :
    public CComponent
{
private:
    const vector<tMTBone>*      m_pVecBones;
    const vector<tMTAnimClip>*  m_pVecClip;

    vector<float>				m_vecClipUpdateTime;
    vector<Matrix>				m_vecFinalBoneMat;      // �ؽ��Ŀ� ������ ���� �������
    int							m_iFrameCount;          // 30
    double						m_dCurTime;
    int							m_iCurClip;             // Ŭ�� �ε���	

    int							m_iFrameIdx;            // Ŭ���� ���� ������
    int							m_iNextFrameIdx;        // Ŭ���� ���� ������
    float						m_fRatio;	            // ������ ���� ����
    
    CStructuredBuffer*          m_pBoneFinalMatBuffer;  // Ư�� �������� ���� ���
    bool						m_bFinalMatUpdate;      // ������� ���� ���࿩��

public:
    void SetBones(const vector<tMTBone>* _vecBones) { m_pVecBones = _vecBones; m_vecFinalBoneMat.resize(m_pVecBones->size()); }
    void SetClipIndex(int _Idx)                     { m_iCurClip = _Idx; }
    void SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip);
    void SetClipTime(int _iClipIdx, float _fTime)   { m_vecClipUpdateTime[_iClipIdx] = _fTime; }
   

    CStructuredBuffer* GetFinalBoneMat()            { return m_pBoneFinalMatBuffer; }
    const Matrix& GetBoneMat()                      { return m_vecFinalBoneMat[m_iCurClip]; }
    UINT GetBoneCount()                             { return (UINT)m_pVecBones->size(); }
    void ClearData();                               
                                                    
    void Binding();                                 
                                                    
public:                                             
    const vector<tMTAnimClip>* GetCurClip()         { return m_pVecClip; }
    int GetFrameIdx()                               { return m_iFrameIdx; }
    int GetNextFrameIdx()                           { return m_iNextFrameIdx; }
    int GetCurClipIdx()                             { return m_iCurClip; }
    float GetFrameRatio()                           { return m_fRatio; }

private:
    void check_mesh(Ptr<CMesh> _pMesh);

public:
    virtual void FinalTick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CAnimator3D);
    CAnimator3D();
    CAnimator3D(const CAnimator3D& _Other);
    ~CAnimator3D();
};