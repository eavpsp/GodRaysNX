#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H
#include <GameObject.h>

class AnimationController;
//controls animation playback
class AnimationComponent : public GameComponent//hash table for animations, anim name -> anim index
{

    public:
        int animsCount = 0;
        unsigned int animIndex = 0;
        unsigned int animCurrentFrame = 0;
        ModelAnimation *modelAnimations;
        ModelAnimation anim;
        AnimationController *animationController;//to access functionality for animation
        void Play();
        void Play(int index);
        void Play(const char* animName);
        void OnUpdate() override;
        void ComponentAddedCallback() override{
            return;
        };
        AnimationComponent(const char* filePath);
        ~AnimationComponent(){
            delete[] modelAnimations;};
};

#endif // ANIMATIONCOMPONENT_H
