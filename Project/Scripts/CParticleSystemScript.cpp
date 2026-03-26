#include "pch.h"
#include "CParticleSystemScript.h"
#include <Engine/CParticleSystem.h>
#include <Engine/CStructuredBuffer.h>

CParticleSystemScript::CParticleSystemScript()
	: CScript((UINT)SCRIPT_TYPE::PARTICLESYSTEMSCRIPT)
	, m_PS(nullptr)
{
}

CParticleSystemScript::~CParticleSystemScript()
{
}

void CParticleSystemScript::Init()
{
	m_PS = (CParticleSystem*)GetOwner()->GetComponent(COMPONENT_TYPE::PARTICLE_SYSTEM);
	if (!m_PS)
		GetOwner()->AddComponent(new CParticleSystem);

	m_PS = ParticleSystem();

	// Á÷·ÄÈ­
	SERIALIZABLE(ParticleSystem);
	PROPERTY_SETNAME(Ptr<CTexture>, "Texture", m_PS->m_ParticleTex);
	PROPERTY_SETNAME(float, "Time", m_PS->m_Time);
	PROPERTY_SETNAME(float, "BurstTime", m_PS->m_BurstTime);
	PROPERTY_SETNAME(int, "MaxCount", m_PS->m_MaxParticleCount);

	HEADER("\n- MODULE");
	HEADER("Spawn");
	PROPERTY_SETNAME(UINT, "SpawnRate", m_PS->m_Module.SpawnRate);
	PROPERTY_SETNAME(Color4, "SpawnColor", m_PS->m_Module.vSpawnColor);
	PROPERTY_SETNAME(Vector4, "MinScale", m_PS->m_Module.vSpawnMinScale);
	PROPERTY_SETNAME(Vector4, "MaxScale", m_PS->m_Module.vSpawnMaxScale);

	HEADER("\nLife");
	PROPERTY_SETNAME(float, "MinLife", m_PS->m_Module.MinLife);
	PROPERTY_SETNAME(float, "MaxLife", m_PS->m_Module.MaxLife);

	// Spawn
	HEADER(" ");
	PROPERTY_SETNAME(bool, "Spawn", m_PS->m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN]);
	PROPERTY_SETNAME(UINT, "SpawnShape", m_PS->m_Module.SpawnShape);
	PROPERTY_SETNAME(Vector3, "SpawnShapeScale", m_PS->m_Module.SpawnShapeScale);
	PROPERTY_SETNAME(UINT, "BlockShape", m_PS->m_Module.BlockSpawnShape);
	PROPERTY_SETNAME(Vector3, "BlockShapeScale", m_PS->m_Module.BlockSpawnShapeScale);
	PROPERTY_SETNAME(UINT, "SpaceType", m_PS->m_Module.SpaceType);

	// Spawn Burst
	HEADER(" ");
	PROPERTY_SETNAME(bool, "Spawn Burst", m_PS->m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST]);
	PROPERTY_SETNAME(UINT, "Count", m_PS->m_Module.SpawnBurstCount);
	PROPERTY_SETNAME(UINT, "Repeat", m_PS->m_Module.SpawnBurstRepeat);
	PROPERTY_SETNAME(float, "RepeatTime", m_PS->m_Module.SpawnBurstRepeatTime);

	// Velocity
	HEADER(" ");
	PROPERTY_SETNAME(bool, "Velocity", m_PS->m_Module.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY]);
	PROPERTY_SETNAME(UINT, "VelocityType", m_PS->m_Module.AddVelocityType);
	PROPERTY_SETNAME(Vector3, "FixedDir", m_PS->m_Module.AddVelocityFixedDir);
	PROPERTY_SETNAME(float, "MinSpeed", m_PS->m_Module.AddMinSpeed);
	PROPERTY_SETNAME(float, "MaxSpeed", m_PS->m_Module.AddMaxSpeed);

	// Scale
	HEADER(" ");
	PROPERTY_SETNAME(bool, "Scale", m_PS->m_Module.Module[(UINT)PARTICLE_MODULE::SCALE]);
	PROPERTY_SETNAME(float, "StartScale", m_PS->m_Module.StartScale);
	PROPERTY_SETNAME(float, "EndScale", m_PS->m_Module.EndScale);

	// Drag
	HEADER(" ");
	PROPERTY_SETNAME(bool, "Drag", m_PS->m_Module.Module[(UINT)PARTICLE_MODULE::DRAG]);
	PROPERTY_SETNAME(float, "DestNormalizedAge", m_PS->m_Module.DestNormalizedAge);
	PROPERTY_SETNAME(float, "LimitSpeed", m_PS->m_Module.LimitSpeed);

	// Noise Force
	HEADER(" ");
	PROPERTY_SETNAME(bool, "Noise Force", m_PS->m_Module.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE]);
	PROPERTY_SETNAME(float, "Term", m_PS->m_Module.NoiseForceTerm);
	PROPERTY_SETNAME(float, "Scale", m_PS->m_Module.NoiseForceScale);

	// Render
	HEADER(" ");
	PROPERTY_SETNAME(bool, "Render", m_PS->m_Module.Module[(UINT)PARTICLE_MODULE::RENDER]);
	PROPERTY_SETNAME(Color3, "EndColor", m_PS->m_Module.EndColor);
	PROPERTY_SETNAME(UINT, "FadeOut", m_PS->m_Module.FadeOut);
	PROPERTY_SETNAME(float, "StartRatio", m_PS->m_Module.StartRatio);
	PROPERTY_SETNAME(UINT, "VelocityAlignment", m_PS->m_Module.VelocityAlignment);


	Ptr<CTexture> pParticleTex = CAssetMgr::GetInst()->Load<CTexture>(L"Particle_01", L"Texture\\particle\\TX_GlowScene_2.png");
	ParticleSystem()->SetParticleTexture(pParticleTex);
}

void CParticleSystemScript::Begin()
{
}



void CParticleSystemScript::Tick()
{
	m_PS->m_ModuleBuffer->SetData(&(m_PS->m_Module));
}