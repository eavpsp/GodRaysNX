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
    bool contains(T boundingBox, T boundingBox2)
    {
       return boundingBox.min.x <= boundingBox2.max.x &&
              boundingBox.max.x >= boundingBox2.min.x &&
              boundingBox.min.y <= boundingBox2.max.y &&
              boundingBox.max.y >= boundingBox2.min.y &&
              boundingBox.min.z <= boundingBox2.max.z &&
              boundingBox.max.z >= boundingBox2.min.z;
    }

    template <typename T>//Rectangle
    bool overlaps(T boundingBox, T boundingBox2)
    {
        return boundingBox.min.x <= boundingBox2.max.x &&
               boundingBox.max.x >= boundingBox2.min.x &&
               boundingBox.min.y <= boundingBox2.max.y &&
               boundingBox.max.y >= boundingBox2.min.y &&
               boundingBox.min.z <= boundingBox2.max.z &&
               boundingBox.max.z >= boundingBox2.min.z;
    }
      Vector3 Min(const Vector3* vectors, int count);

     Vector3 Max(const Vector3* vectors, int count);

};

#endif /* MATH_H */
