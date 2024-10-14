#include "pch.h"
#include "CTileMap.h"

#include "CKeyMgr.h"
#include "CAssetMgr.h"
#include "CTransform.h"

#include "CStructuredBuffer.h"

CTileMap::CTileMap()
	: CRenderComponent(COMPONENT_TYPE::TILEMAP)
	, m_TileAtlas(nullptr)
	, m_Buffer(nullptr)
	, m_Row(1)
	, m_Col(1)
	, m_AtlasMaxRow(0)
	, m_AtlasMaxCol(0)
{
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"TileMapMtrl"));

	m_Buffer = new CStructuredBuffer;
}

CTileMap::CTileMap(const CTileMap& _Origin)
	: CRenderComponent(_Origin)
	, m_Row(_Origin.m_Row)
	, m_Col(_Origin.m_Col)
	, m_TileSize(_Origin.m_TileSize)
	, m_TileAtlas(_Origin.m_TileAtlas)
	, m_AtlasResolution(_Origin.m_AtlasResolution)
	, m_AtlasTileSize(_Origin.m_AtlasTileSize)
	, m_AtlasSliceUV(_Origin.m_AtlasSliceUV)
	, m_AtlasMaxRow(_Origin.m_AtlasMaxRow)
	, m_AtlasMaxCol(_Origin.m_AtlasMaxCol)
	, m_vecTileInfo(_Origin.m_vecTileInfo)
	, m_Buffer(nullptr)
{
	m_Buffer = new CStructuredBuffer;
}

CTileMap::~CTileMap()
{
	SAFE_DELETE(m_Buffer);
}

void CTileMap::FinalTick()
{
}

void CTileMap::Render()
{
	if (m_vecTileInfo.empty())
		return;

	m_Buffer->SetData(m_vecTileInfo.data(), sizeof(tTileInfo) * m_Row * m_Col);
	m_Buffer->Binding(15);

	GetMaterial()->SetTexParam(TEX_0, m_TileAtlas);
	GetMaterial()->SetScalarParam(INT_1, m_AtlasMaxRow);
	GetMaterial()->SetScalarParam(INT_2, m_AtlasMaxCol);
	GetMaterial()->SetScalarParam(VEC2_0, m_AtlasSliceUV);
	GetMaterial()->SetScalarParam(VEC2_1, Vec2((float)m_Col, (float)m_Row));

	GetMaterial()->Binding();
	Transform()->Binding();
	GetMesh()->Render();
}

void CTileMap::Init()
{
	SetRowCol(m_Row, m_Col);
}

void CTileMap::SetRowCol(UINT _Row, UINT _Col)
{
	// Row 와 Col 이 바뀌면 TileMap 크기도 변해야한다.
	m_Row = _Row;
	m_Col = _Col;;

	ChangeTileMapSize();
	
	// 타일 개수
	UINT TileCount = m_Row * m_Col;

	// 타일 정보를 저장하는 벡터 사이즈가 타일 카운트보다 사이즈가 작다면 resize 해준다.
	if (m_vecTileInfo.size() != TileCount)
	{
		m_vecTileInfo.clear();
		m_vecTileInfo.resize(TileCount);
	}

	// 타일 정보를 전달받아서 t 레지스터에 전달시킬 구조화 버퍼가 타일 전체 데이터 사이즈 보다 작으면 리사이즈
	if (m_Buffer->GetElementCount() < TileCount)
	{
		m_Buffer->Create(sizeof(tTileInfo), TileCount, SB_TYPE::SRV_ONLY);
	}
}

void CTileMap::SetTileSize(Vec2 _Size)
{
	// TileSize 가 변경되면 TileMap 크기도 변해야한다.
	m_TileSize = _Size;
	
	ChangeTileMapSize();
}

void CTileMap::ChangeTileMapSize()
{
	Vec2 vSize = m_TileSize * Vec2((float)m_Col, (float)m_Row);	
	Transform()->SetRelativeScale(Vec3(vSize.x, vSize.y, 1.f));
}

void CTileMap::SetAtlasTexture(Ptr<CTexture> _Atlas)
{
	m_TileAtlas = _Atlas;

	if (nullptr == m_TileAtlas)
		m_AtlasResolution = Vec2(0.f, 0.f);
	else
		m_AtlasResolution = Vec2((float)m_TileAtlas->Width(), (float)m_TileAtlas->Height());

	SetAtlasTileSize(m_AtlasTileSize);
}

void CTileMap::SetAtlasTileSize(Vec2 _TileSize)
{
	m_AtlasTileSize = _TileSize;

	if (nullptr != m_TileAtlas)
	{
		m_AtlasSliceUV = m_AtlasTileSize / m_AtlasResolution;

		m_AtlasMaxCol = int(m_AtlasResolution.x / m_AtlasTileSize.x);
		m_AtlasMaxRow = int(m_AtlasResolution.y / m_AtlasTileSize.y);
	}
}

void CTileMap::SaveToFile(FILE* _pFile)
{
	SaveDataToFile(_pFile);
	
	fwrite(&m_Row, sizeof(int), 1, _pFile);
	fwrite(&m_Col, sizeof(int), 1, _pFile);

	fwrite(&m_TileSize, sizeof(Vec2), 1, _pFile);
	fwrite(&m_AtlasTileSize, sizeof(Vec2), 1, _pFile);

	for (size_t i = 0; i < m_vecTileInfo.size(); ++i)
	{
		fwrite(&m_vecTileInfo[i], sizeof(tTileInfo), 1, _pFile);
	}

	// Atlas Texture
	SaveAssetRef(m_TileAtlas, _pFile);
}

void CTileMap::LoadFromFile(FILE* _pFile)
{
	LoadDataFromFile(_pFile);

	fread(&m_Row, sizeof(int), 1, _pFile);
	fread(&m_Col, sizeof(int), 1, _pFile);

	SetRowCol(m_Row, m_Col);

	fread(&m_TileSize, sizeof(Vec2), 1, _pFile);
	fread(&m_AtlasTileSize, sizeof(Vec2), 1, _pFile);

	SetTileSize(m_TileSize);

	for (size_t i = 0; i < m_vecTileInfo.size(); ++i)
	{
		fread(&m_vecTileInfo[i], sizeof(tTileInfo), 1, _pFile);
	}

	// Atlas Texture
	LoadAssetRef(m_TileAtlas, _pFile);

	if (nullptr != m_TileAtlas)
	{
		SetAtlasTexture(m_TileAtlas);
	}
}