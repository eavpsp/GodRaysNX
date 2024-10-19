#include <ParticleSystem.h>
#include <raymath.h>
#include <mwmath.h>

void ParticleSystem::AddComponent(Entity entity, ParticleType particleType)
{
    ParticleComponent component;
    component.particleType = particleType;
    component.position = Vector3Zero();
    component.startPos = Vector3Zero();
    component.velocity = Vector3Zero();
    component.scale = 1.0f;
    particles[entity] = component;
    
}

void ParticleSystem::RemoveComponent(Entity entity)
{
   particles.erase(entity);
}

ParticleComponent ParticleSystem::GetComponent(Entity entity)
{
    return particles[entity];
}

void BurstParticleSystem::Draw()
{
    Update();
    for(auto& particle : particles)
    {
        DrawSphere(particle.second.position, particle.second.scale, RED);//texture pos scale tint
    }
}

void BurstParticleSystem::Update()
{
    for(auto& particle : particles)
    {
        if(!particle.second.init)
        {
            particle.second.init = true;
            //set random velocity
            particle.second.velocity = Vector3{MW_Math::Random(-10.0f,10.0f), MW_Math::Random(-10.0f,10.0f), MW_Math::Random(-10.0f,10.0f)};
        }
        float deltaTime = GetFrameTime();
        particle.second.position = Vector3Add(particle.second.position, Vector3Scale(particle.second.velocity, deltaTime));
        float distance = Vector3Distance(particle.second.position, particle.second.startPos);
        if(distance > maxDistance)
        {
            particle.second.position = particle.second.startPos;
            particle.second.init = false;
        }
    }
}
