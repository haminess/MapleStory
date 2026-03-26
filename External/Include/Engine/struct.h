#pragma once

// __RENDERING_CB__

// 정점 구조체
struct Vertex
{
	// 3차원 공간에 있는 좌표
	Vector3 vPos;

	// 텍스쳐의 렌더링할 좌표
	Vector2 vUV;

	// 색깔 값 (0 ~ 1)
	Vector4 vColor; 
};



struct TransInfo
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;

	Matrix matWV;	// World View
	Matrix matWVP;
};

extern TransInfo g_Trans;

struct MtrlConst
{
	int		iArr[4];
	float	fArr[4];
	Vector2 v2Arr[4];
	Vector4	v4Arr[4];
	Matrix	mat[2];

	// 텍스쳐 전달 했는지 정보
	int bTex[TEX_END];
};



struct SpriteInfo
{
	Vector2 LeftTopUV;
	Vector2 SliceUV;
	Vector2 BackgroundUV;
	Vector2 OffsetUV;
	int		SpriteUse;
	float	SpriteAlpha;
	int		padding[2];
};



struct GlobalData
{
	Vector2 RenderResolution;	// 렌더링 해상도
	Vector2 MousePos;
	Vector2 MouseWorldPos;

	float DeltaTime;			// DT
	float Time;					// 누적시간

	float DT_Engine;			// DT
	float Time_Engine;					// 누적시간

	int Light2DCount;			// 2D 광원 개수
	int Light3DCount;			// 3D 광원 개수
};

extern GlobalData g_Data;


// __RENDERING_SB__

struct tLight2DInfo
{
	Vector3		vColor;		// 광원 색상	
	Vector3		vWorldPos;		// 광원 진행 방향
	Vector3		vDir;		// 광원 위치
	int			Type;		// 광원 타입(LIGHT_TYPE)
	float		Radius;		// 광원 범위반경
	float		Angle;		// 광원 범위각도	
};



// Particle
struct tParticle
{
	Vector4	vColor;
	Vector3	vLocalPos;
	Vector3	vWorldPos;
	Vector3	vWorldInitScale;
	Vector3	vWorldCurrentScale;
	Vector3	vWorldRotation;

	Vector3	vForce;				// Particle 에 누적된 힘
	Vector3	vVelocity;

	float	NoiseForceAccTime;	// Noise Force 텀에 도달하는 누적 시간
	Vector3	NoiseForceDir;		// Noise Force 의 방향

	float	Mass;
	float	Life;				// 최대 수명
	float	Age;				// 현재 수명
	float	NormalizedAge;	// 정규화된 수명 -> 불규칙적인 파티클 표현 가능해짐

	int		Active;				// 파티클 활성화
	Vector2	Padding;
};



// Particle Module
struct tParticleModule
{
	// Spawn
	UINT	SpawnRate;				// 초당 파티클 생성개수 (Spawn Per Second)
	Vector4	vSpawnColor;			// 생성 시점 색상
	Vector4	vSpawnMinScale;			// 생성 시 최소 크기
	Vector4	vSpawnMaxScale;			// 생성 시 최대 크기

	float	MinLife;				// 최소 수명
	float	MaxLife;				// 최대 수명

	UINT	SpawnShape;				// 0 : Box,  1: Sphere
	Vector3	SpawnShapeScale;		// SpawnShapeScale.x == Radius

	UINT	BlockSpawnShape;		// 0 : Box,  1: Sphere
	Vector3	BlockSpawnShapeScale;	// SpawnShapeScale.x == Radius

	UINT	SpaceType;				// 0 : LocalSpace, 1 : WorldSpace

	// Spawn Burst
	UINT	SpawnBurstCount;		// 한번에 발생시키는 Particle 수
	UINT	SpawnBurstRepeat;
	float	SpawnBurstRepeatTime;

	// Add Velocity
	UINT	AddVelocityType;		// 0 : Random, 1 : FromCenter, 2 : ToCenter, 4 : Fixed 
	Vector3	AddVelocityFixedDir;
	float	AddMinSpeed;
	float	AddMaxSpeed;

	// Scale Module
	float	StartScale;
	float	EndScale;

	// Drag Module
	float	DestNormalizedAge;
	float	LimitSpeed;

	// Noise Force Module
	float	NoiseForceTerm;		// Noise Force 적용시키는 텀
	float	NoiseForceScale;	// Noise Force 크기

	// Render Module
	Vector3	EndColor;			// 최종 색상
	UINT	FadeOut;			// 0 : Off, 1 : Normalized Age
	float	StartRatio;			// FadeOut 효과가 시작되는 Normalized Age 지점
	UINT	VelocityAlignment;	// 속도 정렬 0 : Off, 1 : On


	// Module On / Off
	int		Module[(UINT)PARTICLE_MODULE::END];
};


// __ENGINE__

struct tDebugShapeInfo
{
	DEBUG_SHAPE Shape;
	Vector3		WorldPos;
	Vector3		Scale;
	Vector4		Rotation;
	Matrix		matWorld;

	Vector4		Color;
	float		Duration;
	float		Time;

	bool		DepthTest;
};

struct tTextInfo
{
	wstring		Text;
	Vector2		Position;
	float		FontSize;
	UINT		Color;
	float		Time;
};


struct tTask
{
	TASK_TYPE Type;

	// DWORD_PTR
	// 운영체제에 따라 크기가 달라지는 가변 자료형
	// 64바이트 운영체제에서는 8byte
	// 32바이트 운영체제에서는 4byte
	DWORD_PTR	Param0;
	DWORD_PTR	Param1;
	DWORD_PTR	Param2;

	CGameObject*	Owner;			// owner가 비활성화되거나 삭제되면 실행되지 않음
	float			Timer;
	float			ElapsedTime;
	bool			IsLoop;			// (삭제 기능)구현 중에 있으므로 사용에 유의
};



struct tScriptTimerInfo
{
	CScript* Inst;
	SCRIPT_DELEGATE Func;
	SCRIPT_DELEGATE_1 Func_1;
	SCRIPT_DELEGATE_2 Func_2;
	DWORD_PTR	Param0;
	DWORD_PTR	Param1;

	UINT	Id;
	float	Timer;
	float	ElapsedTime;
	bool	IsLoop;
};



// Pixel
struct tPixel
{
	BYTE b;
	BYTE g;
	BYTE r;
	BYTE a;
};



// EDITOR
struct tProperty
{
	string  Type;
	string  Name;
	void* pValue;
};
