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
	// ī�޶� ���
	if (-1 != m_Priority)
	{
		CRenderMgr::GetInst()->RegisterCamera(this, m_Priority);
	}
}

void CCamera::FinalTick()
{
	// View Space �� ī�޶� ��ǥ���� ������ �Ǵ� ��ǥ��
	// 1. ī�޶� ������ ����
	// 2. ī�޶� �ٶ󺸴� ������ Z ��

	// 1. ī�޶� �ִ� ���� �����̾��� ��츦 ���������� ��ü���� ��ǥ�� �˾Ƴ��� �Ѵ�.
	// 2. ī�޶� ���忡�� �ٶ󺸴� ������ Z ������ �����ξ�� �Ѵ�.
	//    ��ü�鵵 ���� ȸ���� �Ѵ�.

	// View ����� ����Ѵ�.
	// View ����� World Space -> View Space �� �����Ҷ� ����ϴ� ���		
	Matrix matTrans = XMMatrixTranslation(-Transform()->GetRelativePos().x, -Transform()->GetRelativePos().y, -Transform()->GetRelativePos().z);

	Matrix matRot;
	Vec3 vR = Transform()->GetWorldDir(DIR::RIGHT);
	Vec3 vU = Transform()->GetWorldDir(DIR::UP);
	Vec3 vF = Transform()->GetWorldDir(DIR::FRONT);

	matRot._11 = vR.x; matRot._12 = vU.x; matRot._13 = vF.x;
	matRot._21 = vR.y; matRot._22 = vU.y; matRot._23 = vF.y;
	matRot._31 = vR.z; matRot._32 = vU.z; matRot._33 = vF.z;

	m_matView = matTrans * matRot;


	// Projection Space ���� ��ǥ�� (NDC)
	if (PROJ_TYPE::ORTHOGRAPHIC == m_ProjType)
	{
		// 1. �������� (Orthographic)
		// ������ ����������
		// �þ� ������ NDC �� ����
		m_matProj = XMMatrixOrthographicLH(m_Width * m_ProjectionScale, m_Height * m_ProjectionScale, 1.f, m_Far);
	}

	else
	{
		// 2. �������� (Perspective)		
		m_matProj = XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, 1.f, m_Far);
	}

	// ����� ���
	m_matViewInv = XMMatrixInverse(nullptr, m_matView);
	m_matProjInv = XMMatrixInverse(nullptr, m_matProj);

	// ���콺���� Ray ���
	CalcRay();

	// Frustum Update
	m_Frustum->FinalTick();
}

void CCamera::CalcRay()
{
	// ViewPort ����
	CMRT* pSwapChainMRT = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN);
	if (nullptr == pSwapChainMRT)
		return;
	const D3D11_VIEWPORT& VP = pSwapChainMRT->GetViewPort();

	// ���� ���콺 ��ǥ
	Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();

	// ���콺�� ���ϴ� ������ ī�޶� ��ġ�� ������.
	m_Ray.vStart = Transform()->GetWorldPos();

	// View ���� �󿡼� ī�޶󿡼� ���콺 ������ ���ϴ� ���⺤�͸� ���Ѵ�.
	//  - ���콺�� �ִ� ��ǥ�� -1 ~ 1 ������ ����ȭ�� ��ǥ�� �ٲ۴�.
	//  - ��������� _11, _22 �� �ִ� ���� Near ���󿡼� Near ���� ���� ���� ���̷� ������
	//  - ���� ViewSpace �󿡼��� Near ���󿡼� ���콺�� �ִ� ������ ���ϴ� ��ġ�� ���ϱ� ���ؼ� ������ ����� 
	//  - ���� Near ���󿡼� ���콺�� ���ϴ� ��ġ�� ��ǥ�� ����
	m_Ray.vDir.x = (((vMousePos.x - VP.TopLeftX) * 2.f / VP.Width) - 1.f) / m_matProj._11;
	m_Ray.vDir.y = -(((vMousePos.y - VP.TopLeftY) * 2.f / VP.Height) - 1.f) / m_matProj._22;
	m_Ray.vDir.z = 1.f;

	// ���� ���Ϳ� ViewMatInv �� ����, ����󿡼��� ������ �˾Ƴ���.
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

			// ������ ����� ���� ������Ʈ�� ����
			if (nullptr == pRenderCom || nullptr == pRenderCom->GetMesh())
				continue;

			// FrustumCheck ����� ����ϴ���, ����Ѵٸ� Frustum ���ο� �������� üũ
			//if (vecObjects[j]->GetRenderComponent()->IsFrustumCheck())
			//{
			//	// vecObjects[j] �� BoundingBox �� Ȯ��
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

			// ���׸��� ������ŭ �ݺ�
			UINT iMtrlCount = pRenderCom->GetMaterialCount();
			for (UINT iMtrl = 0; iMtrl < iMtrlCount; ++iMtrl)
			{
				// ������ ���ų�, ������ ���̴��� ������ �ȵ� ���
				if (nullptr == pRenderCom->GetMaterial(iMtrl)
					|| nullptr == pRenderCom->GetMaterial(iMtrl)->GetShader())
				{
					continue;
				}

				// ���̴� �����ο� ���� �з�
				Ptr<CGraphicShader> pShader = pRenderCom->GetMaterial(iMtrl)->GetShader();
				SHADER_DOMAIN Domain = pShader->GetDomain();

				switch (Domain)
				{
				case SHADER_DOMAIN::DOMAIN_DEFERRED:
				case SHADER_DOMAIN::DOMAIN_OPAQUE:
				case SHADER_DOMAIN::DOMAIN_MASKED:
				{
					// Shader �� DOMAIN �� ���� �ν��Ͻ� �׷��� �з��Ѵ�.
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

					// ID �� 0 �� ==> Mesh �� Material �� ���õ��� �ʾҴ�.
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
		// �׷� ������Ʈ�� ���ų�, ���̴��� ���� ���
		if (pair.second.empty())
			continue;

		// instancing ���� ���� �̸��̰ų�
		// Animation2D ������Ʈ�ų�(��������Ʈ �ִϸ��̼� ������Ʈ)
		// Shader �� Instancing �� �������� �ʴ°��
		if (pair.second.size() <= 1
			|| pair.second[0].pObj->Animator2D()
			|| pair.second[0].pObj->GetRenderComponent()->GetMaterial(pair.second[0].iMtrlIdx)->GetShader()->GetVSInst() == nullptr)
		{
			// �ش� ��ü���� ���� ���������� ��ȯ
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

		// Instancing ���� Ŭ����
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

		// �ν��Ͻ̿� �ʿ��� �����͸� ����(SysMem -> GPU Mem)
		CInstancingBuffer::GetInst()->SetData();

		if (bHasAnim3D)
		{
			pMtrl->SetAnim3D(true); // Animation Mesh �˸���
			pMtrl->SetBoneCount(pMesh->GetBoneCount());
		}

		pMtrl->Binding_Inst();
		pMesh->Render_Instancing(pair.second[0].iMtrlIdx);

		// ����
		if (bHasAnim3D)
		{
			pMtrl->SetAnim3D(false); // Animation Mesh �˸���
			pMtrl->SetBoneCount(0);
		}
	}

	// ���� ������
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
		// �׷� ������Ʈ�� ���ų�, ���̴��� ���� ���
		if (pair.second.empty())
			continue;

		// instancing ���� ���� �̸��̰ų�
		// Animation2D ������Ʈ�ų�(��������Ʈ �ִϸ��̼� ������Ʈ)
		// Shader �� Instancing �� �������� �ʴ°��
		if (pair.second.size() <= 1
			|| pair.second[0].pObj->Animator2D()
			|| pair.second[0].pObj->GetRenderComponent()->GetMaterial(pair.second[0].iMtrlIdx)->GetShader()->GetVSInst() == nullptr)
		{
			// �ش� ��ü���� ���� ���������� ��ȯ
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

		// Instancing ���� Ŭ����
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

		// �ν��Ͻ̿� �ʿ��� �����͸� ����(SysMem -> GPU Mem)
		CInstancingBuffer::GetInst()->SetData();

		if (bHasAnim3D)
		{
			pMtrl->SetAnim3D(true); // Animation Mesh �˸���
			pMtrl->SetBoneCount(pMesh->GetBoneCount());
		}

		pMtrl->Binding_Inst();
		pMesh->Render_Instancing(pair.second[0].iMtrlIdx);

		// ����
		if (bHasAnim3D)
		{
			pMtrl->SetAnim3D(false); // Animation Mesh �˸���
			pMtrl->SetBoneCount(0);
		}
	}

	// ���� ������
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
	// EffectMRT �� ����
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::EFFECT)->Clear();
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::EFFECT)->OMSet();

	// Effect
	for (size_t i = 0; i < m_vecEffect.size(); ++i)
	{
		m_vecEffect[i]->Render();
	}

	// EffectBlurMRT �� ����
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::EFFECT_BLUR)->ClearRT();
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::EFFECT_BLUR)->OMSet();

	Ptr<CMaterial> pBlurMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BlurMtrl");
	Ptr<CMesh> pRectMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh");

	pBlurMtrl->SetTexParam(TEX_0, CRenderMgr::GetInst()->GetMRT(MRT_TYPE::EFFECT)->GetRT(0));
	pBlurMtrl->Binding();
	pRectMesh->Render_Particle(2);

	// ���� ����Ÿ��(SwapChainMRT) �� ����	
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
	// EffectMRT �� ����
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
	// Runtime �� Domain Type�� ���� �� �� �ֱ� ������ Render ȣ�� �ϰ� clear ������
	// ���� ������ �з����� ����
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