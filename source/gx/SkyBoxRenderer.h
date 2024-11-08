#ifndef SKYBOXRENDERER_H
#define SKYBOXRENDERER_H
#include <raylib.h>
#include <ResourceManager.h>
#include <rlgl.h>
#include <raymath.h>
#include <map>
#include <string>
#include <vector>
#include <GameObject.h>
#include <GR_MeshComponent.h>

class SkyBoxObject : public GameObject
{
    public:
    void onUpdate() override;

    SkyBoxObject();
   

    void SetPosition(Vector3 position)
    {
        this->position = position;
    }
    ~SkyBoxObject()
    {
       
    }
};


#endif // SKYBOXRENDERER_H
