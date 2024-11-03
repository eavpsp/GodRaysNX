#include <AnimationSkeletonManager.h>

void AnimationSkeletonManager::AttachBones(AnimationSkeletonAttachment attachment)
{
    for (size_t i = 0; i < modelAnimation->boneCount; i++)
    {
    
        if(modelAnimation[0].bones[i].name == attachment.boneName)
        {
            objAttachments->at(&modelAnimation[0].framePoses[i]) = attachment;
        }
    
    }
    
}

void AnimationSkeletonManager::Update()
{
    for (auto & attachment : *objAttachments)
    {
       attachment.second.boneObject->position = Vector3Add((**attachment.first).translation, attachment.second.boneObject->position);
       attachment.second.boneObject->rotation = QuaternionAdd((**attachment.first).rotation, attachment.second.boneObject->rotation);
    }
    
}
