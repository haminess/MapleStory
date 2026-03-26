#include "pch.h"
#include "CGameObjectEx.h"

#include <Engine/CComponent.h>

// 카메라에 등록 되지 않는 오브젝트
void CGameObjectEx::FinalTick_Editor()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (!GetComponent((COMPONENT_TYPE)i))
			continue;

		GetComponent((COMPONENT_TYPE)i)->FinalTick();
	}

	const vector<CGameObject*>& vecChild = GetChild();
	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		CGameObjectEx* pChild = dynamic_cast<CGameObjectEx*>(vecChild[i]);
		assert(pChild);
		pChild->FinalTick_Editor();
	}
}