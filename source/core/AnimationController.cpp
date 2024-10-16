#include <AnimationController.h>

void AnimationController::SetBool(int index, bool value)
{
    data->boolsToSet[index] = value;
}

bool AnimationController::GetBool(int index)
{
    return data->boolsToSet[index];
}

void AnimationController::SetFloat(int index, float value)
{
    data->floatsToSet[index] = value;
}

float AnimationController::GetFloat(int index)
{
    data->floatsToSet[index];
}

void AnimationController::ResetTrigger(int index)
{
    data->triggersToSet[index] = false;
}

void AnimationController::SetTrigger(int index)
{
    data->triggersToSet[index] = true;
}

bool AnimationController::GetTrigger(int index)
{
    return data->triggersToSet[index];
}
