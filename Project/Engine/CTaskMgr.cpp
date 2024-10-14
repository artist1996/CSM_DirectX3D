#include "pch.h"
#include "CTaskMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"

#include "CAssetMgr.h"

CTaskMgr::CTaskMgr()
{}

CTaskMgr::~CTaskMgr()
{
}

void CTaskMgr::Tick()
{
	ClearGC();

	ExecuteTask();
}

void CTaskMgr::ClearGC()
{
	Delete_Vec(m_GC);
}

void CTaskMgr::ExecuteTask()
{
	for (size_t i = 0; i < m_vecTask.size(); ++i)
	{
		tTask& Task = m_vecTask[i];

		switch (Task.Type)
		{
		case TASK_TYPE::CREATE_OBJECT:
		{
			CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			int Layer = (int)m_vecTask[i].Param_0;
			CGameObject* pObj = (CGameObject*)m_vecTask[i].Param_1;
			pCurLevel->AddObject(Layer, pObj);
			
			if (STOP != pCurLevel->GetState())
				pObj->Begin();
		}
		break;
		case TASK_TYPE::DELETE_OBJECT:
		{
			CGameObject* pObject = (CGameObject*)m_vecTask[i].Param_0;
			if (pObject->m_Dead)
			{
				continue;
			}
		
			// GC ¿¡ ³Ö¾îÁÜ
			pObject->m_Dead = true;
			m_GC.push_back(pObject);

			CLayer* pLayer = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(pObject->GetLayerIdx());

			pLayer->DeregisterObject(pObject);
		
			CLevelMgr::GetInst()->m_LevelChanged = true;
		}
		break;

		case TASK_TYPE::DELETE_ASSET:
		{
			CAsset* pAsset = (CAsset*)m_vecTask[i].Param_0;
		 	ASSET_TYPE Type = pAsset->GetAssetType();

			CAssetMgr::GetInst()->DeleteAsset(Type, pAsset->GetKey());
		}
		break;

		case TASK_TYPE::CHANGE_LEVELSTATE:
		{
			LEVEL_STATE NextState = (LEVEL_STATE)m_vecTask[i].Param_0;
			CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			pCurLevel->ChangeLevelState(NextState);
		}
		break;

		case TASK_TYPE::CHANGE_LEVEL:
		{
			CLevel* pLevel = (CLevel*)m_vecTask[i].Param_0;
			LEVEL_STATE NextState = (LEVEL_STATE)m_vecTask[i].Param_1;

			CLevelMgr::GetInst()->ChangeLevel(pLevel);
			pLevel->ChangeLevelState(NextState);
		}
		break;
		case TASK_TYPE::ASSET_CHANGED:
		{
			CAssetMgr::GetInst()->m_Changed = true;
		}
		break;

		case TASK_TYPE::LEVEL_CHANGED:
		{
			CLevelMgr::GetInst()->m_LevelChanged = true;
		}
		break;
		}
	}

	m_vecTask.clear();	
}