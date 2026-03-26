#include "pch.h"
#include "MeshUI.h"
#include "TreeUI.h"

MeshUI::MeshUI()
	: AssetUI("Mesh", ASSET_TYPE::MESH)
{
}

MeshUI::~MeshUI()
{
}

void MeshUI::Render_Update()
{
	AssetUI::Render_Update();

	ImGui::Text("Mesh");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(150);
	//ImGui::InputText("##MeshName", (char*));

}
