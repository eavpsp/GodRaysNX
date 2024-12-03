#include <AnimationComponent.h>
#include <string.h>
#include <GR_MeshComponent.h>
#include <Texture2DComponent.h>
void AnimationComponent::AddAnimationEvent(std::string animName,float frame, AnimationEventDelegate event)
{
    animationEvents[animName][frame] = event;
}
void AnimationComponent::RemoveAnimationEvent(std::string animName,float frame, AnimationEventDelegate event)
{
    animationEvents[animName].erase(frame);
}
void AnimationComponent::Play()
{
    animCurrentFrame = 0;
}

void AnimationComponent::Play(int index)
{
    if(animIndex == index)
    {
        return;
    }
    animIndex = index;
    animCurrentFrame = 0;
}

void AnimationComponent::Play(const char *animName)
{
    if (strcmp(modelAnimations[animIndex].name, animName) == 0)
    {
        return;
    }

    animCurrentFrame = 0;

    for (int i = 0; i < animsCount; i++)
    {
        if (strcmp(modelAnimations[i].name, animName) == 0)
        {
            animIndex = i;
            break;
        }
    }
}
void AnimationComponent::OnUpdate()//frame time consideration
{
    
    if(modelAnimations == nullptr)
    {
        debugLog("No animations found");
        return;
    
    }
    animCurrentFrame++;

    if (animCurrentFrame >= modelAnimations[animIndex].frameCount) animCurrentFrame = 0;
    if(animationEvents[modelAnimations[animIndex].name].find(animCurrentFrame) != animationEvents[modelAnimations[animIndex].name].end())
    {
        animationEvents[modelAnimations[animIndex].name][animCurrentFrame]();
    }
    UpdateModelAnimation(((GameObject*)parentObject)->GetComponent<GR_MeshComponent>()->mesh->model, modelAnimations[animIndex], animCurrentFrame);
}


AnimationComponent::AnimationComponent(const char *filePath)
{
    modelAnimations = LoadModelAnimations(filePath, &animsCount);   
    Play();
    
}

void SpriteAnimationComponent::AddAnimationEvent(std::string animName,float frame, AnimationEventDelegate event)
{
    animationEvents[animName][frame] = event;
}

void SpriteAnimationComponent::RemoveAnimationEvent(std::string animName,float frame, AnimationEventDelegate event)
{
    animationEvents[animName].erase(frame);
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
    if(animationEvents[currentAnimation.name].find(animCurrentFrame) != animationEvents[currentAnimation.name].end())
    {
        animationEvents[currentAnimation.name][animCurrentFrame]();
    }
    //update texture component
    Texture2DComponent* texComp = ((GameObject*)parentObject)->GetComponent<Texture2DComponent>();
    if(texComp != nullptr)
    {
        texComp->SetTexture(currentAnimation.animationFrames[animCurrentFrame]);
    }
}
