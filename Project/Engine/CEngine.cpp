#include "pch.h"
#include "CEngine.h"
#include "CDevice.h"
#include "CPathMgr.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CAssetMgr.h"
#include "CLevelMgr.h"
#include "CRenderMgr.h"
#include "CCollisionMgr.h"
#include "CTaskMgr.h"
#include "CFontMgr.h"
#include "CPrefab.h"

CEngine::CEngine()
	: m_hWnd(nullptr)
	, m_ptResolution{}
	, m_FMODSystem(nullptr)
{}

CEngine::~CEngine()
{
	if (nullptr != m_FMODSystem)
	{
		m_FMODSystem->release();
		m_FMODSystem = nullptr;
	}
}

int CEngine::Init(HWND _hWnd, POINT _ptResolution, OBJECT_SAVE _SaveFunc, OBJECT_LOAD _LoadFunc)
{
	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;

	ChangeWindowScale(m_ptResolution.x, m_ptResolution.y);

	// Device �ʱ�ȭ
	if (FAILED(CDevice::GetInst()->Init(m_hWnd, m_ptResolution.x, m_ptResolution.y)))
	{
		MessageBox(nullptr, L"��ġ �ʱ�ȭ ����", L"CDevice �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	// FMOD �ʱ�ȭ
	FMOD::System_Create(&m_FMODSystem);
	assert(m_FMODSystem);

	// 32�� ä�� ����
	m_FMODSystem->init(100, FMOD_DEFAULT, nullptr);

	// Manager Init

	CPathMgr::GetInst()->Init();
	CKeyMgr::GetInst()->Init();
	CTimeMgr::GetInst()->Init();
	CAssetMgr::GetInst()->Init();
	CLevelMgr::GetInst()->Init();
	CRenderMgr::GetInst()->Init();
	CFontMgr::GetInst()->Init();

	// Prefab Function ���
	CPrefab::g_ObjectSaveFunc = _SaveFunc;
	CPrefab::g_ObjectLoadFunc = _LoadFunc;

	//CObjectPoolMgr::GetInst()->Init();

	return S_OK;
}

void CEngine::Progress()
{
	m_FMODSystem->update();

	// Manager
	CKeyMgr::GetInst()->Tick();
	CTimeMgr::GetInst()->Tick();
	CAssetMgr::GetInst()->Tick();
	CLevelMgr::GetInst()->Progress();

	// Collision
	CCollisionMgr::GetInst()->Tick();

	// Render
	CRenderMgr::GetInst()->Tick();

	// TaskMgr
	CTaskMgr::GetInst()->Tick();
}

void CEngine::ChangeWindowScale(UINT _Width, UINT _Height)
{
	// Create Window and Change Window Size
	bool bMenu = false;

	if (GetMenu(m_hWnd))
		bMenu = true;
	
	RECT rt = { 0,0, (LONG)_Width, (LONG)_Height };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
}