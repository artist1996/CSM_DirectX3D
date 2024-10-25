#include "pch.h"
#include "CAssetMgr.h"

#include "CDevice.h"

void CAssetMgr::Init()
{
	CreateEngineMesh();

	CreateEngineTexture();

	CreateEngineSprite();

	CreateEngineGraphicShader();

	CreateEngineComputeShader();

	CreateEngineMaterial();
}

void CAssetMgr::CreateEngineMesh()
{
	Ptr<CMesh> pMesh = nullptr;
	Vtx v;

	// PointMesh
	pMesh = new CMesh(true);
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);

	UINT i = 0;
	pMesh->Create(&v, 1, &i, 1);
	AddAsset(L"PointMesh", pMesh);

	// RectMesh 생성	
	// 0 -- 1
	// | \  |
	// 3 -- 2
	Vtx arrVtx[4] = {};

	v.vPos = Vec3(-0.5f, 0.5f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vNormal = Vec3(0.f, 0.f, -1.f);
	v.vBinormal = Vec3(0.f, -1.f, 0.f);
	arrVtx[0] = v;

	v.vPos = Vec3(0.5f, 0.5f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 0.f);
	arrVtx[1] = v;

	v.vPos = Vec3(0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1.f, 1.f);
	arrVtx[2] = v;

	v.vPos = Vec3(-0.5f, -0.5f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 1.f);
	arrVtx[3] = v;

	// Index 버퍼 생성
	UINT arrIdx[6] = {};
	arrIdx[0] = 0;	arrIdx[1] = 1;	arrIdx[2] = 2;
	arrIdx[3] = 0; 	arrIdx[4] = 2;	arrIdx[5] = 3;

	pMesh = new CMesh(true);
	pMesh->Create(arrVtx, 4, arrIdx, 6);
	pMesh->SetEngineAsset();
	AddAsset(L"RectMesh", pMesh);


	// RectMesh_Debug
	arrIdx[0] = 0;	arrIdx[1] = 1;	arrIdx[2] = 2; arrIdx[3] = 3; arrIdx[4] = 0;

	pMesh = new CMesh(true);
	pMesh->Create(arrVtx, 4, arrIdx, 5);
	pMesh->SetEngineAsset();
	AddAsset(L"RectMesh_Debug", pMesh);


	// CircleMesh 
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;

	int Slice = 40;
	float fTheta = XM_2PI / Slice;
	float Radius = 0.5f;

	// 중심점
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vUV = Vec2(0.5f, 0.5f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vNormal = Vec3(0.f, 0.f, -1.f);
	v.vBinormal = Vec3(0.f, -1.f, 0.f);
	vecVtx.push_back(v);

	// 테두리
	float Angle = 0.f;
	for (int i = 0; i < Slice + 1; ++i)
	{
		v.vPos = Vec3(Radius * cosf(Angle), Radius * sinf(Angle), 0.f);
		v.vUV = Vec2(v.vPos.x + 0.5f, -(v.vPos.y - 0.5f));
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		vecVtx.push_back(v);

		Angle += fTheta;
	}

	// 인덱스
	for (int i = 0; i < Slice; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	pMesh->SetEngineAsset();
	AddAsset(L"CircleMesh", pMesh);

	// CircleMesh_Debug
	vecIdx.clear();
	for (size_t i = 1; i < vecVtx.size(); ++i)
	{
		vecIdx.push_back((UINT)i);
	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	pMesh->SetEngineAsset();
	AddAsset(L"CircleMesh_Debug", pMesh);
	vecIdx.clear();

	// CubeMesh
	// 24개의 정점이 필요
	// 평면 하나당 정점 4개 x 6면 = 24개
	Vtx arrCube[24] = {};

	// 윗면
	arrCube[0].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[0].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[0].vUV = Vec2(0.f, 0.f);
	arrCube[0].vTangent = Vec3(1.f, 0.f, 0.f);
	arrCube[0].vNormal = Vec3(0.f, 1.f, 0.f);
	arrCube[0].vBinormal = Vec3(0.f, 0.f, -1.f);

	arrCube[1].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[1].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[1].vUV = Vec2(1.f, 0.f);
	arrCube[1].vTangent = Vec3(1.f, 0.f, 0.f);
	arrCube[1].vNormal = Vec3(0.f, 1.f, 0.f);
	arrCube[1].vBinormal = Vec3(0.f, 0.f, -1.f);

	arrCube[2].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[2].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[2].vUV = Vec2(1.f, 1.f);
	arrCube[2].vTangent = Vec3(1.f, 0.f, 0.f);
	arrCube[2].vNormal = Vec3(0.f, 1.f, 0.f);
	arrCube[2].vBinormal = Vec3(0.f, 0.f, -1.f);

	arrCube[3].vPos = Vec3(-0.5f, 0.5f, -0.5f);
	arrCube[3].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[3].vUV = Vec2(0.f, 1.f);
	arrCube[3].vTangent = Vec3(1.f, 0.f, 0.f);
	arrCube[3].vNormal = Vec3(0.f, 1.f, 0.f);
	arrCube[3].vBinormal = Vec3(0.f, 0.f, -1.f);


	// 아랫 면	
	arrCube[4].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[4].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[4].vUV = Vec2(0.f, 0.f);
	arrCube[4].vTangent = Vec3(1.f, 0.f, 0.f);
	arrCube[4].vNormal = Vec3(0.f, -1.f, 0.f);
	arrCube[4].vBinormal = Vec3(0.f, 0.f, 1.f);

	arrCube[5].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[5].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[5].vUV = Vec2(1.f, 0.f);
	arrCube[5].vTangent = Vec3(1.f, 0.f, 0.f);
	arrCube[5].vNormal = Vec3(0.f, -1.f, 0.f);
	arrCube[5].vBinormal = Vec3(0.f, 0.f, 1.f);

	arrCube[6].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[6].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[6].vUV = Vec2(1.f, 1.f);
	arrCube[6].vTangent = Vec3(1.f, 0.f, 0.f);
	arrCube[6].vNormal = Vec3(0.f, -1.f, 0.f);
	arrCube[6].vBinormal = Vec3(0.f, 0.f, 1.f);

	arrCube[7].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[7].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[7].vUV = Vec2(0.f, 1.f);
	arrCube[7].vTangent = Vec3(1.f, 0.f, 0.f);
	arrCube[7].vNormal = Vec3(0.f, -1.f, 0.f);
	arrCube[7].vBinormal = Vec3(0.f, 0.f, 1.f);

	// 왼쪽 면
	arrCube[8].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[8].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[8].vUV = Vec2(0.f, 0.f);
	arrCube[8].vTangent = Vec3(0.f, 0.f, -1.f);
	arrCube[8].vNormal = Vec3(-1.f, 0.f, 0.f);
	arrCube[8].vBinormal = Vec3(0.f, -1.f, 0.f);

	arrCube[9].vPos = Vec3(-0.5f, 0.5f, -0.5f);
	arrCube[9].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[9].vUV = Vec2(1.f, 0.f);
	arrCube[9].vTangent = Vec3(0.f, 0.f, -1.f);
	arrCube[9].vNormal = Vec3(-1.f, 0.f, 0.f);
	arrCube[9].vBinormal = Vec3(0.f, -1.f, 0.f);

	arrCube[10].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[10].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[10].vUV = Vec2(1.f, 1.f);
	arrCube[10].vTangent = Vec3(0.f, 0.f, -1.f);
	arrCube[10].vNormal = Vec3(-1.f, 0.f, 0.f);
	arrCube[10].vBinormal = Vec3(0.f, -1.f, 0.f);

	arrCube[11].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[11].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[11].vUV = Vec2(0.f, 1.f);
	arrCube[11].vTangent = Vec3(0.f, 0.f, -1.f);
	arrCube[11].vNormal = Vec3(-1.f, 0.f, 0.f);
	arrCube[11].vBinormal = Vec3(0.f, -1.f, 0.f);

	// 오른쪽 면
	arrCube[12].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[12].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[12].vUV = Vec2(0.f, 0.f);
	arrCube[12].vTangent = Vec3(0.f, 0.f, 1.f);
	arrCube[12].vNormal = Vec3(1.f, 0.f, 0.f);
	arrCube[12].vBinormal = Vec3(0.f, -1.f, 0.f);

	arrCube[13].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[13].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[13].vUV = Vec2(1.f, 0.f);
	arrCube[13].vTangent = Vec3(0.f, 0.f, 1.f);
	arrCube[13].vNormal = Vec3(1.f, 0.f, 0.f);
	arrCube[13].vBinormal = Vec3(0.f, -1.f, 0.f);

	arrCube[14].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[14].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[14].vUV = Vec2(1.f, 1.f);
	arrCube[14].vTangent = Vec3(0.f, 0.f, 1.f);
	arrCube[14].vNormal = Vec3(1.f, 0.f, 0.f);
	arrCube[14].vBinormal = Vec3(0.f, -1.f, 0.f);

	arrCube[15].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[15].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[15].vUV = Vec2(0.f, 1.f);
	arrCube[15].vTangent = Vec3(0.f, 0.f, 1.f);
	arrCube[15].vNormal = Vec3(1.f, 0.f, 0.f);
	arrCube[15].vBinormal = Vec3(0.f, -1.f, 0.f);

	// 뒷 면
	arrCube[16].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[16].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[16].vUV = Vec2(0.f, 0.f);
	arrCube[16].vTangent = Vec3(-1.f, 0.f, 0.f);
	arrCube[16].vNormal = Vec3(0.f, 0.f, 1.f);
	arrCube[16].vBinormal = Vec3(0.f, -1.f, 1.f);

	arrCube[17].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[17].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[17].vUV = Vec2(1.f, 0.f);
	arrCube[17].vTangent = Vec3(-1.f, 0.f, 0.f);
	arrCube[17].vNormal = Vec3(0.f, 0.f, 1.f);
	arrCube[17].vBinormal = Vec3(0.f, -1.f, 1.f);

	arrCube[18].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[18].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[18].vUV = Vec2(1.f, 1.f);
	arrCube[18].vTangent = Vec3(-1.f, 0.f, 0.f);
	arrCube[18].vNormal = Vec3(0.f, 0.f, 1.f);
	arrCube[18].vBinormal = Vec3(0.f, -1.f, 1.f);

	arrCube[19].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[19].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[19].vUV = Vec2(0.f, 1.f);
	arrCube[19].vTangent = Vec3(-1.f, 0.f, 0.f);
	arrCube[19].vNormal = Vec3(0.f, 0.f, 1.f);
	arrCube[19].vBinormal = Vec3(0.f, -1.f, 1.f);

	// 앞 면
	arrCube[20].vPos = Vec3(-0.5f, 0.5f, -0.5f);
	arrCube[20].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[20].vUV = Vec2(0.f, 0.f);
	arrCube[20].vTangent = Vec3(1.f, 0.f, 0.f);
	arrCube[20].vNormal = Vec3(0.f, 0.f, -1.f);
	arrCube[20].vBinormal = Vec3(0.f, -1.f, 0.f);

	arrCube[21].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[21].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[21].vUV = Vec2(1.f, 0.f);
	arrCube[21].vTangent = Vec3(1.f, 0.f, 0.f);
	arrCube[21].vNormal = Vec3(0.f, 0.f, -1.f);
	arrCube[21].vBinormal = Vec3(0.f, -1.f, 0.f);

	arrCube[22].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[22].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[22].vUV = Vec2(1.f, 1.f);
	arrCube[22].vTangent = Vec3(1.f, 0.f, 0.f);
	arrCube[22].vNormal = Vec3(0.f, 0.f, -1.f);
	arrCube[22].vBinormal = Vec3(0.f, -1.f, 0.f);

	arrCube[23].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[23].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[23].vUV = Vec2(0.f, 1.f);
	arrCube[23].vTangent = Vec3(1.f, 0.f, 0.f);
	arrCube[23].vNormal = Vec3(0.f, 0.f, -1.f);
	arrCube[23].vBinormal = Vec3(0.f, -1.f, 0.f);

	// 인덱스
	for (int i = 0; i < 12; i += 2)
	{
		vecIdx.push_back(i * 2);
		vecIdx.push_back(i * 2 + 1);
		vecIdx.push_back(i * 2 + 2);

		vecIdx.push_back(i * 2);
		vecIdx.push_back(i * 2 + 2);
		vecIdx.push_back(i * 2 + 3);
	}

	pMesh = new CMesh(true);
	pMesh->Create(arrCube, 24, vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(L"CubeMesh", pMesh);

	vecIdx.clear();

	// CubeMesh_Debug
	vecIdx.push_back(0); vecIdx.push_back(1); vecIdx.push_back(2); vecIdx.push_back(3); vecIdx.push_back(4);
	vecIdx.push_back(7); vecIdx.push_back(6); vecIdx.push_back(5); vecIdx.push_back(4); vecIdx.push_back(3);
	vecIdx.push_back(0); vecIdx.push_back(7); vecIdx.push_back(6); vecIdx.push_back(1); vecIdx.push_back(2);
	vecIdx.push_back(5);

	pMesh = new CMesh(true);
	pMesh->Create(arrCube, 24, vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(L"CubeMesh_Debug", pMesh);

	vecVtx.clear();
	vecIdx.clear();

	// ============
	// Sphere Mesh
	// ============
	float fRadius = 0.5f;

	// Top
	v.vPos = Vec3(0.f, fRadius, 0.f);
	v.vUV = Vec2(0.5f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = v.vPos;
	v.vNormal.Normalize();
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 0.f, -1.f);
	vecVtx.push_back(v);

	// Body
	UINT iStackCount = 40; // 가로 분할 개수
	UINT iSliceCount = 40; // 세로 분할 개수

	float fStackAngle = XM_PI / iStackCount;
	float fSliceAngle = XM_2PI / iSliceCount;

	float fUVXStep = 1.f / (float)iSliceCount;
	float fUVYStep = 1.f / (float)iStackCount;

	for (UINT i = 1; i < iStackCount; ++i)
	{
		float phi = i * fStackAngle;

		for (UINT j = 0; j <= iSliceCount; ++j)
		{
			float theta = j * fSliceAngle;

			v.vPos = Vec3(fRadius * sinf(i * fStackAngle) * cosf(j * fSliceAngle)
				, fRadius * cosf(i * fStackAngle)
				, fRadius * sinf(i * fStackAngle) * sinf(j * fSliceAngle));

			v.vUV = Vec2(fUVXStep * j, fUVYStep * i);
			v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
			v.vNormal = v.vPos;
			v.vNormal.Normalize();

			v.vTangent.x = -fRadius * sinf(phi) * sinf(theta);
			v.vTangent.y = 0.f;
			v.vTangent.z = fRadius * sinf(phi) * cosf(theta);
			v.vTangent.Normalize();

			v.vNormal.Cross(v.vTangent, v.vBinormal);
			v.vBinormal.Normalize();

			vecVtx.push_back(v);
		}
	}

	// Bottom
	v.vPos = Vec3(0.f, -fRadius, 0.f);
	v.vUV = Vec2(0.5f, 1.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = v.vPos;
	v.vNormal.Normalize();

	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 0.f, -1.f);
	vecVtx.push_back(v);

	// 인덱스
	// 북극점
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	// 몸통
	for (UINT i = 0; i < iStackCount - 2; ++i)
	{
		for (UINT j = 0; j < iSliceCount; ++j)
		{
			// + 
			// | \
			// +--+
			vecIdx.push_back((iSliceCount + 1) * (i)+(j)+1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j)+1);

			// +--+
			//  \ |
			//    +
			vecIdx.push_back((iSliceCount + 1) * (i)+(j)+1);
			vecIdx.push_back((iSliceCount + 1) * (i)+(j + 1) + 1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
		}
	}

	// 남극점
	UINT iBottomIdx = (UINT)vecVtx.size() - 1;
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(iBottomIdx);
		vecIdx.push_back(iBottomIdx - (i + 2));
		vecIdx.push_back(iBottomIdx - (i + 1));
	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(L"SphereMesh", pMesh);
	vecVtx.clear();
	vecIdx.clear();

	// Cone Mesh
	fRadius = 0.5f;
	float fHeight = 1.f;
	
	v.vPos      = Vec3(0.f, 0.f, 0.f);
	v.vUV       = Vec2(0.5f, 0.f);
	v.vColor    = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal   = Vec3(0.f, 0.f, -1.f);
	v.vTangent  = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 1.f, 0.f);
	vecVtx.push_back(v);
	
	iSliceCount = 40;
	fSliceAngle = XM_2PI / iSliceCount;
	fUVXStep    = 1.f / float(iSliceCount);
	fUVYStep    = 1.f;
	
	for (UINT i = 0; i < iSliceCount + 1; ++i)
	{
		float fTheta = i * fSliceAngle;
	
		v.vPos      = Vec3(fRadius * cosf(fTheta), fRadius * sinf(fTheta), fHeight);
		v.vUV	    = Vec2(fUVXStep * i, fUVYStep);
		v.vColor    = Vec4(1.f, 1.f, 1.f, 1.f);
		v.vNormal   = Vec3(0.f, 0.f, 1.f);
		v.vTangent  = Vec3(1.f, 0.f, 0.f);
		v.vBinormal = Vec3(0.f, 1.f, 0.f);
		vecVtx.push_back(v);
	
		if (i < iSliceCount)
		{
			vecIdx.push_back(0);
			vecIdx.push_back(i + 2);
			vecIdx.push_back(i + 1);
		}
	}
	
	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(L"ConeMesh", pMesh);
	vecVtx.clear();
	vecIdx.clear();
	
	// 원뿔의 반지름과 높이 설정
	//fRadius = 0.5f;
	//float fHeight = 1.f;
	
	// 꼭대기 정점 (Top Vertex)
	//v.vPos = Vec3(0.f, 0.f, 0.f);	 // 원뿔의 꼭대기
	//v.vUV = Vec2(0.5f, 0.f);
	//v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	//v.vNormal = Vec3(0.f, 0.f, 1.f);     // z축 기준으로 법선 설정
	//v.vTangent = Vec3(1.f, 0.f, 0.f);
	//v.vBinormal = Vec3(0.f, 1.f, 0.f);
	//vecVtx.push_back(v);
	//
	//// 슬라이스 개수 설정
	//iSliceCount = 40;
	//fSliceAngle = XM_2PI / iSliceCount;
	//fUVXStep = 1.f / float(iSliceCount);
	//fUVYStep = 1.f;
	//
	//// 밑면 정점 및 UV 좌표 생성
	//for (UINT i = 0; i <= iSliceCount; ++i)
	//{
	//	float Theta = i * fSliceAngle;
	//
	//	// 밑면 정점 계산
	//	v.vPos = Vec3(fRadius * cosf(Theta), fRadius * sinf(Theta), fHeight);
	//	v.vUV = Vec2(fUVXStep * i, fUVYStep);
	//	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	//
	//	// 법선 벡터 계산
	//	Vec3 normal = Vec3(cosf(Theta), sinf(Theta), 0.0f);  // 원뿔 측면 법선
	//	v.vNormal = normal;
	//	v.vTangent = Vec3(-sinf(Theta), cosf(Theta), 0.0f);  // 법선에 수직인 접선 벡터
	//	v.vBinormal = Vec3(0.f, 0.f, 1.f);					 // Binormal은 z축을 가리킴
	//
	//	vecVtx.push_back(v);
	//
	//	// 인덱스 버퍼 구성
	//	if (i < iSliceCount)
	//	{
	//		// 꼭지점(Top)과 밑면을 연결하는 인덱스
	//		vecIdx.push_back(0);         // 꼭지점
	//		vecIdx.push_back(i + 1);     // 현재 슬라이스 정점
	//		vecIdx.push_back(i + 2);     // 다음 슬라이스 정점 (마지막 슬라이스는 wrap-around 처리)
	//	}
	//}
	//
	//// 밑면 중심점 추가
	//v.vPos = Vec3(0.f, 0.f, 0.f);   // 밑면 중심
	//v.vUV = Vec2(0.5f, 0.5f);
	//v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	//v.vNormal = Vec3(0.f, 0.f, -1.f); // 밑면은 -z 방향으로 법선
	//v.vTangent = Vec3(1.f, 0.f, 0.f);
	//v.vBinormal = Vec3(0.f, 1.f, 0.f);
	//vecVtx.push_back(v);
	//
	//// 밑면 인덱스 추가 (마지막 정점이 밑면 중심이므로 그와 연결)
	//UINT centerIndex = (UINT)vecVtx.size() - 1;
	//for (UINT i = 1; i <= iSliceCount; ++i)
	//{
	//	vecIdx.push_back(centerIndex); // 밑면 중심
	//	vecIdx.push_back(i + 1);       // 다음 슬라이스 정점
	//	vecIdx.push_back(i);           // 현재 슬라이스 정점
	//}
	//
	//// 메쉬 생성
	//pMesh = new CMesh(true);
	//pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	//AddAsset(L"ConeMesh", pMesh);
	//
	//vecVtx.clear();
	//vecIdx.clear();
}

void CAssetMgr::CreateEngineMaterial()
{
	Ptr<CMaterial> pMtrl = nullptr;

	// Std2DMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"Std2DMtrl", pMtrl);

	// Std2DAlphaBlendMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DAlphaBlendShader"));
	AddAsset(L"Std2DAlphaBlendMtrl", pMtrl);

	// EffectMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"EffectShader"));
	AddAsset(L"EffectMtrl", pMtrl);

	// Std2DLessAlphaMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DLessAlphaBlendShader"));
	AddAsset(L"Std2DLessAlphaBlendMtrl", pMtrl);

	// Std2D Addtive Mtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DAdditiveShader"));
	AddAsset(L"Std2DAdditiveMtrl", pMtrl);

	// Std2D Addtive Mtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DAdditiveLessShader"));
	AddAsset(L"Std2DAdditiveLessMtrl", pMtrl);

	// DebugShapeMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"DebugShapeShader"));
	AddAsset(L"DebugShapeMtrl", pMtrl);

	// TileMapMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"TileMapShader"));
	AddAsset(L"TileMapMtrl", pMtrl);

	// ParticleMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"ParticleRenderShader"));
	AddAsset(L"ParticleRenderMtrl", pMtrl);

	// GrayFilterMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"GrayFilterShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"PostProcessTex"));
	pMtrl->SetTexParam(TEX_1, FindAsset<CTexture>(L"texture\\noise\\noise_01.png"));
	pMtrl->SetTexParam(TEX_2, FindAsset<CTexture>(L"texture\\noise\\noise_02.png"));
	pMtrl->SetTexParam(TEX_3, FindAsset<CTexture>(L"texture\\noise\\noise_03.jpg"));
	AddAsset(L"GrayFilterMtrl", pMtrl);

	// DistortionMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"DistortionShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"PostProcessTex"));
	pMtrl->SetTexParam(TEX_1, FindAsset<CTexture>(L"texture\\noise\\noise_01.png"));
	pMtrl->SetTexParam(TEX_2, FindAsset<CTexture>(L"texture\\noise\\noise_02.png"));
	pMtrl->SetTexParam(TEX_3, FindAsset<CTexture>(L"texture\\noise\\noise_03.jpg"));
	AddAsset(L"DistortionMtrl", pMtrl);

	// BlurMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"BlurShader"));
	AddAsset(L"BlurMtrl", pMtrl);

	// EffectMergeMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"EffectMergeShader"));
	AddAsset(L"EffectMergeMtrl", pMtrl);

	// Std3DMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std3DShader"));
	AddAsset(L"Std3DMtrl", pMtrl);

	// Std3D_DeferredMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std3D_DeferredShader"));
	AddAsset(L"Std3D_DeferredMtrl", pMtrl);

	// SkyBoxMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"SkyBoxShader"));
	AddAsset(L"SkyBoxMtrl", pMtrl);

	// DecalMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"DecalShader"));
	AddAsset(L"DecalMtrl", pMtrl);
}

void CAssetMgr::CreateEngineTexture()
{
	// PostProcess Texture 생성
	Vec2 vResolution = CDevice::GetInst()->GetResolution();
	Ptr<CTexture> pTexture = CreateTexture(L"PostProcessTex"
									    , (UINT)vResolution.x, (UINT)vResolution.y
									    , DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);

	pTexture = CreateTexture(L"CopyTexture"
		, (UINT)vResolution.x, (UINT)vResolution.y
		, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);

	Ptr<CTexture> pEffectTarget = CreateTexture(
		L"EffectTargetTex"
		, (UINT)(vResolution.x), (UINT)(vResolution.y)
		, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

	Ptr<CTexture> pEffectDepth = CreateTexture(
		L"EffectDepthStencilTex"
		, (UINT)(vResolution.x), (UINT)(vResolution.y)
		, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL);

	Ptr<CTexture> pEffectBlurTarget = CreateTexture(
		L"EffectBlurTargetTex"
		, (UINT)(vResolution.x), (UINT)(vResolution.y)
		, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

	
	// Noise Texture
	Load<CTexture>(L"texture\\noise\\noise_01.png", L"texture\\noise\\noise_01.png");
	Load<CTexture>(L"texture\\noise\\noise_02.png", L"texture\\noise\\noise_02.png");
	Load<CTexture>(L"texture\\noise\\noise_03.jpg", L"texture\\noise\\noise_03.jpg");
}

void CAssetMgr::CreateEngineSprite()
{
	// Sprite 제작
	//Ptr<CTexture> pTexture = Load<CTexture>(L"texture\\link.png", L"texture\\link.png");
	//
	//Ptr<CSprite> pSprite = nullptr;
	//
	//wstring strContentPath = CPathMgr::GetInst()->GetContentPath();
	//
	//for (int i = 0; i < 10; ++i)
	//{
	//	wchar_t szKey[50] = {};
	//	swprintf_s(szKey, 50, L"Link_MoveDown%d", i);
	//	pSprite = new CSprite;
	//
	//	pSprite->Create(pTexture, Vec2((float)i * 120.f, 520.f), Vec2(120.f, 130.f));
	//	pSprite->SetBackgroundUV(Vec2(200.f, 200.f));
	//	pSprite->SetRelativePath(wstring(L"Animation\\") + szKey + L".sprite");
	//
	//	AddAsset(szKey, pSprite);
	//	pSprite->Save(strContentPath + L"Animation\\" + szKey + L".sprite");
	//}
	//
	//Ptr<CFlipBook> pFlipBook = nullptr;
	//pFlipBook = new CFlipBook;
	//
	//for (int i = 0; i < 10; ++i)
	//{
	//	wchar_t szKey[50] = {};
	//	swprintf_s(szKey, 50, L"Link_MoveDown%d", i);
	//
	//	pFlipBook->AddSprite(FindAsset<CSprite>(szKey));
	//}
	//
	//AddAsset(L"Link_MoveDown", pFlipBook);	
	//pFlipBook->Save(strContentPath + L"Animation\\" + L"Link_MoveDown" + L".flip");

	//Ptr<CSprite> pSprite = nullptr;
	//wstring strContentPath = CPathMgr::GetInst()->GetContentPath();
	//
	//for (int i = 0; i < 10; ++i)
	//{
	//	wchar_t Buffer[50] = {};
	//	swprintf_s(Buffer, 50, L"Link_MoveDown%d", i);
	//
	//	pSprite = Load<CSprite>(Buffer, wstring(L"Animation\\") + Buffer + L".sprite");
	//
	//	pSprite->SetRelativePath(wstring(L"Animation\\") + Buffer + L".sprite");
	//	pSprite->Save(strContentPath + L"Animation\\" + Buffer + L".sprite");
	//}
	//
	//
	//Ptr<CFlipBook> pFilpBook = new CFlipBook;
	//
	//for (int i = 0; i < 10; ++i)
	//{
	//	wchar_t Buffer[50] = {};
	//	swprintf_s(Buffer, 50, L"Link_MoveDown%d", i);
	//	pFilpBook->AddSprite(FindAsset<CSprite>(Buffer));
	//}
	//
	//AddAsset(L"Link_MoveDown", pFilpBook);
	//pFilpBook->Save(strContentPath + L"Animation\\" + L"Link_MoveDown" + L".flip");
	//wstring strCotentPath = CPathMgr::GetInst()->GetContentPath();
	
	//Ptr<CAnimation> pFlipBook = new CAnimation;
	//Load<CAnimation>(L"Link_MoveDown", L"Animation\\player\\Link_MoveDown.flip");
	//Load<CAnimation>(L"Link_MoveRight", L"Animation\\player\\Link_MoveRight.flip");
	//Load<CAnimation>(L"MOVE_RIGHT", L"Animation\\player\\MOVE_RIGHT.flip");
	//Load<CAnimation>(L"IDLE", L"Animation\\player\\IDLE.flip");
	//pFlipBook->Load(strCotentPath + L"Animation\\" + L"Link_MoveDown" + L".flip");
	//
	//AddAsset(L"Link_MoveDown", pFlipBook);
}

void CAssetMgr::CreateEngineGraphicShader()
{
	Ptr<CGraphicShader> pShader = nullptr;

	// Std2DShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

	//pShader->AddScalarParam(INT_0, "Test Parameter");
	//pShader->AddScalarParam(FLOAT_1, "Test Parameter");
	//pShader->AddScalarParam(VEC2_3, "Test Parameter");
	//pShader->AddScalarParam(VEC4_2, "Test Parameter");
	pShader->AddTexParam(TEX_0, "OutputTexture");

	AddAsset(L"Std2DShader", pShader);

	// Std2DAlphaBlend
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D_Alphablend");

	pShader->SetRSType(RS_TYPE::CULL_NONE);	
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	pShader->AddTexParam(TEX_0, "OutputTexture");

	AddAsset(L"Std2DAlphaBlendShader", pShader);

	// Less AlphaBlend Shader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Less_AlphaBlend");
	
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHABLEND_COVERAGE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	AddAsset(L"Std2DLessAlphaBlendShader", pShader);

	// DNF 이미지 바탕이 검은색인것들 전용 Additive Blending
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D_Additive");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ADDITIVE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	pShader->AddTexParam(TEX_0, "tex");
	AddAsset(L"Std2DAdditiveShader", pShader);

	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D_Additive");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ADDITIVE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	pShader->AddTexParam(TEX_0, "tex");
	AddAsset(L"Std2DAdditiveLessShader", pShader);

	// EffectShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Effect");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Effect");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_EFFECT);

	AddAsset(L"EffectShader", pShader);

	// DebugShapeShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\debug.fx", "VS_DebugShape");
	pShader->CreatePixelShader(L"shader\\debug.fx", "PS_DebugShape");
		
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEBUG);

	AddAsset(L"DebugShapeShader", pShader);


	// TileMapShader
	pShader = new CGraphicShader;

	pShader->CreateVertexShader(L"shader\\tilemap.fx", "VS_TileMap");
	pShader->CreatePixelShader(L"shader\\tilemap.fx", "PS_TileMap");
	
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

	AddAsset(L"TileMapShader", pShader);

	// Particle Shader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\particle.fx", "VS_Particle");
	pShader->CreateGeometryShader(L"shader\\particle.fx", "GS_Particle");
	pShader->CreatePixelShader(L"shader\\particle.fx", "PS_Particle");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_PARTICLE);

	AddAsset(L"ParticleRenderShader", pShader);

	// PostProcess Shader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_GrayFilter");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_GrayFilter");
	
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	AddAsset(L"GrayFilterShader", pShader);

	// Distortion Shader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Distortion");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_Distortion");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	AddAsset(L"DistortionShader", pShader);

	// BlurShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Blur");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_Blur");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	AddAsset(L"BlurShader", pShader);

	// EffectMerge
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_EffectMerge");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_EffectMerge");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	AddAsset(L"EffectMergeShader", pShader);
	
	// Std3DShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\std3d.fx", "VS_Std3D");
	pShader->CreatePixelShader(L"shader\\std3d.fx", "PS_Std3D");
	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);
	pShader->AddTexParam(TEX_0, "Albedo Texture");
	pShader->AddTexParam(TEX_1, "Normal Texture");
	AddAsset(L"Std3DShader", pShader);
	
	// Std3D_DeferredShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\std3d_deferred.fx", "VS_Std3D_Deferred");
	pShader->CreatePixelShader(L"shader\\std3d_deferred.fx", "PS_Std3D_Deferred");
	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEFERRED);
	pShader->AddTexParam(TEX_0, "Albedo Texture");
	pShader->AddTexParam(TEX_1, "Normal Texture");
	AddAsset(L"Std3D_DeferredShader", pShader);

	// SkyBoxShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\skybox.fx", "VS_SkyBox");
	pShader->CreatePixelShader(L"shader\\skybox.fx", "PS_SkyBox");
	pShader->SetRSType(RS_TYPE::CULL_FRONT);
	pShader->SetDSType(DS_TYPE::LESS_EQUAL);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

	pShader->AddTexParam(TEX_0, "Albedo Texture");

	AddAsset(L"SkyBoxShader", pShader);

	// DecalShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\decal.fx", "VS_Decal");
	pShader->CreatePixelShader(L"shader\\decal.fx", "PS_Decal");
	pShader->SetRSType(RS_TYPE::CULL_FRONT);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DECAL);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DECAL);

	pShader->AddTexParam(TEX_0, "Decal Texture");

	AddAsset(L"DecalShader", pShader);
}

#include "CParticleTickCS.h"

void CAssetMgr::CreateEngineComputeShader()
{
	Ptr<CComputeShader> pCS = nullptr;
	pCS = new CParticleTickCS;
	AddAsset(L"ParticleTickCS", pCS);
}