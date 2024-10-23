#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H
#include<Entity.h>
#include <RenderSystem.h>
#include <Raylib.h>
//Takes in int of Entities to render, adds Particle Component and Transform Component
//Has update function to alter particles
//must be plugged in custom shader program slot in render system
enum ParticleType
{
    P_SPHERE = 0x01,
    P_PLANE = 0x02, 
    P_CUBE = 0x03,
    P_BILLBOARD = 0x04
};

struct __attribute__((packed)) ParticleComponent
{
    ParticleType particleType;
    Vector3 position;
    Vector3 startPos;
    Vector3 velocity;
    float scale;
    float lifeTime;
    bool init;
};
struct ParticleSystem//parent class
{
    public:
        std::map<Entity, ParticleComponent> particles;
        int numberOfParticles = 0;
        Shader *shaderToUse; //shader to use for rendering, if null, will use default
        virtual void Update() = 0;
        virtual void Draw() = 0;
        void AddComponent(Entity entity, ParticleType particleType);
        void RemoveComponent(Entity entity);
        ParticleComponent GetComponent(Entity entity);
        ParticleSystem(){};
        virtual ~ParticleSystem() = default;
};

//sample burst particle system
struct BurstParticleSystem : public ParticleSystem
{
    public:
        int maxDistance;
        Vector3 position;
        BurstParticleSystem(int particleCount, Vector3 position, int maxDistance = 10);
        
        void Draw() override;
        void Update() override;
        virtual ~BurstParticleSystem() = default;
};

#endif // PARTICLE_SYSTEM_H
