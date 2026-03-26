#ifndef _STRUCT
#define _STRUCT

struct tLight2DInfo
{
    float3  vColor;     // 광원 색상	
    float3  vWorldPos;  // 광원 위치
    float3  vDir;       // 광원 진행 방향
    int     Type;       // 광원 타입(LIGHT_TYPE)
    float   Radius;     // 광원 범위반경
    float   Angle;      // 광원 범위각도	  
};


struct tMTileData
{
    int TexIdx;
    float3 Padding;
    float2 TilePos;
    float2 TileScale;
};


// engine cpp 구조체와 맞춰줘야 함
struct tParticle
{
    float4  vColor;
    float3  vLocalPos;
    float3  vWorldPos;
    float3  vWorldInitScale;
    float3  vWorldCurrentScale;
    float3  vWorldRotation;
    
    float3  vForce;
    float3  vVelocity;
        
    float   NoiseForceAccTime;  // Noise Force 텀에 도달하는 누적 시간
    float3  NoiseForceDir;      // 적용된 Noise Forec 의 방향
            
    float   Mass;
    float   Life;
    float   Age;
    float   NormalizedAge;
    int     Active;
    
    float2  padding;
};

struct tParticleModule
{
    // [Create Module]
    // Spawn
    uint    SpawnRate;          // 초당 파티클 생성 개수
    float4  vSpawnColor;        // 생성 시점 색상
    float4  vSpawnMinScale;     // 생성 시 최소 크기
    float4  vSpawnMaxScale;     // 생성 시 최대 크기

    float   MinLife;            // 최소 수명
    float   MaxLife;            // 최대 수명
    
    //  구 내에서 파티클 생성 생성
    uint    SpawnShape;             // 0 : Box,  1: Sphere
    float3  SpawnShapeScale;        // SpawnShapeScale.x == Radius
    
    uint    BlockSpawnShape;        // 0 : Box,  1: Sphere
    float3  BlockSpawnShapeScale;   // SpawnShapeScale.x == Radius    
    
    //  입자가 오브젝트 위치에 영향을 받느냐, 월드에 독자적으로 존재하냐
    uint    SpaceType; // 0 : LocalSpcae, 1 : WorldSpace
    
        
        
    //  [Create Module]
    //  Spawn Burst
    //  순간적으로 터지는 이펙트
    uint    SpawnBurstCount; // 한번에 발생시키는 Particle 수
    uint    SpawnBurstRepeat;
    float   SpawnBurstRepeatTime;
    
    //  Add Velocity
    //  뻗어나가는 이펙트
    uint    AddVelocityType;    // 0 : Random, 1 : FromCenter, 2 : ToCenter, 4 : Fixed 방향
    float3  AddVelocityFixedDir;
    float   AddMinSpeed;
    float   AddMaxSpeed;
    
    //  Scale Module
    //  스폰 시작 시점 크기, 종료 시점 크기
    float   StartScale;
    float   EndScale;
    
    //  Drag Module
    //  감속 모듈, age까지 speed가 되도록 감속
    float   DestNormalizedAge;
    float   LimitSpeed;
    
    //  Noise Force Module
    //  노이즈 설정
    float   NoiseForceTerm;     // Noise Force 적용시키는 텀
    float   NoiseForceScale;    // Noise Force 의 크기
       
    //  Render Module
    float3  EndColor;   // 최종 색상
    uint    FadeOut;    // 0 : Off, 1 : Normalized Age
    float   StartRatio; // FadeOut 효과가 시작되는 Normalized Age 지점
    //  빌보드 무시하고 진행 방향으로 회전
    //  이 경우 메시는 십자로 구성해서 입체적으로 보일 수 있도록 함
    uint    VelocityAlignment; // 속도 정렬 0 : Off, 1 : On
    
	//  Module On / Off
    //  모듈을 켜야함
    int     Module[7];
};

#endif