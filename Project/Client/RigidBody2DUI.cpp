#include "pch.h"
#include "RigidBody2DUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CRigidBody2D.h>

RigidBody2DUI::RigidBody2DUI()
    : ComponentUI("RigidBody2DUI", COMPONENT_TYPE::RIGIDBODY2D)
{
}

RigidBody2DUI::~RigidBody2DUI()
{
}

void RigidBody2DUI::Render_Update()
{
    ComponentTitle("RigidBody2D");

    CRigidBody2D* pRigidBody = GetTargetObject()->RigidBody2D();

    // Mode ComboBox
    const char* modes[] = { "TopView", "PlatFormer" };
    int currentMode = (int)pRigidBody->GetMode();
    ImGui::Text("Mode");
    ImGui::SameLine(100);
    ImGui::SetNextItemWidth(150);
    if (ImGui::Combo("##Mode", &currentMode, modes, IM_ARRAYSIZE(modes)))
    {
        pRigidBody->SetMode((RIGIDBODY_MODE)currentMode);
    }

    // Mass
    float mass = pRigidBody->GetMass();
    ImGui::Text("Mass");
    ImGui::SameLine(100);
    ImGui::SetNextItemWidth(150);
    if (ImGui::DragFloat("##Mass", &mass, 0.1f, 0.f, 1000.f))
    {
        pRigidBody->SetMass(mass);
    }

    // Friction
    float friction = pRigidBody->GetFriction();
    ImGui::Text("Friction");
    ImGui::SameLine(100);
    ImGui::SetNextItemWidth(150);
    if (ImGui::DragFloat("##Friction", &friction, 1.0f, 0.f, 10000.f))
    {
        pRigidBody->SetFriction(friction);
    }

    // MaxSpeed
    float maxSpeed = pRigidBody->GetMaxSpeed();
    ImGui::Text("Max Speed");
    ImGui::SameLine(100);
    ImGui::SetNextItemWidth(150);
    if (ImGui::DragFloat("##MaxSpeed", &maxSpeed, 1.0f, 0.f, 10000.f))
    {
        pRigidBody->SetMaxSpeed(maxSpeed);
    }

    // Use Gravity
    bool useGravity = pRigidBody->GetUseGravity();
    ImGui::Text("Use Gravity");
    ImGui::SameLine(100);
    if (ImGui::Checkbox("##UseGravity", &useGravity))
    {
        pRigidBody->UseGravity(useGravity);
    }

    // Gravity Max Speed
    float gravityMaxSpeed = pRigidBody->GetGravityMaxSpeed();
    ImGui::Text("Gravity Max Speed");
    ImGui::SameLine(100);
    ImGui::SetNextItemWidth(150);
    if (ImGui::DragFloat("##GravityMaxSpeed", &gravityMaxSpeed, 1.0f, 0.f, 10000.f))
    {
        pRigidBody->SetGravityMaxSpeed(gravityMaxSpeed);
    }

    // Gravity Accel
    float gravityAccel = pRigidBody->GetGravityAccel();
    ImGui::Text("Gravity Accel");
    ImGui::SameLine(100);
    ImGui::SetNextItemWidth(150);
    if (ImGui::DragFloat("##GravityAccel", &gravityAccel, 1.0f, 0.f, 10000.f))
    {
        pRigidBody->SetGravityAccel(gravityAccel);
    }

    // Jump Speed
    float jumpSpeed = pRigidBody->GetJumpSpeed();
    ImGui::Text("Jump Speed");
    ImGui::SameLine(100);
    ImGui::SetNextItemWidth(150);
    if (ImGui::DragFloat("##JumpSpeed", &jumpSpeed, 1.0f, 0.f, 10000.f))
    {
        pRigidBody->SetJumpSpeed(jumpSpeed);
    }

    // Current Status (읽기 전용)
    ImGui::Separator();
    ImGui::Text("Status");

    Vector3 velocity = pRigidBody->GetVelocity();
    ImGui::Text("Velocity");
    ImGui::SameLine(100);
    ImGui::SetNextItemWidth(150);
    ImGui::InputFloat3("##Velocity", velocity, "%.1f", ImGuiInputTextFlags_ReadOnly);

    Vector3 gravityVel = pRigidBody->GetGravityVelocity();
    ImGui::Text("Gravity Velocity");
    ImGui::SameLine(100);
    ImGui::SetNextItemWidth(150);
    ImGui::InputFloat3("##GravityVel", gravityVel, "%.1f", ImGuiInputTextFlags_ReadOnly);

    bool isGround = pRigidBody->IsGround();
    ImGui::Text("Is Ground");
    ImGui::SameLine(100);
    ImGui::Checkbox("##IsGround", &isGround);

    bool isMove = pRigidBody->IsMove();
    ImGui::Text("Is Moving");
    ImGui::SameLine(100);
    ImGui::Checkbox("##IsMove", &isMove);
}