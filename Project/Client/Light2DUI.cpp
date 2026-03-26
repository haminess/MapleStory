#include "pch.h"
#include "Light2DUI.h"

#include <Engine/CLight2D.h>


Light2DUI::Light2DUI()
	: ComponentUI("Light2DUI", COMPONENT_TYPE::LIGHT2D)
{
}

Light2DUI::~Light2DUI()
{

}

void Light2DUI::Render_Update()
{
	CLight2D* pLight2D = GetTargetObject()->Light2D();

	ComponentTitle("Light2D");

	tLight2DInfo info = pLight2D->GetLight2DInfo();

	// Type
	ImGui::Text("Type");
	ImGui::SameLine(100);
	string lightType[(int)LIGHT_TYPE::END] = {"DIRECTIONAL", "POINT", "SPOT"};
	if (ImGui::BeginCombo("##LightType", lightType[info.Type].c_str(), 0))
	{
		for (int i = 0; i < (int)LIGHT_TYPE::END; i++)
		{
			if (ImGui::Selectable(lightType[i].c_str()))
			{
				pLight2D->SetLightType((LIGHT_TYPE)i);
			}
		}
		ImGui::EndCombo();
	}

	// Color
	ImGui::Text("Color");
	ImGui::SameLine(100);
	ImGui::ColorEdit3("##Color", (float*)&info.vColor, 0);
	pLight2D->SetLightColor(info.vColor);


	// Radius
	if ((int)LIGHT_TYPE::DIRECTIONAL != info.Type)
	{
		ImGui::Text("Radius");
		ImGui::SameLine(100);
		if (ImGui::DragFloat("##Radius", &info.Radius))
		{
			pLight2D->SetRadius(info.Radius);
		}
	}

	// Angle
	if ((int)LIGHT_TYPE::SPOT == info.Type)
	{
		ImGui::Text("Angle");
		ImGui::SameLine(100);
		if (ImGui::DragFloat("##Angle", &info.Angle))
		{
			pLight2D->SetAngle(info.Angle);
		}
	}

	ImGui::SameLine(100);

}