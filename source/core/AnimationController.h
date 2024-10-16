#ifndef ANIMATIONCONTROLLER_H
#define ANIMATIONCONTROLLER_H
#include <vector>
#include <string>
#include <AnimationComponent.h>
//Animation Controller Data, used to send data to the animator for functionality
//ex if SetBool 
struct __attribute__((packed))AnimationControllerData
{
    bool boolsToSet[9];
    bool triggersToSet[6];
    float floatsToSet[6];


    AnimationControllerData()
    {}
    ~AnimationControllerData()
    {
        delete this;
    }
};
//stores the data such as triggers, floats, and bools to control animations each animation type has a different controller
struct AnimationController
{
    //Set, Get Bools
    //Set, Get Floats
    //Set, Reset Triggers
    virtual void OnUpdate(){
        return;
    };
    void SetBool(int index, bool value);
    bool GetBool(int index);
    void SetFloat(int index, float value);
    float GetFloat(int index);
    void ResetTrigger(int index);
    void SetTrigger(int index);
    bool GetTrigger(int index);
    AnimationComponent *animationComponent;
    AnimationControllerData *data;
/**
 * @brief Initializes the AnimationController with the provided AnimationComponent and AnimationControllerData.
 *
 * This constructor sets the animationComponent and data members of the AnimationController.
 *
 * @param _animationComponent Pointer to the AnimationComponent associated with this AnimationController.
 * @param _data Pointer to the AnimationControllerData used to control animations.
 */
    AnimationController(AnimationComponent *_animationComponent, AnimationControllerData *_data)
    {
        animationComponent = _animationComponent;
        data = _data;
        animationComponent->animationController = this;
    }
    ~AnimationController()
    {
        
    }

};


#endif // ANIMATIONCONTROLLER_H
