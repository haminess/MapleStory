#include "pch.h"
#include "Collider2DUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CCollider2D.h>

Collider2DUI::Collider2DUI()
	: ComponentUI("Collider2DUI", COMPONENT_TYPE::COLLIDER2D)
{
}

Collider2DUI::~Collider2DUI()
{
}

void Collider2DUI::Render_Update()
{
	CGameObject* pTarget = GetTargetObject();
	if (nullptr == pTarget)
		return;

	CCollider2D* pCollider2D = pTarget->Collider2D();
	if (nullptr == pCollider2D)
		return;

	ComponentTitle("Collider2D");

	Vector2 vOffset = pCollider2D->GetOffset();
	Vector2 vScale = pCollider2D->GetScale();
	bool bIndependent = pCollider2D->IsIndependentScale();

	ImGui::Text("Scale");
	ImGui::SameLine(100);
	if (ImGui::DragFloat2("##ColliderScale", vScale))
	{
		pTarget->Collider2D()->SetScale(vScale);
	}

	ImGui::Text("Offset");
	ImGui::SameLine(100);
	if (ImGui::DragFloat2("##ColliderOffset", vOffset))
	{
		pTarget->Collider2D()->SetOffset(vOffset);
	}

	ImGui::Text("Independent");
	ImGui::SameLine(100);
	if (ImGui::Checkbox("##Independent", &bIndependent))
	{
		pTarget->Collider2D()->SetIndependentScale(bIndependent);
	}
}