#include "pch.h"

#include "CRenderMgr.h"
#include "CTaskMgr.h"
#include "CGameObject.h"

void CreateObject(CGameObject* _NewObject, int _LayerIndex)
{
	tTask Task = {};

	Task.Type = TASK_TYPE::CREATE_OBJECT;
	Task.Param_0 = _LayerIndex;
	Task.Param_1 = (DWORD_PTR)_NewObject;
	
	CTaskMgr::GetInst()->AddTask(Task);
}

void DeleteObject(CGameObject* _DeleteObject)
{
	tTask Task = {};
	Task.Type = TASK_TYPE::DELETE_OBJECT;
	Task.Param_0 = (DWORD_PTR)_DeleteObject;

	CTaskMgr::GetInst()->AddTask(Task);
}

void DisconnectObject(CGameObject* _DisconnetObject)
{
	tTask Task = {};
	Task.Type = TASK_TYPE::DISCONNECT_LAYER;
	Task.Param_0 = (DWORD_PTR)_DisconnetObject;

	CTaskMgr::GetInst()->AddTask(Task);
}

void ChangeLevelState(LEVEL_STATE _State)
{
	tTask Task = {};
	Task.Type = TASK_TYPE::CHANGE_LEVELSTATE;
	Task.Param_0 = (DWORD_PTR)_State;

	CTaskMgr::GetInst()->AddTask(Task);
}

void ChangeLevel(CLevel* _NextLevel, LEVEL_STATE _NextLevelState)
{
	tTask Task = {};
	Task.Type = TASK_TYPE::CHANGE_LEVEL;
	Task.Param_0 = (DWORD_PTR)_NextLevel;
	Task.Param_1 = (DWORD_PTR)_NextLevelState;

	CTaskMgr::GetInst()->AddTask(Task);
}

bool IsValid(CGameObject*& _Object)
{
	if (nullptr == _Object)
		return false;
	if (_Object->IsDead())
	{
		_Object = nullptr;
		return false;
	}
	
	return true;
}

void DrawDebugRect(Vec3 _Pos, Vec3 _Scale, Vec3 _Rot, Vec4 _Color, float _Life, bool _DepthTest)
{
	tDebugShapeInfo Info = {};

	Info.Shape = DEBUG_SHAPE::RECT;
	Info.vPos = _Pos;
	Info.vScale = _Scale;
	Info.vRot = _Rot;
	Info.vColor = _Color;
	Info.LifeTime = _Life;
	Info.DepthTest = _DepthTest;
	
	// World Space 행렬 계산
	Info.matWorld = XMMatrixScaling(_Scale.x, _Scale.y, _Scale.z)
				  * XMMatrixRotationX(_Rot.x)
				  * XMMatrixRotationY(_Rot.y)
				  * XMMatrixRotationZ(_Rot.z)
				  * XMMatrixTranslation(_Pos.x, _Pos.y, _Pos.z);
	
	CRenderMgr::GetInst()->AddDebugShapeInfo(Info);
}

void DrawDebugRect(Matrix _matWorld, Vec4 _Color, float _Life, bool _DepthTest)
{
	tDebugShapeInfo Info = {};

	Info.Shape = DEBUG_SHAPE::RECT;
	Info.matWorld = _matWorld;
	Info.vColor = _Color;
	Info.LifeTime = _Life;
	Info.DepthTest = _DepthTest;

	CRenderMgr::GetInst()->AddDebugShapeInfo(Info);
}

void DrawDebugCircle(Vec3 _Pos, float _Radius, Vec4 _Color, float _Life, bool _DepthTest)
{
	tDebugShapeInfo Info = {};

	Info.Shape = DEBUG_SHAPE::CIRCLE;
	Info.vPos = _Pos;
	Info.vScale = Vec3(_Radius * 2.f, _Radius * 2.f, 1.f);
	Info.vRot = Vec3(0.f, 0.f, 0.f);
	Info.LifeTime = _Life;

	Info.matWorld = XMMatrixScaling(Info.vScale.x, Info.vScale.y, Info.vScale.z)
				  * XMMatrixTranslation(_Pos.x, _Pos.y, _Pos.z);
					
	Info.vColor = _Color;
	Info.DepthTest = _DepthTest;

	CRenderMgr::GetInst()->AddDebugShapeInfo(Info);
}

void SaveWString(const wstring& _String, FILE* _pFile)
{
	size_t len = _String.length();
	fwrite(&len, sizeof(size_t), 1, _pFile);
	fwrite(_String.c_str(), sizeof(wchar_t), len, _pFile);
}

void LoadWString(wstring& _String, FILE* _pFile)
{
	size_t len = 0;

	fread(&len, sizeof(size_t), 1, _pFile);
	_String.resize(len);
	fread((wchar_t*)_String.c_str(), sizeof(wchar_t), len, _pFile);
}
