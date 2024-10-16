#ifndef MATH_H
#define MATH_H

#include <raylib.h>
namespace MW_Math
{
    //float lerp(float a, float b, float t);
    float Random(float min, float max);
    int Random(int min, int max);
    float lerp(float a, float b, float t);
    Vector3 lerp(Vector3 a, Vector3 b, float t);
    float clamp(float value, float min, float max);
    float between(float a, float b, float t);
    float distance(float a, float b);
};


#endif /* MATH_H */
