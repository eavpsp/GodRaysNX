#include <AnimationController.h>
#include <string.h>

void AnimationController::SetBool(int index, bool value)
{
    animData->boolsToSet[index] = value;
}

bool AnimationController::GetBool(int index)
{
    return animData->boolsToSet[index];
}

void AnimationController::SetFloat(int index, float value)
{
    animData->floatsToSet[index] = value;
}

float AnimationController::GetFloat(int index)
{
    animData->floatsToSet[index];
}

void AnimationController::ResetTrigger(int index)
{
    animData->triggersToSet[index] = false;
}

void AnimationController::SetTrigger(int index)
{
    animData->triggersToSet[index] = true;
}

bool AnimationController::GetTrigger(int index)
{
    return animData->triggersToSet[index];
}

bool AnimationController::LoadAnimationData(const char *filename)
{
            FILE *fp = fopen(filename, "rb");
            if(fp == NULL)
            {
                printf("File not found: %s", filename);
                return false;
            }

            fseek(fp, 0, SEEK_END);
            int size = ftell(fp);
            fseek(fp, 0, SEEK_SET);
            char *data = new char[size];
            fread(data, 1, size, fp);
            char header[4];
            memcpy(header, data, 4);
            if(strcmp(header, "ADGR") != 0)
            {
                animData = (AnimationControllerData *)data + sizeof(header);
                return true;
            }
}
