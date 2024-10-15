#include <mwmath.h>
#include <random>



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

float MW_Math::lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

Vector3 MW_Math::lerp(Vector3 a, Vector3 b, float t)
{
    Vector3 results =Vector3();
    results.x = lerp(a.x, b.x, t);
    results.y = lerp(a.y, b.y, t);
    results.z = lerp(a.z, b.z, t);
    return results;
}

float MW_Math::clamp(float value, float min, float max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}
