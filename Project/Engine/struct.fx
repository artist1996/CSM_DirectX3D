#ifndef _STRUCT
#define _STRUCT

struct tLight
{
    float4 Color;   // ���� ����
    float4 Ambient; // ȯ�汤
    float  SpecCoef;
};

struct tLightInfo
{
    tLight light;         // ���� ���� ����
    float3 WorldPos;     // ���� ��ġ
    float3 WorldDir;     // ������ �����ϴ� ����
    float  Radius;       // ������ �ݰ�
    float  Angle;        // ���� ���� ����
    uint   Type;         // ���� ����
    int2   Padding;      // �е�
};

struct tParticle
{
    float4 vColor;
    float3 vLocalPos;
    float3 vWorldPos;
    float3 vWorldInitScale;
    float3 vWorldCurrentScale;
    float3 vWorldRotation;
    
    float3 vForce;
    float3 vVelocity;
    
    float  NoiseForceAccTime;    // Noise Force �ҿ� �����ϴ� ���� �ð�
    float3 NoiseForceDir;        // ����� Noise Force �� ����
    
    float  Mass;
    float  Life;
    float  Age;
    float  NormalizedAge;
    
    int    Active;
    
    float2 Padding;
};

struct tSpawnCount
{
    uint  iSpawnCount;
    uint3 Padding;
};

struct tParticleModule
{
    // Spawn
    uint   SpawnRate;            // �ʴ� ��ƼŬ ���� ����
    float4 vSpawnColor;          // ���� ���� ����
    float4 vSpawnMinScale;       // ���� �� �ּ� ũ��
    float4 vSpawnMaxScale;       // ���� �� �ִ� ũ��

    float  MinLife;              // �ּ� ����
    float  MaxLife;              // �ִ� ����

    uint   SpawnShape;           // 0 : Box,  1: Sphere
    float3 SpawnShapeScale;      // SpawnShapeScale.x == Radius

    uint   BlockSpawnShape;      // 0 : Box,  1: Sphere
    float3 BlockSpawnShapeScale; // SpawnShapeScale.x == Radius    
    
    uint   SpaceType;            // 0 : LocalSpcae, 1 : WorldSpace
    
    // Spawn Burst
    uint   SpawnBurstCount;      // �ѹ��� �߻���Ű�� Particle ��
    uint   SpawnBurstRepeat;
    float  SpawnBurstRepeatTime;
    
    // Add Velocity
    uint   AddVelocityType;      // 0 : Random, 1 : FromCenter, 2 : ToCenter, 3 : Fixed 
    float3 AddVelocityFixedDir;
    float  AddMinSpeed;
    float  AddMaxSpeed;
    
    // Scale Module
    float  StartScale;
    float  EndScale;
    
    // Drag Module
    float  DestNormalizedAge;
    float  LimitSpeed;
    
    // Noise Force Module
    float  NoiseForceTerm;      // Noise Force �����Ű�� ��
    float  NoiseForceScale;     // Noise Force �� ũ��
       
    // Render Module
    float3 EndColor;            // ���� ����
    uint   FadeOut;             // 0 : Off, 1 : Normalized Age
    float  StartRatio;          // FadeOut ȿ���� ���۵Ǵ� Normalized Age ����
    uint   VelocityAlignment;   // �ӵ� ���� 0 : Off, 1 : On
    
	// Module On / Off
    int    Module[7];
};

#endif