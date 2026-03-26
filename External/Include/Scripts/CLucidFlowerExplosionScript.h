#pragma once
#include <Engine/CScript.h>

#define S(Size) LUCID_FE_SIZE::Size
#define Desc tLucidFlowerExplosionDesc
#define LUCID_FE_PATTERN_SIZE 6

enum class LUCID_FE_SIZE
{
	MS,
	M,
	L,
	XL,
};

struct tLucidFlowerExplosionDesc
{
	Vector2 pos;
	int size;
	float angle;

	tLucidFlowerExplosionDesc(Vector2 _pos, int _size, float _angle)
		: pos(_pos)
		, size(_size)
		, angle(_angle)
	{
	}
	tLucidFlowerExplosionDesc(Vector2 _pos, LUCID_FE_SIZE _size, float _angle)
		: pos(_pos)
		, size((UINT)_size)
		, angle(_angle)
	{
	}
};


class CLucidFlowerExplosionScript :
	public CScript
{
private:
	const vector<tLucidFlowerExplosionDesc> m_Desc[LUCID_FE_PATTERN_SIZE] = {
		{ { Vector2(-700, -200), 3, 45 }, { Vector2(-550, -200), 2, 100 }, { Vector2(-550, -150), 3, 10 }, { Vector2(-200, -100), 2, 45 }, { Vector2(100, -150), 1, 65 }, { Vector2(300, -50), 0, 110 }, { Vector2(500, -200), 3, 90 }, { Vector2(700, -200), 3, 60 } },
		{ Desc({-600, -50}, S(XL), 120), Desc({-400, -50}, S(MS), 70), Desc({100, -50}, S(XL), 90 ), Desc({0, -50}, S(L), 90), Desc({200, -50}, S(MS), 100), Desc({300, -50}, S(MS), 120), Desc({450, -200}, S(XL), 60), Desc({450, -100}, S(L), 120) },
		{ Desc({-600, -200}, S(XL), 120), Desc({-400, -100}, S(XL), 80 ), Desc({-200, -200}, S(L), 90), Desc({0, -50}, S(M), 120), Desc({200, -200}, S(XL), 90), Desc({500, -200}, S(L), 120), Desc({600, -100}, S(M), 70), Desc({800, -200}, S(M), 60) },
		{ Desc({-550, -200}, S(XL), 60), Desc({-200, -200}, S(L), 120), Desc({100, -100}, S(M), 120), Desc({300, -100}, S(MS), 100), Desc({500, -50}, S(XL), 90), Desc({0, -250}, S(XL), 70) },
		{ Desc({-550, -200}, S(XL), 70), Desc({-250, -200}, S(L), 50), Desc({0, -200}, S(L), 95), Desc({200, -100}, S(MS), 110), Desc({500, -250}, S(M), 170), Desc({500, -50}, S(XL), 110) },
		{ Desc({-500, -200}, S(M), 60), Desc({-200, -200}, S(L), 90), Desc({250, -90}, S(MS), 100), Desc({200, -250}, S(L), 135), Desc({500, -50}, S(XL), 70), Desc({0, -50}, S(M), 160) }
	};

	vector<CGameObject*> m_Projectile;
	Ptr<CFlipbook> m_FlowerAnim[4];
	Ptr<CFlipbook> m_HitAnim;
	bool		   m_Hitted;

	UINT m_SpawnTimerHandle;

private:
	void Spawn();
	void ClearProjectiles();

	void ActivateBomb();
	void ProjectileBeginOverlap(DWORD_PTR _Collider, DWORD_PTR _OtherCollider);

public:
	virtual void Init() override;
	virtual void Begin() override;
	virtual void Tick() override;

	virtual void Activate() override;
	virtual void Deactivate() override;

	virtual void SaveComponent(FILE* _File) override;
	virtual void LoadComponent(FILE* _FILE) override;

public:
	CLONE(CLucidFlowerExplosionScript);
	CLucidFlowerExplosionScript();
	CLucidFlowerExplosionScript(const CLucidFlowerExplosionScript& _Other);
	~CLucidFlowerExplosionScript();
};

#undef S
#undef Desc