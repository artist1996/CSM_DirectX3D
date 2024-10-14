#include "pch.h"
#include "CMeshRender.h"
#include "CAssetMgr.h"
#include "CTransform.h"

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
	if (!GetMesh() || !GetMaterial() || !GetMaterial()->GetShader() || !GetOwner()->IsActive())
		return;

	if (Animator2D())
		Animator2D()->Binding();
	else
		CAnimator2D::Clear();
	

	Transform()->Binding();
	GetMaterial()->Binding();
	GetMesh()->Render();
}

void CMeshRender::SaveToFile(FILE* _pFile)
{
	SaveDataToFile(_pFile);
}

void CMeshRender::LoadFromFile(FILE* _pFile)
{
	LoadDataFromFile(_pFile);
}
