#pragma once

struct Vtx
{
	Vec3	vPos;
	Vec4	vColor;
	Vec2	vUV;

	Vec3	vTangent;
	Vec3    vNormal;
	Vec3    vBinormal;
};

struct tRay
{
	Vec3 vStart;	// Ray 의 시작 지점
	Vec3 vDir;		// Ray 의 방향
};

// ====================
// 상수 버퍼 연동 구조체
// ====================
struct tTransform
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;

	Matrix matWorldInv;
	Matrix matViewInv;
	Matrix matProjInv;

	Matrix matWV;
	Matrix matWVP;
};

extern tTransform g_Trans;

struct tMtrlConst
{
	int		iArr[4];
	float	fArr[4];
	Vec2	v2Arr[4];
	Vec4	v4Arr[4];
	Matrix  matArr[4];
	int		btex[TEX_PARAM::END + 2];
};

struct tSpriteInfo
{
	Vec2 vLeftTopUV;
	Vec2 vSliceUV;
	Vec2 vBackGroundUV;
	Vec2 vOffsetUV;
	int  UseFlipBook;
	int  Padding[3];
};

struct tPixel
{
	BYTE b;
	BYTE g;
	BYTE r;
	BYTE a;
};

struct tDebugShapeInfo
{
	DEBUG_SHAPE Shape;
	Vec3		vPos;
	Vec3		vScale;
	Vec3		vRot;
	Matrix		matWorld;
	Vec4		vColor;

	float		LifeTime;
	float		Age;

	bool		DepthTest;
};

struct tTask
{
	TASK_TYPE Type;
	
	DWORD_PTR Param_0;
	DWORD_PTR Param_1;
};

struct tLight
{
	Vec4  Color;	// 빛의 색상
	Vec4  Ambient;	// 환경광
	float SpecCoef;	// 
};

struct tLightInfo
{
	tLight		light;	  // 광원 색상 정보
	Vec3		WorldPos; // 광원 위치
	Vec3		WorldDir; // 광원이 진행하는 방향
	
	float		Radius;	  // 광원의 반경
	float		Angle;    // 광원 범위 각도

	LIGHT_TYPE	Type;	  // 광원 종류

	int			Padding[2];	 // 패딩
};

struct tParticle
{
	Vec4   vColor;
	Vec3   vLocalPos;
	Vec3   vWorldPos;
	Vec3   vWorldInitScale;
	Vec3   vWorldIdCurrentScale;
	Vec3   vWorldRotation;

	Vec3   vForce;				// Particle에 누적된 힘
	Vec3   vVelocity;

	float  NoiseForceAccTime;	// Noise Force 텀에 도달하는 누적 시간
	Vec3   NoiseForceDir;		// Noise Force 의 방향

	float  Mass;
	float  Life;
	float  Age;
	float  NormalizedAge;
	int    Active;

	Vec2   Padding;
};

// Particle Module
struct tParticleModule
{
	// Spawn
	UINT  SpawnRate;			// 초당 파티클 생성개수 (Spawn Per Second)
	Vec4  vSpawnColor;			// 생성 시점 색상
	Vec4  vSpawnMinScale;		// 생성 시 최소 크기
	Vec4  vSpawnMaxScale;		// 생성 시 최대 크기
	
	float MinLife;				// 최소 수명
	float MaxLife;				// 최대 수명

	UINT  SpawnShape;			// 0 : Box, 1 : Sphere
	Vec3  SpawnShapeScale;		// SpawnShapeScale.x == Radius
		  
	UINT  BlockSpawnShape;		// 0 : Box, 1 : Sphere
	Vec3  BlockSpawnShapeScale; // SpawnShapeScale.x == Radius
		  
	UINT  SpaceType;			// 0 : Local Space, 1 : World Space

	// Spawn Burst
	UINT  SpawnBurstCount;		// 한번에 발생시키는 Particle 수
	UINT  SpawnBurstRepeat;		// 반복 여부
	float SpawnBurstRepeatTime; // 반복 생성 시간

	// Add Velocity
	UINT  AddVelocityType;		// 0 : Random, 1 : FromCenter, 2 : ToCenter, 3 : Fixed
	Vec3  AddVelocityFixedDir;
	float AddMinSpeed;			// 최소 속력
	float AddMaxSpeed;			// 최대 속력

	// Scale Module
	float StartScale;
	float EndScale;
	
	// Drag Module
	float DestNormalizedAge;
	float LimitSpeed;

	// Noise Force Module
	float NoiseForceTerm;		// Noise Force 적용시키는 텀
	float NoiseForceScale;		// Noise Force 크기

	// Render Module
	Vec3  EndColor;				// 최종 색상
	UINT  FadeOut;				// 0 : Off, 1 : Normalized Age
	float FadeOutStartRatio;	// FadeOut 효과가 시작되는 Normalized Age 지점
	UINT  VelocityAlignment;	// 속도 정렬 0 : Off, 1 : On

	// Moudle On / Off
	int Module[(UINT)PARTICLE_MODULE::END];
};

struct tGlobalData
{
	float   g_DT;            // DT
	float   g_EngineDT;      // Engine DT
	float   g_Time;          // Time
	float   g_EngineTime;    // Engine Time

	Vec4	g_CamWorldPos;	 // 카메라 월드 위치

	Vec2    g_Resolution;    // RenderTarget 해상도 정보

	int     g_Light2DCount;  // 2D 광원 개수
	int     g_Light3DCount;  // 3D 광원 개수
};

extern tGlobalData g_GlobalData;