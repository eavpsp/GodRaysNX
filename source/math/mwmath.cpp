#include <mwmath.h>
#include <random>
#include <math.h>


/**
 * \brief Generates a random floating-point number between min and max, inclusive.
 *
 * \details
 * This function uses the Mersenne Twister random number generator to generate
 * a random floating-point number between the given min and max values, inclusive.
 *
 * Example:
 * \code
 * float val = MW_Math::Random(0.0f, 1.0f);
 * \endcode
 *
 * \param min The lower bound of the range.
 * \param max The upper bound of the range.
 *
 * \return A random floating-point number between min and max, inclusive.
 */
float MW_Math::Random(float min, float max)
{
     std::random_device rd;
     static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

/**
 * \brief Generates a random integer between min and max, inclusive.
 *
 * \param min The lowest possible value.
 * \param max The highest possible value.
 *
 * \return A random integer between min and max, inclusive.
 */
int MW_Math::Random(int min, int max)
{
     std::random_device rd;
     std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return (int)dis(gen);
}


/**
 * \brief Linearly interpolate between two floats.
 *
 * \details
 * This function returns a float that is a linear interpolation between the two
 * given floats, a and b, based on the given float t. The value t should be in
 * the range [0, 1], where t = 0 returns a and t = 1 returns b.
 *
 * \param a The first float.
 * \param b The second float.
 * \param t The value to interpolate between 0 and 1.
 *
 * \return The interpolated float.
 */
float MW_Math::lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}
/**
 * \brief Linearly interpolate between two Vector3s.
 *
 * \details
 * This function returns a Vector3 that is a linear interpolation between the two
 * given Vector3s, a and b, based on the given float t. The value t should be in
 * the range [0, 1], where t = 0 returns a and t = 1 returns b.
 *
 * \param a The first Vector3.
 * \param b The second Vector3.
 * \param t The value to interpolate between 0 and 1.
 *
 * \return The interpolated Vector3.
 */

Vector3 MW_Math::lerp(Vector3 a, Vector3 b, float t)
{
    Vector3 results =Vector3();
    results.x = lerp(a.x, b.x, t);
    results.y = lerp(a.y, b.y, t);
    results.z = lerp(a.z, b.z, t);
    return results;
}

/**
 * \brief Clamp a float value between a minimum and maximum value.
 *
 * \details
 * This function takes a float value and returns it if it is within the range
 * [min, max]. If the value is below min, it returns min. If the value is above
 * max, it returns max.
 *
 * \param value The float to clamp.
 * \param min The minimum value.
 * \param max The maximum value.
 *
 * \return The clamped float value.
 */
float MW_Math::clamp(float value, float min, float max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

/**
 * \brief Returns a float value between two given floats at a given distance.
 *
 * \details
 * This function returns a float that is between the two given floats, a and b,
 * at the given distance. The distance is a float that should be in the range
 * [0, 1], where 0 is equal to a and 1 is equal to b.
 *
 * \param a The first float.
 * \param b The second float.
 * \param distance The distance between a and b, in the range [0, 1].
 *
 * \return The float value between a and b at the given distance.
 */
float MW_Math::between(float a, float b, float distance)
{
    return a + (b-a) * distance;
}

/**
 * \brief Returns the absolute difference between two float values.
 *
 * \param a The first float.
 * \param b The second float.
 *
 * \return The absolute difference between a and b.
 */
float MW_Math::distance(float a, float b)
{
    return abs(b - a);
}

Vector3 MW_Math::Min(const Vector3 *vectors, int count)
{
        Vector3 minVector = vectors[0];
        for (int i = 1; i < count; i++) {
            if (vectors[i].x < minVector.x) minVector.x = vectors[i].x;
            if (vectors[i].y < minVector.y) minVector.y = vectors[i].y;
            if (vectors[i].z < minVector.z) minVector.z = vectors[i].z;
        }
        return minVector;
}

Vector3 MW_Math::Max(const Vector3 *vectors, int count)
{
        Vector3 maxVector = vectors[0];
        for (int i = 1; i < count; i++) {
            if (vectors[i].x > maxVector.x) maxVector.x = vectors[i].x;
            if (vectors[i].y > maxVector.y) maxVector.y = vectors[i].y;
            if (vectors[i].z > maxVector.z) maxVector.z = vectors[i].z;
        }
        return maxVector;
}