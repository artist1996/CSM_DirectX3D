#include "pch.h"
#include "CCamera.h"

#include "CDevice.h"
#include "CAssetMgr.h"
#include "CRenderMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"

#include "CRenderComponent.h"

#include "CTransform.h"
#include "CCollider2D.h"
#include "CBoundingBox.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"

#include "CMRT.h"
#include "CFrustum.h"

#include "CAnimator2D.h"
#include "CAnimator3D.h"
#include "CInstancingBuffer.h"

CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
	, m_Priority(-1)
	, m_LayerCheck(0)
	, m_ProjType(PROJ_TYPE::ORTHOGRAPHIC)
	, m_Width(0.f)
	, m_Height(0.f)
	, m_Far(100000.f)
	, m_FOV(XM_PI / 2.f)
	, m_ProjectionScale(1.f)
	, m_AspectRatio(0.f)
	, m_ZoomIn(false)
	, m_ZoomOut(false)
	, m_ShakingIn(false)
	, m_ShakingOut(false)
	, m_UI(false)
	, m_Frustum(nullptr)
{
	Vec2 vResolution = CDevice::GetInst()->GetResolution();
	m_Width = vResolution.x;
	m_Height = vResolution.y;
	//m_Width  = 1280.f;
	//m_Height = 768.f;
	m_AspectRatio = m_Width / m_Height;
	m_Frustum = new CFrustum(this);
}

CCamera::CCamera(const CCamera& _Other)
	: CComponent(_Other)
	, m_Priority(-1)
	, m_LayerCheck(_Other.m_LayerCheck)
	, m_ProjType(_Other.m_ProjType)
	, m_Width(_Other.m_Width)
	, m_Height(_Other.m_Height)
	, m_Far(_Other.m_Far)
	, m_FOV(_Other.m_FOV)
	, m_ProjectionScale(_Other.m_ProjectionScale)
	, m_AspectRatio(_Other.m_AspectRatio)
	, m_ZoomIn(_Other.m_ZoomIn)
	, m_ZoomOut(_Other.m_ZoomOut)
	, m_ShakingIn(_Other.m_ShakingIn)
	, m_ShakingOut(_Other.m_ShakingOut)
	, m_UI(_Other.m_UI)
	, m_Frustum(nullptr)
{
	m_Frustum = m_Frustum->Clone();
}

CCamera::~CCamera()
{
	SAFE_DELETE(m_Frustum);
}

void CCamera::Begin()
{
	// 카메라 등록
	if (-1 != m_Priority)
	{
		CRenderMgr::GetInst()->RegisterCamera(this, m_Priority);
	}
}

void CCamera::FinalTick()
{
	// View Space 란 카메라가 좌표계의 기준이 되는 좌표계
	// 1. 카메라가 원점에 존재
	// 2. 카메라가 바라보는 방향이 Z 축

	// 1. 카메라가 있는 곳이 원점이었을 경우를 기준으로한 물체들의 좌표를 알아내야 한다.
	// 2. 카메라가 월드에서 바라보던 방향을 Z 축으로 돌려두어야 한다.
	//    물체들도 같이 회전을 한다.

	// View 행렬을 계산한다.
	// View 행렬은 World Space -> View Space 로 변경할때 사용하는 행렬		
	Matrix matTrans = XMMatrixTranslation(-Transform()->GetRelativePos().x, -Transform()->GetRelativePos().y, -Transform()->GetRelativePos().z);

	Matrix matRot;
	Vec3 vR = Transform()->GetWorldDir(DIR::RIGHT);
	Vec3 vU = Transform()->GetWorldDir(DIR::UP);
	Vec3 vF = Transform()->GetWorldDir(DIR::FRONT);

	matRot._11 = vR.x; matRot._12 = vU.x; matRot._13 = vF.x;
	matRot._21 = vR.y; matRot._22 = vU.y; matRot._23 = vF.y;
	matRot._31 = vR.z; matRot._32 = vU.z; matRot._33 = vF.z;

	m_matView = matTrans * matRot;


	// Projection Space 투영 좌표계 (NDC)
	if (PROJ_TYPE::ORTHOGRAPHIC == m_ProjType)
	{
		// 1. 직교투영 (Orthographic)
		// 투영을 일직선으로
		// 시야 범위를 NDC 로 압축
		m_matProj = XMMatrixOrthographicLH(m_Width * m_ProjectionScale, m_Height * m_ProjectionScale, 1.f, m_Far);
	}

	else
	{
		// 2. 원근투영 (Perspective)		
		m_matProj = XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, 1.f, m_Far);
	}

	// 역행렬 계산
	m_matViewInv = XMMatrixInverse(nullptr, m_matView);
	m_matProjInv = XMMatrixInverse(nullptr, m_matProj);

	// 마우스방향 Ray 계산
	CalcRay();

	// Frustum Update
	m_Frustum->FinalTick();
}

void CCamera::CalcRay()
{
	// ViewPort 정보
	CMRT* pSwapChainMRT = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN);
	if (nullptr == pSwapChainMRT)
		return;
	const D3D11_VIEWPORT& VP = pSwapChainMRT->GetViewPort();

	// 현재 마우스 좌표
	Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();

	// 마우스를 향하는 직선은 카메라 위치를 지난다.
	m_Ray.vStart = Transform()->GetWorldPos();

	// View 공간 상에서 카메라에서 마우스 방향을 향하는 방향벡터를 구한다.
	//  - 마우스가 있는 좌표를 -1 ~ 1 사이의 정규화된 좌표로 바꾼다.
	//  - 투영행렬의 _11, _22 에 있는 값은 Near 평면상에서 Near 값을 가로 세로 길이로 나눈값
	//  - 실제 ViewSpace 상에서의 Near 평명상에서 마우스가 있는 지점을 향하는 위치를 구하기 위해서 비율을 나누어서 
	//  - 실제 Near 평면상에서 마우스가 향하는 위치를 좌표를 구함
	m_Ray.vDir.x = (((vMousePos.x - VP.TopLeftX) * 2.f / VP.Width) - 1.f) / m_matProj._11;
	m_Ray.vDir.y = -(((vMousePos.y - VP.TopLeftY) * 2.f / VP.Height) - 1.f) / m_matProj._22;
	m_Ray.vDir.z = 1.f;

	// 방향 벡터에 ViewMatInv 를 적용, 월드상에서의 방향을 알아낸다.
	m_Ray.vDir = XMVector3TransformNormal(m_Ray.vDir, m_matViewInv);
	m_Ray.vDir.Normalize();
}

void CCamera::SortGameObject()
{
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (false == (m_LayerCheck & (1 << i)))
		{
			continue;
		}

		CLayer* pLayer = pLevel->GetLayer(i);

		const vector<CGameObject*>& vecObjects = pLayer->GetObjects();
		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			CRenderComponent* pRenderCom = vecObjects[j]->GetRenderComponent();

			// 렌더링 기능이 없는 오브젝트는 제외
			if (nullptr == pRenderCom || nullptr == pRenderCom->GetMesh())
				continue;

			// FrustumCheck 기능을 사용하는지, 사용한다면 Frustum 내부에 들어오는지 체크
			//if (vecObjects[j]->GetRenderComponent()->IsFrustumCheck())
			//{
			//	// vecObjects[j] 의 BoundingBox 를 확인
			//	if (vecObjects[j]->BoundingBox())
			//	{
			//		Vec3 vWorldPos = vecObjects[j]->BoundingBox()->GetWorldPos();
			//		float Radius = vecObjects[j]->BoundingBox()->GetRadius();

			//		if (false == m_Frustum->FrustumCheck(vWorldPos, Radius))
			//		{
			//			continue;
			//		}
			//	}

			//	else
			//	{
			//		Vec3 vWorldPos = vecObjects[j]->Transform()->GetWorldPos();
			//		Vec3 vWorldScale = vecObjects[j]->Transform()->GetWorldScale();

			//		float Radius = vWorldScale.x;
			//		if (Radius < vWorldScale.y) Radius = vWorldScale.y;
			//		if (Radius < vWorldScale.z) Radius = vWorldScale.z;

			//		if (false == m_Frustum->FrustumCheck(vWorldPos, Radius))
			//		{
			//			continue;
			//		}
			//	}
			//}

			// 메테리얼 개수만큼 반복
			UINT iMtrlCount = pRenderCom->GetMaterialCount();
			for (UINT iMtrl = 0; iMtrl < iMtrlCount; ++iMtrl)
			{
				// 재질이 없거나, 재질의 쉐이더가 설정이 안된 경우
				if (nullptr == pRenderCom->GetMaterial(iMtrl)
					|| nullptr == pRenderCom->GetMaterial(iMtrl)->GetShader())
				{
					continue;
				}

				// 쉐이더 도메인에 따른 분류
				Ptr<CGraphicShader> pShader = pRenderCom->GetMaterial(iMtrl)->GetShader();
				SHADER_DOMAIN Domain = pShader->GetDomain();

				switch (Domain)
				{
				case SHADER_DOMAIN::DOMAIN_DEFERRED:
				case SHADER_DOMAIN::DOMAIN_OPAQUE:
				case SHADER_DOMAIN::DOMAIN_MASKED:
				{
					// Shader 의 DOMAIN 에 따라서 인스턴싱 그룹을 분류한다.
					map<ULONG64, vector<tInstObj>>* pMap = NULL;
					Ptr<CMaterial> pMtrl = pRenderCom->GetMaterial(iMtrl);

					if (pShader->GetDomain() == SHADER_DOMAIN::DOMAIN_DEFERRED)
					{
						pMap = &m_mapInstGroup_D;
					}
					else if (pShader->GetDomain() == SHADER_DOMAIN::DOMAIN_OPAQUE
						|| pShader->GetDomain() == SHADER_DOMAIN::DOMAIN_MASKED)
					{
						pMap = &m_mapInstGroup_F;
					}
					else
					{
						assert(nullptr);
						continue;
					}

					uInstID uID = {};
					uID.llID = pRenderCom->GetInstID(iMtrl);

					// ID 가 0 다 ==> Mesh 나 Material 이 셋팅되지 않았다.
					if (0 == uID.llID)
						continue;

					map<ULONG64, vector<tInstObj>>::iterator iter = pMap->find(uID.llID);
					if (iter == pMap->end())
					{
						pMap->insert(make_pair(uID.llID, vector<tInstObj>{tInstObj{ vecObjects[j], iMtrl }}));
					}
					else
					{
						iter->second.push_back(tInstObj{ vecObjects[j], iMtrl });
					}
				}
				break;
				case DOMAIN_DECAL:
					m_vecDecal.push_back(vecObjects[j]);
					break;
				case DOMAIN_TRANSPARENT:
					m_vecTransparent.push_back(vecObjects[j]);
					break;
				case DOMAIN_PARTICLE:
					m_vecParticles.push_back(vecObjects[j]);
					break;
				case DOMAIN_EFFECT:
					m_vecEffect.push_back(vecObjects[j]);
					break;
				case DOMAIN_POSTPROCESS:
					m_vecPostProcess.push_back(vecObjects[j]);
					break;
				case DOMAIN_UI:
					m_vecUI.push_back(vecObjects[j]);
					break;
				}
			}
		}
	}
}

void CCamera::SortGameObject_ShadowMap()
{
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (false == (m_LayerCheck & (1 << i)))
		{
			continue;
		}

		CLayer* pLayer = pLevel->GetLayer(i);

		const vector<CGameObject*>& vecObjects = pLayer->GetObjects();

		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			if (nullptr == vecObjects[j]->GetRenderComponent()
				|| nullptr == vecObjects[j]->GetRenderComponent()->GetMesh()
				|| nullptr == vecObjects[j]->GetRenderComponent()->GetMaterial(0)
				|| nullptr == vecObjects[j]->GetRenderComponent()->GetMaterial(0)->GetShader())
			{
				continue;
			}

			if (vecObjects[j]->BoundingBox())
			{
				if (vecObjects[j]->GetRenderComponent()->IsFrustumCheck()
					&& false == m_Frustum->FrustumCheck(vecObjects[j]->Transform()->GetWorldPos()
						, vecObjects[j]->BoundingBox()->GetRadius()))
					continue;
			}

			Ptr<CGraphicShader> pShader = vecObjects[j]->GetRenderComponent()->GetMaterial(0)->GetShader();
			
			m_vecShadowMap.push_back(vecObjects[j]);
		}
	}
}

void CCamera::render_deferred()
{
	for (auto& pair : m_mapSingleObj)
	{
		pair.second.clear();
	}

	// Deferred object render
	tInstancingData tInstData = {};

	for (auto& pair : m_mapInstGroup_D)
	{
		// 그룹 오브젝트가 없거나, 쉐이더가 없는 경우
		if (pair.second.empty())
			continue;

		// instancing 개수 조건 미만이거나
		// Animation2D 오브젝트거나(스프라이트 애니메이션 오브젝트)
		// Shader 가 Instancing 을 지원하지 않는경우
		if (pair.second.size() <= 1
			|| pair.second[0].pObj->Animator2D()
			|| pair.second[0].pObj->GetRenderComponent()->GetMaterial(pair.second[0].iMtrlIdx)->GetShader()->GetVSInst() == nullptr)
		{
			// 해당 물체들은 단일 랜더링으로 전환
			for (UINT i = 0; i < pair.second.size(); ++i)
			{
				map<INT_PTR, vector<tInstObj>>::iterator iter
					= m_mapSingleObj.find((INT_PTR)pair.second[i].pObj);

				if (iter != m_mapSingleObj.end())
					iter->second.push_back(pair.second[i]);
				else
				{
					m_mapSingleObj.insert(make_pair((INT_PTR)pair.second[i].pObj, vector<tInstObj>{pair.second[i]}));
				}
			}
			continue;
		}

		CGameObject* pObj = pair.second[0].pObj;
		Ptr<CMesh> pMesh = pObj->GetRenderComponent()->GetMesh();
		Ptr<CMaterial> pMtrl = pObj->GetRenderComponent()->GetMaterial(pair.second[0].iMtrlIdx);

		// Instancing 버퍼 클리어
		CInstancingBuffer::GetInst()->Clear();

		int iRowIdx = 0;
		bool bHasAnim3D = false;
		for (UINT i = 0; i < pair.second.size(); ++i)
		{
			tInstData.matWorld = pair.second[i].pObj->Transform()->GetWorldMat();
			tInstData.matWV = tInstData.matWorld * m_matView;
			tInstData.matWVP = tInstData.matWV * m_matProj;

			if (pair.second[i].pObj->Animator3D())
			{
				pair.second[i].pObj->Animator3D()->Binding();
				tInstData.iRowIdx = iRowIdx++;
				CInstancingBuffer::GetInst()->AddInstancingBoneMat(pair.second[i].pObj->Animator3D()->GetFinalBoneMat());
				bHasAnim3D = true;
			}
			else
				tInstData.iRowIdx = -1;

			CInstancingBuffer::GetInst()->AddInstancingData(tInstData);
		}

		// 인스턴싱에 필요한 데이터를 세팅(SysMem -> GPU Mem)
		CInstancingBuffer::GetInst()->SetData();

		if (bHasAnim3D)
		{
			pMtrl->SetAnim3D(true); // Animation Mesh 알리기
			pMtrl->SetBoneCount(pMesh->GetBoneCount());
		}

		pMtrl->Binding_Inst();
		pMesh->Render_Instancing(pair.second[0].iMtrlIdx);

		// 정리
		if (bHasAnim3D)
		{
			pMtrl->SetAnim3D(false); // Animation Mesh 알리기
			pMtrl->SetBoneCount(0);
		}
	}

	// 개별 랜더링
	for (auto& pair : m_mapSingleObj)
	{
		if (pair.second.empty())
			continue;

		pair.second[0].pObj->Transform()->Binding();

		for (auto& instObj : pair.second)
		{
			instObj.pObj->GetRenderComponent()->Render(instObj.iMtrlIdx);
		}

		if (pair.second[0].pObj->Animator3D())
		{
			pair.second[0].pObj->Animator3D()->ClearData();
		}
	}
}

void CCamera::render_forward()
{
	for (auto& pair : m_mapSingleObj)
	{
		pair.second.clear();
	}

	// Deferred object render
	tInstancingData tInstData = {};

	for (auto& pair : m_mapInstGroup_F)
	{
		// 그룹 오브젝트가 없거나, 쉐이더가 없는 경우
		if (pair.second.empty())
			continue;

		// instancing 개수 조건 미만이거나
		// Animation2D 오브젝트거나(스프라이트 애니메이션 오브젝트)
		// Shader 가 Instancing 을 지원하지 않는경우
		if (pair.second.size() <= 1
			|| pair.second[0].pObj->Animator2D()
			|| pair.second[0].pObj->GetRenderComponent()->GetMaterial(pair.second[0].iMtrlIdx)->GetShader()->GetVSInst() == nullptr)
		{
			// 해당 물체들은 단일 랜더링으로 전환
			for (UINT i = 0; i < pair.second.size(); ++i)
			{
				map<INT_PTR, vector<tInstObj>>::iterator iter
					= m_mapSingleObj.find((INT_PTR)pair.second[i].pObj);

				if (iter != m_mapSingleObj.end())
					iter->second.push_back(pair.second[i]);
				else
				{
					m_mapSingleObj.insert(make_pair((INT_PTR)pair.second[i].pObj, vector<tInstObj>{pair.second[i]}));
				}
			}
			continue;
		}

		CGameObject* pObj = pair.second[0].pObj;
		Ptr<CMesh> pMesh = pObj->GetRenderComponent()->GetMesh();
		Ptr<CMaterial> pMtrl = pObj->GetRenderComponent()->GetMaterial(pair.second[0].iMtrlIdx);

		// Instancing 버퍼 클리어
		CInstancingBuffer::GetInst()->Clear();

		int iRowIdx = 0;
		bool bHasAnim3D = false;
		for (UINT i = 0; i < pair.second.size(); ++i)
		{
			tInstData.matWorld = pair.second[i].pObj->Transform()->GetWorldMat();
			tInstData.matWV = tInstData.matWorld * m_matView;
			tInstData.matWVP = tInstData.matWV * m_matProj;

			if (pair.second[i].pObj->Animator3D())
			{
				pair.second[i].pObj->Animator3D()->Binding();
				tInstData.iRowIdx = iRowIdx++;
				CInstancingBuffer::GetInst()->AddInstancingBoneMat(pair.second[i].pObj->Animator3D()->GetFinalBoneMat());
				bHasAnim3D = true;
			}
			else
				tInstData.iRowIdx = -1;

			CInstancingBuffer::GetInst()->AddInstancingData(tInstData);
		}

		// 인스턴싱에 필요한 데이터를 세팅(SysMem -> GPU Mem)
		CInstancingBuffer::GetInst()->SetData();

		if (bHasAnim3D)
		{
			pMtrl->SetAnim3D(true); // Animation Mesh 알리기
			pMtrl->SetBoneCount(pMesh->GetBoneCount());
		}

		pMtrl->Binding_Inst();
		pMesh->Render_Instancing(pair.second[0].iMtrlIdx);

		// 정리
		if (bHasAnim3D)
		{
			pMtrl->SetAnim3D(false); // Animation Mesh 알리기
			pMtrl->SetBoneCount(0);
		}
	}

	// 개별 랜더링
	for (auto& pair : m_mapSingleObj)
	{
		if (pair.second.empty())
			continue;

		pair.second[0].pObj->Transform()->Binding();

		for (auto& instObj : pair.second)
		{
			instObj.pObj->GetRenderComponent()->Render(instObj.iMtrlIdx);
		}

		if (pair.second[0].pObj->Animator3D())
		{
			pair.second[0].pObj->Animator3D()->ClearData();
		}
	}
}

void CCamera::render_decal()
{
	// Decal
	for (size_t i = 0; i < m_vecDecal.size(); ++i)
	{
		m_vecDecal[i]->Render();
	}
}

void CCamera::render_effect()
{
	// EffectMRT 로 변경
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::EFFECT)->Clear();
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::EFFECT)->OMSet();

	// Effect
	for (size_t i = 0; i < m_vecEffect.size(); ++i)
	{
		m_vecEffect[i]->Render();
	}

	// EffectBlurMRT 로 변경
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::EFFECT_BLUR)->ClearRT();
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::EFFECT_BLUR)->OMSet();

	Ptr<CMaterial> pBlurMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BlurMtrl");
	Ptr<CMesh> pRectMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh");

	pBlurMtrl->SetTexParam(TEX_0, CRenderMgr::GetInst()->GetMRT(MRT_TYPE::EFFECT)->GetRT(0));
	pBlurMtrl->Binding();
	pRectMesh->Render_Particle(2);

	// 원래 렌더타겟(SwapChainMRT) 로 변경	
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet();
	Ptr<CMaterial> pEffectMergeMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"EffectMergeMtrl");

	pEffectMergeMtrl->SetTexParam(TEX_0, CRenderMgr::GetInst()->GetMRT(MRT_TYPE::EFFECT)->GetRT(0));
	pEffectMergeMtrl->SetTexParam(TEX_1, CRenderMgr::GetInst()->GetMRT(MRT_TYPE::EFFECT_BLUR)->GetRT(0));
	pEffectMergeMtrl->Binding();
	pRectMesh->Render(0);
}

void CCamera::render_transparent()
{
	// Transparent
	for (size_t i = 0; i < m_vecTransparent.size(); ++i)
	{
		m_vecTransparent[i]->Render();
	}
}

void CCamera::render_particle()
{
	// Particles
	for (size_t i = 0; i < m_vecParticles.size(); ++i)
	{
		m_vecParticles[i]->Render();
	}
}

void CCamera::render_postprocess()
{
	for (size_t i = 0; i < m_vecPostProcess.size(); ++i)
	{
		CRenderMgr::GetInst()->PostProcessCopy();
		m_vecPostProcess[i]->Render();
	}
}

void CCamera::render_ui()
{
	for (size_t i = 0; i < m_vecUI.size(); ++i)
	{
		m_vecUI[i]->Render();
	}
}

void CCamera::render_shadowmap()
{
	for (size_t i = 0; i < m_vecShadowMap.size(); ++i)
	{
		m_vecShadowMap[i]->GetRenderComponent()->render_shadowmap();
	}

	m_vecShadowMap.clear();
}

void CCamera::render_shadowblur()
{
	// EffectMRT 로 변경
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SHADOWBLUR)->Clear();
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SHADOWBLUR)->OMSet();

	Ptr<CMaterial> pBlurMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ShadowBlurMtrl");
	Ptr<CMesh> pRectMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh");

	pBlurMtrl->SetTexParam(TEX_0, CRenderMgr::GetInst()->GetMRT(MRT_TYPE::LIGHT)->GetRT(2));
	pBlurMtrl->Binding();
	pRectMesh->Render(0);
}

void CCamera::clear()
{
	// Runtime 중 Domain Type이 변경 될 수 있기 때문에 Render 호출 하고 clear 시켜줌
	// 이전 프레임 분류정보 제거
	m_mapInstGroup_D.clear();
	m_mapInstGroup_F.clear();

	m_vecDecal.clear();

	m_vecTransparent.clear();
	m_vecParticles.clear();
	m_vecEffect.clear();
	m_vecPostProcess.clear();
	m_vecUI.clear();
}

void CCamera::SaveToFile(FILE* _pFile)
{
	fwrite(&m_Priority, sizeof(int), 1, _pFile);
	fwrite(&m_LayerCheck, sizeof(UINT), 1, _pFile);
	fwrite(&m_ProjType, sizeof(PROJ_TYPE), 1, _pFile);
	fwrite(&m_Width, sizeof(float), 1, _pFile);
	fwrite(&m_Height, sizeof(float), 1, _pFile);
	fwrite(&m_Far, sizeof(float), 1, _pFile);
	fwrite(&m_AspectRatio, sizeof(float), 1, _pFile);
	fwrite(&m_FOV, sizeof(float), 1, _pFile);
	fwrite(&m_ProjectionScale, sizeof(float), 1, _pFile);
}

void CCamera::LoadFromFile(FILE* _pFile)
{
	fread(&m_Priority, sizeof(int), 1, _pFile);
	fread(&m_LayerCheck, sizeof(UINT), 1, _pFile);
	fread(&m_ProjType, sizeof(PROJ_TYPE), 1, _pFile);
	fread(&m_Width, sizeof(float), 1, _pFile);
	fread(&m_Height, sizeof(float), 1, _pFile);
	fread(&m_Far, sizeof(float), 1, _pFile);
	fread(&m_AspectRatio, sizeof(float), 1, _pFile);
	fread(&m_FOV, sizeof(float), 1, _pFile);
	fread(&m_ProjectionScale, sizeof(float), 1, _pFile);
}