#include "pch.h"
#include "CMesh.h"

#include "CDevice.h"
#include "CPathMgr.h"

#include "CStructuredBuffer.h"
#include "CInstancingBuffer.h"

CMesh::CMesh(bool _IsEngine)
	: CAsset(ASSET_TYPE::MESH)
	, m_VBDesc{}
	, m_VtxCount(0)
	, m_VtxSysMem(nullptr)
{
	if (_IsEngine)
		SetEngineAsset();
}

CMesh::~CMesh()
{
	if (nullptr != m_VtxSysMem)
		delete m_VtxSysMem;

	for (size_t i = 0; i < m_vecIdxInfo.size(); ++i)
	{
		if (nullptr != m_vecIdxInfo[i].pIdxSysMem)
			delete m_vecIdxInfo[i].pIdxSysMem;
	}

	SAFE_DELETE(m_pBoneFrameData);

	if (nullptr != m_pBoneInverse)
		delete m_pBoneInverse;
}


CMesh* CMesh::CreateFromContainer(CFBXLoader& _loader)
{
	const tContainer* container = &_loader.GetContainer(0);

	UINT iVtxCount = (UINT)container->vecPos.size();

	D3D11_BUFFER_DESC tVtxDesc = {};

	tVtxDesc.ByteWidth = sizeof(Vtx) * iVtxCount;
	tVtxDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	tVtxDesc.Usage = D3D11_USAGE_DEFAULT;
	if (D3D11_USAGE_DYNAMIC == tVtxDesc.Usage)
		tVtxDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA tSub = {};
	tSub.pSysMem = malloc(tVtxDesc.ByteWidth);
	Vtx* pSys = (Vtx*)tSub.pSysMem;
	for (UINT i = 0; i < iVtxCount; ++i)
	{
		pSys[i].vPos = container->vecPos[i];
		pSys[i].vUV = container->vecUV[i];
		pSys[i].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
		pSys[i].vNormal = container->vecNormal[i];
		pSys[i].vTangent = container->vecTangent[i];
		pSys[i].vBinormal = container->vecBinormal[i];
		pSys[i].vWeights = container->vecWeights[i];
		pSys[i].vIndices = container->vecIndices[i];
	}

	ComPtr<ID3D11Buffer> pVB = NULL;
	if (FAILED(DEVICE->CreateBuffer(&tVtxDesc, &tSub, pVB.GetAddressOf())))
	{
		return NULL;
	}

	CMesh* pMesh = new CMesh;
	pMesh->m_VB = pVB;
	pMesh->m_VBDesc = tVtxDesc;
	pMesh->m_VtxSysMem = pSys;

	// 인덱스 정보
	UINT iIdxBufferCount = (UINT)container->vecIdx.size();
	D3D11_BUFFER_DESC tIdxDesc = {};

	for (UINT i = 0; i < iIdxBufferCount; ++i)
	{
		tIdxDesc.ByteWidth = (UINT)container->vecIdx[i].size() * sizeof(UINT); // Index Format 이 R32_UINT 이기 때문
		tIdxDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		tIdxDesc.Usage = D3D11_USAGE_DEFAULT;
		if (D3D11_USAGE_DYNAMIC == tIdxDesc.Usage)
			tIdxDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		void* pSysMem = malloc(tIdxDesc.ByteWidth);
		memcpy(pSysMem, container->vecIdx[i].data(), tIdxDesc.ByteWidth);
		tSub.pSysMem = pSysMem;

		ComPtr<ID3D11Buffer> pIB = nullptr;
		if (FAILED(DEVICE->CreateBuffer(&tIdxDesc, &tSub, pIB.GetAddressOf())))
		{
			return NULL;
		}

		tIndexInfo info = {};
		info.tIBDesc = tIdxDesc;
		info.iIdxCount = (UINT)container->vecIdx[i].size();
		info.pIdxSysMem = pSysMem;
		info.pIB = pIB;

		pMesh->m_vecIdxInfo.push_back(info);
	}

	// Animation3D
	if (!container->bAnimation)
		return pMesh;

	vector<tBone*>& vecBone = _loader.GetBones();
	UINT iFrameCount = 0;

	for (UINT i = 0; i < vecBone.size(); ++i)
	{
		tMTBone bone = {};
		bone.iDepth = vecBone[i]->iDepth;
		bone.iParentIndx = vecBone[i]->iParentIndx;
		bone.matBone = GetMatrixFromFbxMatrix(vecBone[i]->matBone);
		bone.matOffset = GetMatrixFromFbxMatrix(vecBone[i]->matOffset);
		bone.strBoneName = vecBone[i]->strBoneName;

		for (UINT j = 0; j < vecBone[i]->vecKeyFrame.size(); ++j)
		{
			tMTKeyFrame tKeyframe = {};
			tKeyframe.dTime = vecBone[i]->vecKeyFrame[j].dTime;
			tKeyframe.iFrame = j;
			tKeyframe.vTranslate.x = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetT().mData[0];
			tKeyframe.vTranslate.y = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetT().mData[1];
			tKeyframe.vTranslate.z = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetT().mData[2];

			tKeyframe.vScale.x = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetS().mData[0];
			tKeyframe.vScale.y = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetS().mData[1];
			tKeyframe.vScale.z = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetS().mData[2];

			tKeyframe.qRot.x = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetQ().mData[0];
			tKeyframe.qRot.y = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetQ().mData[1];
			tKeyframe.qRot.z = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetQ().mData[2];
			tKeyframe.qRot.w = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetQ().mData[3];

			bone.vecKeyFrame.push_back(tKeyframe);
		}

		iFrameCount = max(iFrameCount, (UINT)bone.vecKeyFrame.size());

		pMesh->m_vecBones.push_back(bone);
	}

	vector<tAnimClip*>& vecAnimClip = _loader.GetAnimClip();

	for (UINT i = 0; i < vecAnimClip.size(); ++i)
	{
		tMTAnimClip tClip = {};

		tClip.strAnimName = vecAnimClip[i]->strName;
		tClip.dStartTime = vecAnimClip[i]->tStartTime.GetSecondDouble();
		tClip.dEndTime = vecAnimClip[i]->tEndTime.GetSecondDouble();
		tClip.dTimeLength = tClip.dEndTime - tClip.dStartTime;

		tClip.iStartFrame = (int)vecAnimClip[i]->tStartTime.GetFrameCount(vecAnimClip[i]->eMode);
		tClip.iEndFrame = (int)vecAnimClip[i]->tEndTime.GetFrameCount(vecAnimClip[i]->eMode);
		tClip.iFrameLength = tClip.iEndFrame - tClip.iStartFrame;
		tClip.eMode = vecAnimClip[i]->eMode;

		pMesh->m_vecAnimClip.push_back(tClip);
	}

	// Animation 이 있는 Mesh 경우 structuredbuffer 만들어두기
	if (pMesh->IsAnimMesh())
	{
		// Bone Offset 행렬
		vector<Matrix> vecOffset;
		for (size_t i = 0; i < pMesh->m_vecBones.size(); ++i)
		{
			vecOffset.push_back(pMesh->m_vecBones[i].matOffset);
		}

		// 클립별로 Frame Data 저장
		size_t clipsize = pMesh->m_vecAnimClip.size();
		vector<tFrameTrans> vecFrameTrans;
		for (size_t clipIdx = 0; clipIdx < clipsize; ++clipIdx)
		{
			//const tMTAnimClip& clip = pMesh->m_vecAnimClip[clipIdx];
			vecFrameTrans.resize((UINT)pMesh->m_vecBones.size() * iFrameCount);

			size_t bonesize = pMesh->m_vecBones.size();

			for (size_t i = 0; i < pMesh->m_vecBones.size(); ++i)
			{
				size_t keyframesize = pMesh->m_vecBones[i].vecKeyFrame.size();

				for (size_t j = 0; j < keyframesize; ++j)
				{
					size_t index = pMesh->m_vecBones.size() * j + i;

					if (index >= vecFrameTrans.size())
						continue;

					vecFrameTrans[index] = tFrameTrans{	Vec4(pMesh->m_vecBones[i].vecKeyFrame[j].vTranslate, 0.f),
														Vec4(pMesh->m_vecBones[i].vecKeyFrame[j].vScale, 0.f),
															 pMesh->m_vecBones[i].vecKeyFrame[j].qRot };
				}
			}

			// 클립별 구조화 버퍼 생성
			//CStructuredBuffer* pBoneFrameData = new CStructuredBuffer;
			//pBoneFrameData->Create(sizeof(tFrameTrans)
			//					, (UINT)vecFrameTrans.size()
			//					, SB_TYPE::SRV_ONLY
			//					, false
			//					, vecFrameTrans.data());
			//
			//// 벡터에 구조화 버퍼 추가
			//pMesh->m_pVecBoneFrameData.push_back(pBoneFrameData);
		}

		// Inverse Offset 데이터 생성
		pMesh->m_pBoneInverse = new CStructuredBuffer;
		pMesh->m_pBoneInverse->Create(sizeof(Matrix)
									, (UINT)vecOffset.size()
									, SB_TYPE::SRV_ONLY
									, false
									, vecOffset.data());

		pMesh->m_pBoneFrameData = new CStructuredBuffer;
		pMesh->m_pBoneFrameData->Create(sizeof(tFrameTrans), (UINT)vecOffset.size()* iFrameCount
			, SB_TYPE::SRV_ONLY, false, vecFrameTrans.data());
	}

	return pMesh;
}

int CMesh::Create(Vtx* _VtxSysMem, UINT _VtxCount, UINT* _IdxSysMem, UINT _IdxCount)
{
	// 버텍스 개수
	m_VtxCount = (UINT)_VtxCount;

	// 버텍스 시스템 메모리
	m_VtxSysMem = new Vtx[m_VtxCount];
	memcpy(m_VtxSysMem, _VtxSysMem, sizeof(Vtx) * m_VtxCount);

	// 버텍스 버퍼 생성
	m_VBDesc.ByteWidth = sizeof(Vtx) * m_VtxCount;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// Vertex 버퍼가 생성된 이후에도 데이터 쓰기가 가능하도록 설정
	m_VBDesc.Usage = D3D11_USAGE_DEFAULT;
	m_VBDesc.CPUAccessFlags = 0;

	m_VBDesc.MiscFlags = 0;
	m_VBDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA tSub = {};
	tSub.pSysMem = _VtxSysMem;

	if (FAILED(DEVICE->CreateBuffer(&m_VBDesc, &tSub, m_VB.GetAddressOf())))
	{
		MessageBox(nullptr, L"VertexBuffer 생성 실패", L"Mesh 생성 실패", MB_OK);
		return E_FAIL;
	}

	// 인덱스 버퍼 생성
	tIndexInfo IndexInfo = {};
	IndexInfo.iIdxCount = _IdxCount;

	IndexInfo.tIBDesc.ByteWidth = sizeof(UINT) * _IdxCount;

	// 버퍼 생성 이후에도, 버퍼의 내용을 수정 할 수 있는 옵션
	IndexInfo.tIBDesc.CPUAccessFlags = 0;
	IndexInfo.tIBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

	// 정점을 저장하는 목적의 버퍼 임을 알림
	IndexInfo.tIBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	IndexInfo.tIBDesc.MiscFlags = 0;
	IndexInfo.tIBDesc.StructureByteStride = 0;

	// 초기 데이터를 넘겨주기 위한 정보 구조체	
	tSub.pSysMem = _IdxSysMem;

	if (FAILED(DEVICE->CreateBuffer(&IndexInfo.tIBDesc, &tSub, IndexInfo.pIB.GetAddressOf())))
	{
		assert(nullptr);
	}

	// 시스템 메모리에 저장
	IndexInfo.pIdxSysMem = new UINT[IndexInfo.iIdxCount];
	memcpy(IndexInfo.pIdxSysMem, _IdxSysMem, sizeof(UINT) * IndexInfo.iIdxCount);

	m_vecIdxInfo.push_back(IndexInfo);

	return S_OK;
}

void CMesh::Binding(UINT _iSubset)
{
	UINT stride = sizeof(Vtx);
	UINT offset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &stride, &offset);
	CONTEXT->IASetIndexBuffer(m_vecIdxInfo[_iSubset].pIB.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void CMesh::Binding_Inst(UINT _iSubset)
{
	if (_iSubset >= m_vecIdxInfo.size())
		assert(nullptr);

	ID3D11Buffer* arrBuffer[2] = { m_VB.Get(), CInstancingBuffer::GetInst()->GetBuffer().Get() };
	UINT		  iStride[2] = { sizeof(Vtx), sizeof(tInstancingData) };
	UINT		  iOffset[2] = { 0, 0 };

	CONTEXT->IASetVertexBuffers(0, 2, arrBuffer, iStride, iOffset);
	CONTEXT->IASetIndexBuffer(m_vecIdxInfo[_iSubset].pIB.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void CMesh::Render(UINT _Subset)
{
	Binding(_Subset);

	CONTEXT->DrawIndexed(m_vecIdxInfo[_Subset].iIdxCount, 0, 0);
}

void CMesh::Render_Particle(UINT _Count)
{
	Binding(0);

	CONTEXT->DrawIndexedInstanced(m_vecIdxInfo[0].iIdxCount, _Count, 0, 0, 0);
}

void CMesh::Render_Instancing(UINT _Subset)
{
	Binding_Inst(_Subset);

	CONTEXT->DrawIndexedInstanced(m_vecIdxInfo[_Subset].iIdxCount
		, CInstancingBuffer::GetInst()->GetInstanceCount(), 0, 0, 0);
}

int CMesh::Save(const wstring& _FilePath)
{
	// 상대경로 저장
	wstring strRelativePath = CPathMgr::GetInst()->GetRelativePath(_FilePath);
	SetRelativePath(strRelativePath);

	// 파일 쓰기모드로 열기
	FILE* pFile = nullptr;
	errno_t err = _wfopen_s(&pFile, _FilePath.c_str(), L"wb");
	assert(pFile);

	// 키값, 상대 경로	
	SaveWString(GetName(), pFile);
	SaveWString(GetKey(), pFile);
	SaveWString(GetRelativePath(), pFile);

	// 정점 데이터 저장				
	int iByteSize = m_VBDesc.ByteWidth;
	fwrite(&iByteSize, sizeof(int), 1, pFile);
	fwrite(m_VtxSysMem, iByteSize, 1, pFile);

	// 인덱스 정보
	UINT iMtrlCount = (UINT)m_vecIdxInfo.size();
	fwrite(&iMtrlCount, sizeof(int), 1, pFile);

	UINT iIdxBuffSize = 0;
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		fwrite(&m_vecIdxInfo[i], sizeof(tIndexInfo), 1, pFile);
		fwrite(m_vecIdxInfo[i].pIdxSysMem
			, m_vecIdxInfo[i].iIdxCount * sizeof(UINT)
			, 1, pFile);
	}

	// Animation3D 정보 
	UINT iCount = (UINT)m_vecAnimClip.size();
	fwrite(&iCount, sizeof(int), 1, pFile);
	for (UINT i = 0; i < iCount; ++i)
	{
		SaveWString(m_vecAnimClip[i].strAnimName, pFile);
		fwrite(&m_vecAnimClip[i].dStartTime, sizeof(double), 1, pFile);
		fwrite(&m_vecAnimClip[i].dEndTime, sizeof(double), 1, pFile);
		fwrite(&m_vecAnimClip[i].dTimeLength, sizeof(double), 1, pFile);
		fwrite(&m_vecAnimClip[i].eMode, sizeof(int), 1, pFile);
		fwrite(&m_vecAnimClip[i].fUpdateTime, sizeof(float), 1, pFile);
		fwrite(&m_vecAnimClip[i].iStartFrame, sizeof(int), 1, pFile);
		fwrite(&m_vecAnimClip[i].iEndFrame, sizeof(int), 1, pFile);
		fwrite(&m_vecAnimClip[i].iFrameLength, sizeof(int), 1, pFile);
	}

	iCount = (UINT)m_vecBones.size();
	fwrite(&iCount, sizeof(int), 1, pFile);

	for (UINT i = 0; i < iCount; ++i)
	{
		SaveWString(m_vecBones[i].strBoneName, pFile);
		fwrite(&m_vecBones[i].iDepth, sizeof(int), 1, pFile);
		fwrite(&m_vecBones[i].iParentIndx, sizeof(int), 1, pFile);
		fwrite(&m_vecBones[i].matBone, sizeof(Matrix), 1, pFile);
		fwrite(&m_vecBones[i].matOffset, sizeof(Matrix), 1, pFile);

		int iFrameCount = (int)m_vecBones[i].vecKeyFrame.size();
		fwrite(&iFrameCount, sizeof(int), 1, pFile);

		for (int j = 0; j < m_vecBones[i].vecKeyFrame.size(); ++j)
		{
			fwrite(&m_vecBones[i].vecKeyFrame[j], sizeof(tMTKeyFrame), 1, pFile);
		}
	}

	fclose(pFile);

	return S_OK;
}

int CMesh::Load(const wstring& _FilePath)
{
	// 읽기모드로 파일열기
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"rb");

	// 키값, 상대경로
	wstring strName, strKey, strRelativePath;
	LoadWString(strName, pFile);
	LoadWString(strKey, pFile);
	LoadWString(strRelativePath, pFile);

	SetName(strName);
	SetKey(strKey);
	SetRelativePath(strRelativePath);

	// 정점데이터
	UINT iByteSize = 0;
	fread(&iByteSize, sizeof(int), 1, pFile);

	m_VtxSysMem = (Vtx*)malloc(iByteSize);
	fread(m_VtxSysMem, 1, iByteSize, pFile);


	D3D11_BUFFER_DESC tDesc = {};
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tDesc.ByteWidth = iByteSize;
	tDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA tSubData = {};
	tSubData.pSysMem = m_VtxSysMem;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tSubData, m_VB.GetAddressOf())))
	{
		assert(nullptr);
	}

	// 인덱스 정보
	UINT iMtrlCount = 0;
	fread(&iMtrlCount, sizeof(int), 1, pFile);

	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		tIndexInfo info = {};
		fread(&info, sizeof(tIndexInfo), 1, pFile);

		UINT iByteWidth = info.iIdxCount * sizeof(UINT);

		void* pSysMem = malloc(iByteWidth);
		info.pIdxSysMem = pSysMem;
		fread(info.pIdxSysMem, iByteWidth, 1, pFile);

		tSubData.pSysMem = info.pIdxSysMem;

		if (FAILED(DEVICE->CreateBuffer(&info.tIBDesc, &tSubData, info.pIB.GetAddressOf())))
		{
			assert(nullptr);
		}

		m_vecIdxInfo.push_back(info);
	}

	// Animation3D 정보 읽기
	int iCount = 0;
	fread(&iCount, sizeof(int), 1, pFile);
	for (int i = 0; i < iCount; ++i)
	{
		tMTAnimClip tClip = {};

		LoadWString(tClip.strAnimName, pFile);
		fread(&tClip.dStartTime, sizeof(double), 1, pFile);
		fread(&tClip.dEndTime, sizeof(double), 1, pFile);
		fread(&tClip.dTimeLength, sizeof(double), 1, pFile);
		fread(&tClip.eMode, sizeof(int), 1, pFile);
		fread(&tClip.fUpdateTime, sizeof(float), 1, pFile);
		fread(&tClip.iStartFrame, sizeof(int), 1, pFile);
		fread(&tClip.iEndFrame, sizeof(int), 1, pFile);
		fread(&tClip.iFrameLength, sizeof(int), 1, pFile);

		m_vecAnimClip.push_back(tClip);
	}

	iCount = 0;
	fread(&iCount, sizeof(int), 1, pFile);
	m_vecBones.resize(iCount);

	UINT _iFrameCount = 0;
	for (int i = 0; i < iCount; ++i)
	{
		LoadWString(m_vecBones[i].strBoneName, pFile);
		fread(&m_vecBones[i].iDepth, sizeof(int), 1, pFile);
		fread(&m_vecBones[i].iParentIndx, sizeof(int), 1, pFile);
		fread(&m_vecBones[i].matBone, sizeof(Matrix), 1, pFile);
		fread(&m_vecBones[i].matOffset, sizeof(Matrix), 1, pFile);

		UINT iFrameCount = 0;
		fread(&iFrameCount, sizeof(int), 1, pFile);
		m_vecBones[i].vecKeyFrame.resize(iFrameCount);
		_iFrameCount = max(_iFrameCount, iFrameCount);
		for (UINT j = 0; j < iFrameCount; ++j)
		{
			fread(&m_vecBones[i].vecKeyFrame[j], sizeof(tMTKeyFrame), 1, pFile);
		}
	}

	// Animation 이 있는 Mesh 경우 Bone StructuredBuffer 만들기
	if (m_vecAnimClip.size() > 0 && m_vecBones.size() > 0)
	{
		wstring strBone = GetName();

		//// BoneOffet 행렬
		//vector<Matrix> vecOffset;
		//vector<tFrameTrans> vecFrameTrans;
		//vecFrameTrans.resize((UINT)m_vecBones.size() * _iFrameCount);
		//
		//for (size_t i = 0; i < m_vecBones.size(); ++i)
		//{
		//	vecOffset.push_back(m_vecBones[i].matOffset);
		//
		//	for (size_t j = 0; j < m_vecBones[i].vecKeyFrame.size(); ++j)
		//	{
		//		vecFrameTrans[(UINT)m_vecBones.size() * j + i]
		//			= tFrameTrans{ Vec4(m_vecBones[i].vecKeyFrame[j].vTranslate, 0.f)
		//			, Vec4(m_vecBones[i].vecKeyFrame[j].vScale, 0.f)
		//			, Vec4(m_vecBones[i].vecKeyFrame[j].qRot) };
		//	}
		//}

		//m_pBoneInverse = new CStructuredBuffer;
		//m_pBoneInverse->Create(sizeof(Matrix), (UINT)vecOffset.size(), SB_TYPE::SRV_ONLY, false, vecOffset.data());
		//
		//m_pBoneFrameData = new CStructuredBuffer;
		//m_pBoneFrameData->Create(sizeof(tFrameTrans), (UINT)vecOffset.size() * (UINT)_iFrameCount
		//	, SB_TYPE::SRV_ONLY, false, vecFrameTrans.data());

		// Bone Offset 행렬
		vector<Matrix> vecOffset;
		for (size_t i = 0; i < m_vecBones.size(); ++i)
		{
			vecOffset.push_back(m_vecBones[i].matOffset);
		}

		// 클립별로 Frame Data 저장
		size_t clipsize = m_vecAnimClip.size();
		vector<tFrameTrans> vecFrameTrans;
		for (size_t clipIdx = 0; clipIdx < clipsize; ++clipIdx)
		{
			const tMTAnimClip& clip = m_vecAnimClip[clipIdx];
			
			vecFrameTrans.resize((UINT)m_vecBones.size() * _iFrameCount);

			size_t bonesize = m_vecBones.size();

			for (size_t i = 0; i < m_vecBones.size(); ++i)
			{
				size_t keyframesize = m_vecBones[i].vecKeyFrame.size();

				for (size_t j = 0; j < keyframesize; ++j)
				{
					size_t index = m_vecBones.size() * j + i;

					if (index >= vecFrameTrans.size())
						continue;

					vecFrameTrans[index] = tFrameTrans{ Vec4(m_vecBones[i].vecKeyFrame[j].vTranslate, 0.f),
														Vec4(m_vecBones[i].vecKeyFrame[j].vScale, 0.f),
															 m_vecBones[i].vecKeyFrame[j].qRot };
				}
			}

		

			// 벡터에 구조화 버퍼 추가
			//m_pVecBoneFrameData.push_back(pBoneFrameData);
		}

		// 클립별 구조화 버퍼 생성
		//CStructuredBuffer* pBoneFrameData = new CStructuredBuffer;
		//pBoneFrameData->Create(sizeof(tFrameTrans)
		//	, (UINT)vecFrameTrans.size()
		//	, SB_TYPE::SRV_ONLY
		//	, false
		//	, vecFrameTrans.data());

		// Inverse Offset 데이터 생성
		m_pBoneInverse = new CStructuredBuffer;
		m_pBoneInverse->Create(sizeof(Matrix)
			, (UINT)vecOffset.size()
			, SB_TYPE::SRV_ONLY
			, false
			, vecOffset.data());

		m_pBoneFrameData = new CStructuredBuffer;
		m_pBoneFrameData->Create(sizeof(tFrameTrans), (UINT)vecOffset.size() * (UINT)_iFrameCount
			, SB_TYPE::SRV_ONLY, false, vecFrameTrans.data());
	}

	fclose(pFile);

	return S_OK;
}