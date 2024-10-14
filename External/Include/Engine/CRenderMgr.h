#pragma once

class CCamera;
class CGameObject;
class CLight2D;
class CLight3D;
class CStructuredBuffer;

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

public:
	void AddDebugShapeInfo(tDebugShapeInfo _Info) { m_DebugShapeList.push_back(_Info); }
	void RegisterEditorCamera(CCamera* _Cam)	  { m_EditorCamera = _Cam; }
	void RegisterCamera(CCamera* _Cam, int _CamPriority);
	void RegisterLight2D(CLight2D* _Light) { m_vecLight2D.push_back(_Light); }
	void RegisterLight3D(CLight3D* _Light) { m_vecLight3D.push_back(_Light); }

	void PostProcessCopy();
	void CopyTexture();

	Ptr<CTexture> GetCopyTex() { return m_CopyTex; }

private:
	void RenderDebugShape();
	void RenderStart();
	void Clear();

public:
	void Init();
	void Tick();
};

