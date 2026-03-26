#include "pch.h"
#include "CHeroAuraBladeSkill.h"

CHeroAuraBladeSkill::CHeroAuraBladeSkill(UINT _Type)
	: CSkillScript(_Type)
{
}

CHeroAuraBladeSkill::CHeroAuraBladeSkill(const CHeroAuraBladeSkill& _Other)
	: CSkillScript(_Other)
{
}

CHeroAuraBladeSkill::~CHeroAuraBladeSkill()
{
}


void CHeroAuraBladeSkill::SkillKeyTap()
{
	Vector2 vDir;
	if (KEY_PRESSED(KEY::LEFT))		vDir.x -= 1000.f;
	if (KEY_PRESSED(KEY::RIGHT))	vDir.x += 1000.f;
	if (KEY_PRESSED(KEY::UP))		vDir.y += 1500.f;
	if (KEY_PRESSED(KEY::DOWN))		vDir.y -= 1500.f;

	GetUser()->RigidBody2D()->AddImpulse(vDir);
}

void CHeroAuraBladeSkill::Tick()
{
}
