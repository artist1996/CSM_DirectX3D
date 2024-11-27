#include "pch.h"
#include "CMeshRender.h"
#include "CAssetMgr.h"

#include "CTransform.h"
#include "CAnimator3D.h"

#include "CAnimator2D.h"
#include "CSprite.h"


CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)
{
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::FinalTick()
{
}

void CMeshRender::Render()
{
	// FlipBookComponent 가 있으면 현재 재생중인 Sprite 정보를 Binding 하게 한다.
	if (Animator2D())
		Animator2D()->Binding();
	else
		CAnimator2D::Clear();

	// 위치, 크기, 회전 상태정보 바인딩
	Transform()->Binding();

	// Animator3D Binding
	if (Animator3D())
	{
		Animator3D()->Binding();

		for (UINT i = 0; i < GetMesh()->GetSubsetCount(); ++i)
		{
			if (nullptr == GetMaterial(i))
				continue;

			GetMaterial(i)->SetAnim3D(true); // Animation Mesh 알리기
			GetMaterial(i)->SetBoneCount(Animator3D()->GetBoneCount());
		}
	}

	for (UINT i = 0; i < GetMesh()->GetSubsetCount(); ++i)
	{
		// 재질 바인딩(재질 상수, 쉐이더 등등)
		if (!GetMaterial(i))
			continue;

		GetMaterial(i)->Binding();

		// 버텍스버퍼, 인덱스버퍼 바인딩 및 렌더링 호출
		GetMesh()->Render(i);
	}
}

void CMeshRender::SaveToFile(FILE* _pFile)
{
	SaveDataToFile(_pFile);
}

void CMeshRender::LoadFromFile(FILE* _pFile)
{
	LoadDataFromFile(_pFile);
}
