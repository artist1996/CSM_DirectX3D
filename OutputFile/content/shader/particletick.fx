#ifndef _PARTICLE_TICK
#define _PARTICLE_TICK

#include "value.fx"
#include "struct.fx"
#include "func.fx"


RWStructuredBuffer<tParticle>     ParticleBuffer   : register(u0);
RWStructuredBuffer<tSpawnCount>   SpawnCountBuffer : register(u1);
Texture2D                         NoiseTex         : register(t20);
StructuredBuffer<tParticleModule> Module           : register(t21);
Texture2D                         FallTex          : register(t22);

#define ParticleObjectPos   g_vec4_0.xyz
#define MAX_COUNT           g_int_0
#define PARTICLE            ParticleBuffer[_ID.x]

// Module Check
#define SpawnModule         Module[0].Module[0]
#define SpawnBurstModule    Module[0].Module[1]
#define SpawnShapeType      Module[0].SpawnShape
#define AddVelocityModule   Module[0].Module[2]
#define ScaleModule         Module[0].Module[3]
#define DragModule          Module[0].Module[4]
#define NoiseForce          Module[0].Module[5]
#define Render              Module[0].Module[6]

[numthreads(1024, 1, 1)]
void CS_ParticleTick(int3 _ID : SV_DispatchThreadID)
{
    if (MAX_COUNT <= _ID.x)
        return;
    
    if (false == PARTICLE.Active)
    {
        int SpawnCount = SpawnCountBuffer[0].iSpawnCount;
        
        while (0 < SpawnCount)
        {
            int Origin = 0;
               
            InterlockedCompareExchange(SpawnCountBuffer[0].iSpawnCount
                                     , SpawnCount
                                     , SpawnCountBuffer[0].iSpawnCount - 1
                                     , Origin);
            
            if(SpawnCount == Origin)
            {
                // 파티클의 위치를 특정 범위내에서 랜덤한 위치로 잡아준다.
                
                // Thread 를 UV 로 맵핑하기 위해서 ID 를 0 ~ 1 범위로 정규화
                float3 vRandom0 = GetRandom(NoiseTex, _ID.x    , MAX_COUNT);
                float3 vRandom1 = GetRandom(NoiseTex, _ID.x + 1, MAX_COUNT);
                float3 vRandom2 = GetRandom(NoiseTex, _ID.x + 2, MAX_COUNT);
                                
                float3 vSpawnPos = (float3) 0.f;
                
                // 0 : Box, 1 : Sphere             
                if(0 == SpawnShapeType)
                {
                    vSpawnPos.x = vRandom0.x * Module[0].SpawnShapeScale.x - (Module[0].SpawnShapeScale.x / 2.f);
                    vSpawnPos.y = vRandom0.y * Module[0].SpawnShapeScale.y - (Module[0].SpawnShapeScale.y / 2.f);
                    vSpawnPos.z = vRandom0.z * Module[0].SpawnShapeScale.z - (Module[0].SpawnShapeScale.z / 2.f);
                }
                
                else if(1 == SpawnShapeType)
                {
                    float fRadius = Module[0].SpawnShapeScale.x;
                    float fBlockRadius = Module[0].BlockSpawnShapeScale.x;
                    float fDifferRadius = fRadius - fBlockRadius;
                        
                    vSpawnPos = normalize(vRandom1 - 0.5f) * fDifferRadius * vRandom2.x 
                                    + normalize(vRandom1 - 0.5f) * fBlockRadius;
                   
                }
                
                // Add Velocity Module
                PARTICLE.vVelocity = (float3) 0.f;
                
                if(AddVelocityModule)
                {
                    float fSpeed = Module[0].AddMinSpeed + (Module[0].AddMaxSpeed - Module[0].AddMinSpeed) * vRandom2.x;
                    
                    // Random
                    if(0 == Module[0].AddVelocityType)
                        PARTICLE.vVelocity = normalize(vRandom2 - 0.f) * fSpeed;
                    else if(1 == Module[0].AddVelocityType)
                        PARTICLE.vVelocity = normalize(vSpawnPos) * fSpeed;
                    else if(2 == Module[0].AddVelocityType)
                        PARTICLE.vVelocity = -normalize(vSpawnPos) * fSpeed;   
                    else
                        PARTICLE.vVelocity = Module[0].AddVelocityFixedDir * fSpeed;
                }
                
                PARTICLE.vLocalPos       = vSpawnPos;
                PARTICLE.vWorldPos       = PARTICLE.vLocalPos + ParticleObjectPos.xyz;     
                PARTICLE.vWorldInitScale = (Module[0].vSpawnMaxScale - Module[0].vSpawnMinScale.x) * vRandom0.x + Module[0].vSpawnMinScale;
               
                PARTICLE.vColor          = Module[0].vSpawnColor;
    
                PARTICLE.Mass            = 1.f;
                PARTICLE.Age             = 0.f;
                PARTICLE.NormalizedAge   = 0.f;
                PARTICLE.Life            = (Module[0].MaxLife - Module[0].MinLife) * vRandom1.y + Module[0].MinLife;
                PARTICLE.Active          = 1;
                
                break;
            }
            
            SpawnCount = SpawnCountBuffer[0].iSpawnCount;
        }      
    }
    else
    {   
        PARTICLE.vForce = float3(0.f, 0.f, 0.f);
        
        // Noise Force Module
        if(NoiseForce)
        {
            // 일정 시간마다 Noise Force 의 방향을 랜덤하게 구함
            if(Module[0].NoiseForceTerm <= PARTICLE.NoiseForceAccTime)
            {
                PARTICLE.NoiseForceAccTime -= Module[0].NoiseForceTerm;
                
                float3 vRandom = GetRandom(NoiseTex, _ID.x, MAX_COUNT);
                float3 vNoiseForce = normalize(vRandom.xyz - 0.5f);
                
                PARTICLE.NoiseForceDir = vNoiseForce;
            }
            
            PARTICLE.vForce += PARTICLE.NoiseForceDir * Module[0].NoiseForceScale;
            PARTICLE.NoiseForceAccTime += g_EngineDT;
        }
        
        float3 vAccel = PARTICLE.vForce / PARTICLE.Mass;
        
        // 가속도에 따른 속도의 변화
        if (PARTICLE.Life - PARTICLE.Age <= 0.5f)
        {
             // 수명이 0.5초 남았을 때 속도를 0으로 설정하여 멈추게 함
             PARTICLE.vVelocity = float3(0.f, 0.f, 0.f);
        }
        else
        {
             // 가속도에 따른 속도의 변화
             PARTICLE.vVelocity += vAccel * g_EngineDT;
        }
        
        //PARTICLE.vVelocity += vAccel * g_EngineDT;
        
        // Velocity 에 따른 이동 구현
        if(0 == Module[0].SpaceType)
        {
            // Space가 Local 이라면
            PARTICLE.vLocalPos += PARTICLE.vVelocity * g_EngineDT;
            PARTICLE.vWorldPos = PARTICLE.vLocalPos + ParticleObjectPos.xyz;
        }
        else
        {
           PARTICLE.vLocalPos += PARTICLE.vVelocity * g_EngineDT;
           PARTICLE.vWorldPos += PARTICLE.vVelocity * g_EngineDT;
        }
       
        // Scale Module 에 따른 현재 크기 계산
        PARTICLE.vWorldCurrentScale = PARTICLE.vWorldInitScale;
        
        if(ScaleModule)
            PARTICLE.vWorldCurrentScale += ((Module[0].EndScale - Module[0].StartScale) * PARTICLE.NormalizedAge + Module[0].StartScale) * PARTICLE.vWorldInitScale;
            
        if(DragModule)
        {
            if(PARTICLE.NormalizedAge < Module[0].DestNormalizedAge)
            {
                // 기울기
                // (목적 속력 - 현재 속력) / (목적 NA - 현재 NA)
                float Gradient = (Module[0].LimitSpeed - length(PARTICLE.vVelocity)) / (Module[0].DestNormalizedAge - PARTICLE.NormalizedAge);
                float NADT     = g_EngineDT / PARTICLE.Life;
                
                float NewSpeed     = length(PARTICLE.vVelocity) + (Gradient * NADT);
                PARTICLE.vVelocity = normalize(PARTICLE.vVelocity) * NewSpeed;
            }
            else
            {
                //if (Module[0].LimitSpeed < length(Particle.vVelocity))
                //{
                //    Particle.vVelocity = normalize(Particle.vVelocity) * Module[0].LimitSpeed;
                //}
            }
        }

        PARTICLE.vWorldPos += PARTICLE.vVelocity * g_EngineDT;  
        PARTICLE.Age += g_EngineDT;
        
        if (PARTICLE.Life <= PARTICLE.Age)
        {
            PARTICLE.Active = false;
        }
    }
    
    if (Render)
    {
        PARTICLE.vColor.rgb = (Module[0].EndColor - Module[0].vSpawnColor.rgb) * PARTICLE.NormalizedAge + Module[0].vSpawnColor.rgb;
        
        if(Module[0].FadeOut)
        {
            float fRatio = saturate(1.f - (PARTICLE.NormalizedAge - Module[0].StartRatio) / (1.f - Module[0].StartRatio));
            PARTICLE.vColor.a = fRatio;
        }
    }

    PARTICLE.Age += g_EngineDT;
    PARTICLE.NormalizedAge = PARTICLE.Age / PARTICLE.Life;
    
    if(PARTICLE.Life <= PARTICLE.Age)
    {
        PARTICLE.Active = 0;
    }
}

#endif