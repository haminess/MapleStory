#pragma once

class CProjectileScript;
CProjectileScript* CreateProjectile();

void PlayerDamage(int _Damage, Vector2 _Dir = {});
void PlayerDamageByPercent(float _Per, Vector2 _Dir = {});

class CCollider2D;
Vector2 GetKnockbackDir(CCollider2D* _Hitted, CCollider2D* _Other, float _Power = 500.f);