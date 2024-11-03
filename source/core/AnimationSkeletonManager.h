#ifndef ANIMATIONSKELETONMANAGER_H
#define ANIMATIONSKELETONMANAGER_H
#include <GameObject.h>
#include <AnimationComponent.h>
#include <map>
//uses raylibs m3d implementaion to attach objects to bones and control weight and position of bones to enforce animation IK

//anims[animId].framePoses[animFrameCounter][i].translation Gets bone at index
//anims[animId].bones[animFrameCounter][i].name Gets the bone name at index
struct AnimationSkeletonAttachment
{
    std::string boneName;
    GameObject *boneObject;
};
struct AnimationSkeletonManager
{
    //ref the gameobject that i
    std::map<Transform **, AnimationSkeletonAttachment> *objAttachments;
    //ref to the animation
    ModelAnimation *modelAnimation;
    void AttachBones(AnimationSkeletonAttachment attachment);
    void Update();
};

#endif