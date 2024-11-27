#pragma once
#include "CRenderComponent.h"

#include "CRaycastCS.h"
#include "CHeightMapCS.h"
#include "CWeightMapCS.h"

struct tRaycastOut
{
    Vec2    Location;
    UINT    Distance;
    int     Success;
};

enum LANDSCAPE_MODE
{
    NONE,
    HEIGHTMAP,
    SPLATING,
};

struct tWeight8
{
    float arrWeight[8];
};


class CLandScape :
    public CRenderComponent
{
private:
    int                     m_FaceX;
    int                     m_FaceZ;
    float                   m_TessLevel;

    // Tessellation 
    float                   m_MinLevel;
    float                   m_MaxLevel;
    float                   m_MaxLevelRange;
    float                   m_MinLevelRange;

    // Raycasting
    Ptr<CRaycastCS>         m_RaycastCS;
    CStructuredBuffer*      m_RaycastOut;
    tRaycastOut             m_Out;

    // Brush
    Vec2                    m_BrushScale;
    vector<Ptr<CTexture>>   m_vecBrush;
    UINT                    m_BrushIdx;

    // HeightMap
    Ptr<CTexture>           m_HeightMap;
    bool                    m_IsHeightMapCreated;
    Ptr<CHeightMapCS>       m_HeightMapCS;

    // WeightMap
    Ptr<CTexture>           m_ColorTex;
    Ptr<CTexture>           m_NormalTex;
    CStructuredBuffer*      m_WeightMap;
    UINT                    m_WeightWidth;
    UINT                    m_WeightHeight;
    Ptr<CWeightMapCS>       m_WeightMapCS;
    int                     m_WeightIdx;

    // LandScape ���
    LANDSCAPE_MODE          m_Mode;


public:
    void SetFace(int _X, int _Z);
    void AddBrushTexture(Ptr<CTexture> _BrushTex) { m_vecBrush.push_back(_BrushTex); }
    void SetHeightMap(Ptr<CTexture> _HeightMap) { m_HeightMap = _HeightMap;  m_IsHeightMapCreated = false; }
    void CreateHeightMap(UINT _Width, UINT _Height);
    float GetTessLevel() { return m_TessLevel; }
    void SetTessLevel(float _TessLevel) { m_TessLevel = _TessLevel; }
    void SetMode(LANDSCAPE_MODE _Mode) { m_Mode = _Mode; };

    int GetFaceX() { return m_FaceX; }
    int GetFaceZ() { return m_FaceZ; }
    Ptr<CTexture> GetHeightMap() { return m_HeightMap; }

public:
    virtual void Init() override;
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void CreateMesh();
    void CreateComputeShader();
    void CreateTextureAndStructuredBuffer();
    int Raycasting();

public:
    CLONE(CLandScape);
    CLandScape();
    ~CLandScape();
};

