#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H
#include <GameObject.h>
#include <string>
#include <GameOverlays.h>
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
struct SpriteAnimation
{
    std::string name;
    int totalFrames = 0;
    int playBackSpeed = 60;
    int currentFrame = 0;
    float frameTime = 0;
    Texture2D *animationFrames;
};

struct SpriteAnimationComponent : public GameComponent
{
    int animsCount = 0;
    unsigned int animIndex = 0;
    unsigned int animCurrentFrame = 0;
    SpriteAnimation *spriteAnimations;
    SpriteAnimation currentAnimation;
    AnimationController *animationController;//to access functionality for animation
    Image2D baseImage;//updated
    void Play();
    void Play(int index);
    void Play(const char* animName);
    void OnUpdate() override;
    SpriteAnimationComponent(const char* filePath);
    ~SpriteAnimationComponent()
    {
        delete[] spriteAnimations;
    };
};
#endif // ANIMATIONCOMPONENT_H
