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
	// FlipBookComponent �� ������ ���� ������� Sprite ������ Binding �ϰ� �Ѵ�.
	if (Animator2D())
		Animator2D()->Binding();
	else
		CAnimator2D::Clear();

	// ��ġ, ũ��, ȸ�� �������� ���ε�
	Transform()->Binding();

	// Animator3D Binding
	if (Animator3D())
	{
		Animator3D()->Binding();

		for (UINT i = 0; i < GetMesh()->GetSubsetCount(); ++i)
		{
			if (nullptr == GetMaterial(i))
				continue;

			GetMaterial(i)->SetAnim3D(true); // Animation Mesh �˸���
			GetMaterial(i)->SetBoneCount(Animator3D()->GetBoneCount());
		}
	}

	for (UINT i = 0; i < GetMesh()->GetSubsetCount(); ++i)
	{
		// ���� ���ε�(���� ���, ���̴� ���)
		if (!GetMaterial(i))
			continue;

		GetMaterial(i)->Binding();

		// ���ؽ�����, �ε������� ���ε� �� ������ ȣ��
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
