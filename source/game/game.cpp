#include <game.h>
#include <GameManager.h>
#include "RenderSystem.h"
#include <PhysicsComponent.h>

extern RenderSystem *renderSystem;
extern GameManager *gameManager;

//PROCS TO RUN//
void TestPhysicsBullet()//Sets up our game scene
{
    Texture2D texture = LoadTexture("romfs:/textures/nosignal.png");
    GameObject *plane = GameObject::InstantiateGameObject<GameObject>(Vector3{0,-5,0}, Quaternion{0,0,0,0},20.0f);
    GR_Mesh* planeData = new GR_Mesh(LoadModel("romfs:/models/prim/plane.obj"));
    GR_MeshComponent *planeMesh = new GR_MeshComponent(planeData);
    planeMesh->SetShader(renderSystem->defaultShader);
    planeMesh->SetTint(RED);
    plane->AddComponent(planeMesh);
    BulletPhysicsComponent *planeBody = new BulletPhysicsComponent(plane->position, plane->rotation, 0.0f, new btBoxShape(btVector3(20.0f,0.5f,20.0f)));      
    plane->AddComponent(planeBody);
    
    GameObject *slope = GameObject::InstantiateGameObject<GameObject>(Vector3{0,0,0}, Quaternion{0,180,0,0}, 2.0f);
    GR_Mesh* slopeData = new GR_Mesh(LoadModel("romfs:/models/prim/cube.obj"));
    GR_MeshComponent *slopeMesh = new GR_MeshComponent(slopeData);
    slopeMesh->SetShader(renderSystem->defaultShader);
    slopeMesh->SetTint(GREEN);
    slope->AddComponent(slopeMesh);
    BulletPhysicsComponent *slopeBody = new BulletPhysicsComponent(slope->position, slope->rotation, 0.0f, new btBoxShape(btVector3(10.0f,2.0f,10.0f)));      
    slope->AddComponent(slopeBody);
   
    GameObject *meshObject = GameObject::InstantiateGameObject<GameObject>(Vector3{0,10,0}, Quaternion{0,0,0,0}, 2.0f);
    GR_Mesh* meshData = new GR_Mesh(LoadModel("romfs:/models/prim/sphere.obj"));
    GR_MeshComponent *mesh = new GR_MeshComponent(meshData);
    mesh->SetShader(renderSystem->defaultShader);
    mesh->SetTexture(texture);
    meshObject->AddComponent(mesh);
    BulletPhysicsComponent *meshBody = new BulletPhysicsComponent(meshObject->position, meshObject->rotation, 1.0f, new btSphereShape(1.2f));      
    meshObject->AddComponent(meshBody);

}  
void initGame()
{
  TestPhysicsBullet();
}