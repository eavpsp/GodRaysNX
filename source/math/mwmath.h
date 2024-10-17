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
   

    template <typename T>//Rectangle
    bool contains(T rect, T rect2)
    {
       return rect.x <= rect2.x && rect.y <= rect2.y && rect.x + rect.width >= rect2.x + rect2.width && rect.y + rect.height >= rect2.y + rect2.height;
    }

    template <typename T>//Rectangle
    bool overlaps(T rect, T rect2)
    {
        return rect.x < rect2.x + rect2.width && rect.x + rect.width > rect2.x && rect.y < rect2.y + rect2.height && rect.y + rect.height > rect2.y;
    }

};

#endif /* MATH_H */
