#include <AnimationComponent.h>
#include <string.h>

void AnimationComponent::Play()
{
        animCurrentFrame = 0;
        anim = modelAnimations[animIndex];

}

void AnimationComponent::Play(int index)
{
    animIndex = index;
    animCurrentFrame = 0;
    anim = modelAnimations[animIndex];

}

void AnimationComponent::Play(const char *animName)
{
    animCurrentFrame = 0;

    for (int i = 0; i < animsCount; i++)
    {
        if (strcmp(modelAnimations[i].name, animName) == 0)
        {
            animIndex = i;
            anim = modelAnimations[animIndex];  
            break;
        }
    }
}
void AnimationComponent::OnUpdate()
{
     animCurrentFrame = (animCurrentFrame + 1)%anim.frameCount;
    UpdateModelAnimation(((GameObject*)parentObject)->objectModel, anim, animCurrentFrame);
}

AnimationComponent::AnimationComponent(const char *filePath)
{
    modelAnimations = LoadModelAnimations(filePath, &animsCount);
    Play();
}
