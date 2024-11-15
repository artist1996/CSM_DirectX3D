#include "pch.h"
#include "CLandScape.h"

#include "CAssetMgr.h"
#include "CTexture.h"

#include "CDevice.h"
#include "CStructuredBuffer.h"

void CLandScape::Init()
{
	// LandScape 전용 Mesh 생성
	CreateMesh();

	// LandScape 전용 재질 참조
	Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LandScapeMtrl");
	SetMaterial(pMtrl);

	// LandScape 전용 컴퓨트 쉐이더 제작
	CreateComputeShader();

	// LandScape 용 텍스쳐 생성 및 로딩
	CreateTextureAndStructuredBuffer();

	// BrushTexture 추가	
	AddBrushTexture(CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\brush\\TX_GlowScene_2.png"));
	AddBrushTexture(CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\brush\\TX_HitFlash_0.png"));
	AddBrushTexture(CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\brush\\TX_HitFlash02.png"));
	AddBrushTexture(CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\brush\\TX_Twirl02.png"));
	AddBrushTexture(CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\brush\\FX_Flare.png"));
	m_BrushIdx = 0;
}

void CLandScape::CreateMesh()
{
	Vtx v;
	vector<Vtx> vecVtx;

	// 정점
	for (UINT Row = 0; Row < m_FaceZ + 1; ++Row)
	{
		for (UINT Col = 0; Col < m_FaceX + 1; ++Col)
		{
			v.vPos = Vec3((float)Col, 0.f, (float)Row);
			v.vNormal = Vec3(0.f, 1.f, 0.f);
			v.vTangent = Vec3(1.f, 0.f, 0.f);
			v.vBinormal = Vec3(0.f, 0.f, -1.f);

			vecVtx.push_back(v);
		}
	}

	// 인덱스
	vector<UINT> vecIdx;
	for (UINT Row = 0; Row < m_FaceZ; ++Row)
	{
		for (UINT Col = 0; Col < m_FaceX; ++Col)
		{
			// 0
			// | \
			// 2--1
			vecIdx.push_back((Row * (m_FaceX + 1)) + Col + m_FaceX + 1);
			vecIdx.push_back((Row * (m_FaceX + 1)) + Col + 1);
			vecIdx.push_back((Row * (m_FaceX + 1)) + Col);

			// 1--2 
			//  \ |
			//    0
			vecIdx.push_back((Row * (m_FaceX + 1)) + Col + 1);
			vecIdx.push_back((Row * (m_FaceX + 1)) + Col + m_FaceX + 1);
			vecIdx.push_back((Row * (m_FaceX + 1)) + Col + m_FaceX + 1 + 1);
		}
	}

	Ptr<CMesh> pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	SetMesh(pMesh);
}

void CLandScape::CreateComputeShader()
{
	// HeightMapCS 가 있으면 찾아오고 없으면 컴파일해서 등록한다.
	m_HeightMapCS = (CHeightMapCS*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"HeightMapCS").Get();
	if (nullptr == m_HeightMapCS)
	{
		m_HeightMapCS = new CHeightMapCS;
		CAssetMgr::GetInst()->AddAsset<CComputeShader>(L"HeightMapCS", m_HeightMapCS.Get());
	}

	// RaycastCS 생성
	m_RaycastCS = (CRaycastCS*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"RaycastCS").Get();
	if (nullptr == m_RaycastCS)
	{
		m_RaycastCS = new CRaycastCS;
		CAssetMgr::GetInst()->AddAsset<CComputeShader>(L"RaycastCS", m_RaycastCS.Get());
	}
}

void CLandScape::CreateTextureAndStructuredBuffer()
{
	// Raycasting 결과를 받는 용도의 구조화버퍼
	m_RaycastOut = new CStructuredBuffer;
	m_RaycastOut->Create(sizeof(tRaycastOut), 1, SB_TYPE::SRV_UAV, true);
}



void CLandScape::CreateHeightMap(UINT _Width, UINT _Height)
{
	m_IsHeightMapCreated = true;

	m_HeightMap = CAssetMgr::GetInst()->CreateTexture(L"LandScapeHeightMap", _Width, _Height
		, DXGI_FORMAT_R32_FLOAT
		, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);
}
