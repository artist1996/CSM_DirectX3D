#include "pch.h"
#include "CSprite.h"
#include "CAssetMgr.h"

CSprite::CSprite()
	: CAsset(ASSET_TYPE::SPRITE)
{
}

CSprite::~CSprite()
{
}

void CSprite::SetLeftTopUV(Vec2 _LeftTop)
{
	Vec2 AtlasResolution = Vec2((float)m_Atlas->Width(), (float)m_Atlas->Height());
	m_LeftTopUV = _LeftTop / AtlasResolution;
}

void CSprite::SetSliceUV(Vec2 _Slice)
{
	Vec2 AtlasResolution = Vec2((float)m_Atlas->Width(), (float)m_Atlas->Height());
	m_SliceUV = _Slice / AtlasResolution;
}

void CSprite::SetBackgroundUV(Vec2 _Background)
{
	Vec2 AtlasResolution = Vec2((float)m_Atlas->Width(), (float)m_Atlas->Height());
	m_BackgroundUV = _Background / AtlasResolution;
}

void CSprite::SetOffsetUV(Vec2 _Offset)
{
	Vec2 AtlasResolution = Vec2((float)m_Atlas->Width(), (float)m_Atlas->Height());
	m_OffsetUV = _Offset / AtlasResolution;
}

void CSprite::Create(Ptr<CTexture> _Atlas, Vec2 _LeftTopPixel, Vec2 _SlicePixel)
{
	m_Atlas = _Atlas;

	assert(m_Atlas);

	UINT Width = m_Atlas->Width();
	UINT Height = m_Atlas->Height();

	m_LeftTopUV = Vec2(_LeftTopPixel.x / (float)Width, _LeftTopPixel.y / (float)Height);
	m_SliceUV = Vec2(_SlicePixel.x / (float)Width, _SlicePixel.y / (float)Height);
}

int CSprite::Save(const wstring& _strRelativePath)
{
	SetRelativePath(_strRelativePath);
	SetKey(_strRelativePath);

	// 파일 개방
	FILE* pFile = nullptr;

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");

	if (nullptr == pFile)
		return E_FAIL;

	// Atlas 는 포인터 이니 저장해봤자 무쓸모 따라서 참조중인 Texture 의 Key 값을 저장
	SaveAssetRef(m_Atlas, pFile);

	// Vector2 자료형은 그냥 저장 시키면 끝
	fwrite(&m_LeftTopUV, sizeof(Vec2), 1, pFile);
	fwrite(&m_SliceUV, sizeof(Vec2), 1, pFile);
	fwrite(&m_BackgroundUV, sizeof(Vec2), 1, pFile);
	fwrite(&m_OffsetUV, sizeof(Vec2), 1, pFile);

	fclose(pFile);

	return S_OK;
}

int CSprite::Load(const wstring& _FilePath)
{
	// 파일 개방
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, _FilePath.c_str(), L"rb");

	if (nullptr == pFile)
		return E_FAIL;
	
	wstring Key = {};
	LoadAssetRef(m_Atlas, pFile);

	fread(&m_LeftTopUV, sizeof(Vec2), 1, pFile);
	fread(&m_SliceUV, sizeof(Vec2), 1, pFile);
	fread(&m_BackgroundUV, sizeof(Vec2), 1, pFile);
	fread(&m_OffsetUV, sizeof(Vec2), 1, pFile);

	fclose(pFile);

	return S_OK;
}