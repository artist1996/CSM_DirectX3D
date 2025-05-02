#pragma once

class CCamera;
class CGameObject;
class CLight2D;
class CLight3D;
class CStructuredBuffer;
class CMRT;
class CDownScaleCS;
class CThresholdCS;
class CVerticalBlurCS;
class CHorizontalBlurCS;
class CUpScaleCS;
class CVolumetricLightCS;

class CRenderMgr
	: public CSingleton<CRenderMgr>
{
	SINGLE(CRenderMgr);
private:
	vector<CCamera*>		m_vecCam;
	CCamera*				m_EditorCamera;

	list<tDebugShapeInfo>	m_DebugShapeList;

	CGameObject*			m_DebugObject;

	vector<CLight2D*>		m_vecLight2D;
	CStructuredBuffer*	    m_Light2DBuffer;

	vector<CLight3D*>		m_vecLight3D;
	CStructuredBuffer*	    m_Light3DBuffer;

	Ptr<CTexture>			m_PostProcessTex;
	Ptr<CTexture>			m_CopyTex;

	CMRT*					m_arrMRT[(UINT)MRT_TYPE::END];

	Ptr<CMesh>			    m_RectMesh;
	Ptr<CMaterial>			m_MergeMtrl;

	// Bloom
	Ptr<CDownScaleCS>		m_DownScaleCS;
	Ptr<CThresholdCS>		m_ThresholdCS;
	Ptr<CVerticalBlurCS>	m_VerticalBlurCS;
	Ptr<CHorizontalBlurCS>  m_HorizontalBlurCS;
	Ptr<CUpScaleCS>			m_UpScaleCS;

	Ptr<CVolumetricLightCS> m_VolumetricCS;
	
	int						m_RenderBloom;

public:
	void AddDebugShapeInfo(tDebugShapeInfo _Info) { m_DebugShapeList.push_back(_Info); }
	void RegisterEditorCamera(CCamera* _Cam)	  { m_EditorCamera = _Cam; }
	void RegisterCamera(CCamera* _Cam, int _CamPriority);
	void RegisterLight2D(CLight2D* _Light)		  { m_vecLight2D.push_back(_Light); }
	int RegisterLight3D(CLight3D* _Light)	      { m_vecLight3D.push_back(_Light); return (int)m_vecLight3D.size() - 1; }
 
	void PostProcessCopy();
	void CopyTexture();

	int GetUseBloom() { return m_RenderBloom; }
	void SetUseBloom(int _use) { m_RenderBloom = _use; }

	Ptr<CTexture> GetCopyTex()	 { return m_CopyTex; }

	CMRT* GetMRT(MRT_TYPE _Type) { return m_arrMRT[(UINT)_Type]; }
	CCamera* GetPOVCam();

private:
	void RenderStart();
	void Render(CCamera* _Cam);
	void Render_Sub(CCamera* _Cam);

	void Clear();
	void RenderDebugShape();
	void CreateMRT();
	void ClearMRT();
	void CreateMaterial();
	
	void ClearOMSet(CCamera* _Cam);

	// Bloom
	void Render_Bloom();
	void Render_Threshold();
	void Render_DownScale();
	void Render_Blur();
	void Render_UpScale();

	// Volumetric
	void Render_VolumetricLighting();

	void CreateComputeShader();

public:
	void Init();
	void Tick();
};

