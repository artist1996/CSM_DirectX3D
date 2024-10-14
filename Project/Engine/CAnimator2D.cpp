#include "pch.h"
#include "CAnimator2D.h"

#include "CTimeMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CAssetMgr.h"

#include "CSprite.h"

CAnimator2D::CAnimator2D()
	: CComponent(COMPONENT_TYPE::ANIMATOR2D)
	, m_CurAnimation(nullptr) 
	, m_CurFrmSprite(nullptr)
	, m_CurFrmIdx(0)
	, m_MaxFrm(0)
	, m_FPS(0.f)
	, m_AccTime(0.f)
	, m_Repeat(false)
	, m_Finish(false)
{
}

CAnimator2D::CAnimator2D(const CAnimator2D& _Origin)
	: CComponent(_Origin)
	, m_vecAnimation(_Origin.m_vecAnimation)
	, m_CurAnimation(_Origin.m_CurAnimation)
	, m_CurFrmIdx(0)
	, m_FPS(_Origin.m_FPS)
	, m_AccTime(0.f)
	, m_Repeat(_Origin.m_Repeat)
	, m_Finish(false)
{
	if (nullptr != m_CurAnimation)
	{
		int FlipBookIdx = 0;
		for (; FlipBookIdx < m_vecAnimation.size(); ++FlipBookIdx)
		{
			if (m_CurAnimation == m_vecAnimation[FlipBookIdx])
			{
				break;
			}
		}
		Play(FlipBookIdx, m_FPS, m_Repeat);
	}
}

CAnimator2D::~CAnimator2D()
{
}

void CAnimator2D::FinalTick()
{
	if (m_Finish)
	{
		if (!m_Repeat)
			return;

		Reset();
	}

	if (nullptr != m_CurAnimation)
	{
		float MaxTime = 1.f / m_FPS;
		
		m_AccTime += EngineDT;
		
		if (MaxTime < m_AccTime)
		{
			m_AccTime -= MaxTime;
			++m_CurFrmIdx;

			if (m_CurAnimation->GetMaxFrameCount() <= m_CurFrmIdx)
			{
				--m_CurFrmIdx;
				m_Finish = true;
			}
		}

		m_CurFrmSprite = m_CurAnimation->GetSprite(m_CurFrmIdx);
	}
}

void CAnimator2D::AddAnimation(int _Idx, Ptr<CAnimation> _FlipBook)
{
	if(m_vecAnimation.size() <= _Idx)
		m_vecAnimation.resize(_Idx + 1);

	m_vecAnimation[_Idx] = _FlipBook;
}

Ptr<CAnimation> CAnimator2D::FindAnimation(const wstring& _Key)
{
	for (size_t i = 0; i < m_vecAnimation.size(); ++i)
	{
		if (m_vecAnimation[i]->GetKey() == _Key)
			return m_vecAnimation[i];
	}

	return nullptr;
}

void CAnimator2D::Play(int _FlipBookIdx, float _FPS, bool _Repeat)
{
	m_CurAnimation = m_vecAnimation[_FlipBookIdx];
	
	if (nullptr == m_CurAnimation)
		return;

	m_CurFrmIdx = 0;
	m_AccTime = 0.f;
	m_FPS = _FPS;
	m_Repeat = _Repeat;
}

void CAnimator2D::Stop()
{
	m_FPS = 0;
}

void CAnimator2D::Reset()
{
	m_CurFrmIdx = 0;
	m_AccTime = 0.f;
	m_Finish = false;
}

void CAnimator2D::Binding()
{
	tSpriteInfo tInfo = {};

	if (nullptr != m_CurFrmSprite)
	{
		// 현재 표시해야하는 Sprite 의 정보를 Sprite 전용 상수버퍼를 통해서 GPU 에 전달
		tSpriteInfo tInfo = {};

		tInfo.vLeftTopUV = m_CurFrmSprite->GetLeftTopUV();
		tInfo.vSliceUV = m_CurFrmSprite->GetSliceUV();
		tInfo.vBackGroundUV = m_CurFrmSprite->GetBackgroundUV();
		tInfo.vOffsetUV = m_CurFrmSprite->GetOffsetUV();
		tInfo.UseFlipBook = 1;

		static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::SPRITE);

		pCB->SetData(&tInfo);
		pCB->Binding();

		// FlipBook Sprite 아틀라스 텍스쳐 전용 Register 번호 t10에 Binding
		Ptr<CTexture> pAtlas = m_CurFrmSprite->GetAtlasTexture();
		pAtlas->Binding(10);
	}
	
	else
	{
		Clear();
	}
}

void CAnimator2D::erase(int _Idx)
{
	m_vecAnimation.erase(m_vecAnimation.begin() + _Idx);
}

void CAnimator2D::Clear()
{
	tSpriteInfo tInfo = {};
	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::SPRITE);
	pCB->SetData(&tInfo);
	pCB->Binding();
}

void CAnimator2D::SaveToFile(FILE* _pFile)
{
	size_t AnimationCount = m_vecAnimation.size();

	fwrite(&AnimationCount, sizeof(size_t), 1, _pFile);

	for (size_t i = 0; i < AnimationCount; ++i)
	{
		SaveAssetRef(m_vecAnimation[i], _pFile);
	}

	// 현재 재생중인 Animation 정보 저장
	SaveAssetRef(m_CurAnimation, _pFile);

	// 현재 재생중인 Animation 의 현재 Frame Sprite 정보 저장
	SaveAssetRef(m_CurFrmSprite, _pFile);
	
	fwrite(&m_CurFrmIdx, sizeof(int), 1, _pFile);
	fwrite(&m_FPS, sizeof(float), 1, _pFile);
	fwrite(&m_AccTime, sizeof(float), 1, _pFile);
	fwrite(&m_Repeat, sizeof(bool), 1, _pFile);
}

void CAnimator2D::LoadFromFile(FILE* _pFile)
{
	size_t AnimationCount;
	fread(&AnimationCount, sizeof(size_t), 1, _pFile);

	for (size_t i = 0; i < AnimationCount; ++i)
	{
		Ptr<CAnimation> pAnimation;
		LoadAssetRef(pAnimation, _pFile);
		m_vecAnimation.push_back(pAnimation);
	}

	// Animation 정보 Load
	LoadAssetRef(m_CurAnimation, _pFile);

	// Frame Sprite 정보 Load
	LoadAssetRef(m_CurFrmSprite, _pFile);

	fread(&m_CurFrmIdx, sizeof(int), 1, _pFile);
	fread(&m_FPS, sizeof(float), 1, _pFile);
	fread(&m_AccTime, sizeof(float), 1, _pFile);
	fread(&m_Repeat, sizeof(bool), 1, _pFile);
}