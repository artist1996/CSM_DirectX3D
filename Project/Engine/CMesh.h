#pragma once
#include "CAsset.h"

#include "CFBXLoader.h"

class CStructuredBuffer;

struct tIndexInfo
{
    ComPtr<ID3D11Buffer>    pIB;
    D3D11_BUFFER_DESC       tIBDesc;
    UINT				    iIdxCount;
    void* pIdxSysMem;
};

class CMesh :
    public CAsset
{
private:
    ComPtr<ID3D11Buffer>    m_VB;
    D3D11_BUFFER_DESC       m_VBDesc;
    UINT                    m_VtxCount;
    Vtx* m_VtxSysMem;

    // �ϳ��� ���ؽ����ۿ� �������� �ε������۰� ����
    vector<tIndexInfo>		m_vecIdxInfo;

    // Animation3D ����
    vector<tMTAnimClip>		m_vecAnimClip;
    vector<tMTBone>			m_vecBones;

    CStructuredBuffer*      m_pBoneFrameData;    // ��ü �� ������ ����(ũ��, �̵�, ȸ��) (������ ������ŭ)
    CStructuredBuffer*      m_pBoneInverse;	     // �� ���� Inverse ���(�� ���� ��ġ�� �ǵ����� ���) (1�� ¥��)

    vector<CStructuredBuffer*> m_pVecBoneFrameData;


public:
    static CMesh* CreateFromContainer(CFBXLoader& _loader);
    int Create(Vtx* _VtxSysMem, UINT _VtxCount, UINT* _IdxSysMem, UINT _IdxCount);

    UINT GetVertexCount() { return m_VtxCount; }
    UINT GetSubsetCount() { return (UINT)m_vecIdxInfo.size(); }

    void* GetVtxSysMem() { return m_VtxSysMem; }

    const vector<tMTBone>* GetBones() { return &m_vecBones; }
    UINT GetBoneCount() { return (UINT)m_vecBones.size(); }
    const vector<tMTAnimClip>* GetAnimClip() { return &m_vecAnimClip; }
    bool IsAnimMesh() { return !m_vecAnimClip.empty(); }
    CStructuredBuffer* GetBoneFrameDataBuffer() { return m_pBoneFrameData; }    // ��ü �� ������ ����
    CStructuredBuffer* GetBoneInverseBuffer() { return  m_pBoneInverse; }	    // �� Bone �� Inverse ���
    CStructuredBuffer* GetBoneFrameDataBufferByIndex(int _Idx) { return m_pVecBoneFrameData[_Idx]; }

    void Binding(UINT _Subset);
    void Binding_Inst(UINT _iSubset);
    void Render(UINT _Subset);
    void Render_Particle(UINT _Count);
    void Render_Instancing(UINT _iSubset);


    virtual int Load(const wstring& _FilePath) override;
    virtual int Save(const wstring& _FilePath) override;

public:
    CMesh(bool _IsEngine = false);
    ~CMesh();
};


