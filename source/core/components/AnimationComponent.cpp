#include <AnimationComponent.h>
#include <string.h>
#include <GR_MeshComponent.h>
#include <Texture2DComponent.h>
void AnimationComponent::AddAnimationEvent(float frame, AnimationEventDelegate event)
{
    animationEvents[frame] = event;
}
void AnimationComponent::RemoveAnimationEvent(float frame, AnimationEventDelegate event)
{
    animationEvents.erase(frame);
}
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
void AnimationComponent::OnUpdate()//frame time consideration
{
    animCurrentFrame = (animCurrentFrame + 1)%anim.frameCount;
    if(animationEvents.find(animCurrentFrame) != animationEvents.end())
    {
        animationEvents[animCurrentFrame]();
    }
    UpdateModelAnimation(((GameObject*)parentObject)->GetComponent<GR_MeshComponent>()->mesh->model, anim, animCurrentFrame);
}

AnimationComponent::AnimationComponent(const char *filePath)
{
    modelAnimations = LoadModelAnimations(filePath, &animsCount);
    Play();
}

void SpriteAnimationComponent::AddAnimationEvent(float frame, AnimationEventDelegate event)
{
    animationEvents[frame] = event;
}

void SpriteAnimationComponent::RemoveAnimationEvent(float frame, AnimationEventDelegate event)
{
    animationEvents.erase(frame);
}

void SpriteAnimationComponent::Play()
{
    animCurrentFrame = 0;
    currentAnimation = spriteAnimations[animIndex];
}

void SpriteAnimationComponent::Play(int index)
{
    animIndex = index;
    animCurrentFrame = 0;
    currentAnimation = spriteAnimations[animIndex];
}

void SpriteAnimationComponent::Play(const char *animName)
{
    for (size_t i = 0; i < animsCount; i++)
    {
        if (spriteAnimations[i].name == animName)
        {
            Play(i);
            break;
        }
        
    }
    
}

void SpriteAnimationComponent::OnUpdate()
{
    animCurrentFrame = (animCurrentFrame + 1)%currentAnimation.totalFrames;
    if(animationEvents.find(animCurrentFrame) != animationEvents.end())
    {
        animationEvents[animCurrentFrame]();
    }
    //update texture component
    Texture2DComponent* texComp = ((GameObject*)parentObject)->GetComponent<Texture2DComponent>();
    if(texComp != nullptr)
    {
        texComp->SetTexture(&currentAnimation.animationFrames[animCurrentFrame]);
    }
}
