/*----------------------------------------------------------
* Author: Connor Hitchcock
*
* The class for a cone object.
* A subclass of Object, implements intersect() and normal().
-------------------------------------------------------------*/

#include "Cone.h"
#include <math.h>

/**
 * Cone's intersection method.  The input is a ray
 */
float Cone::intersect(glm::vec3 p0, glm::vec3 dir)
{
    glm::vec3 dist = p0 - center;
    float vertical = height - p0.y + center.y;
    float s = (radius / height) * (radius / height);
    float a = (dir.x * dir.x) + (dir.z * dir.z) - (s * dir.y * dir.y);
    float b = 2 * ((dist.x * dir.x) + (dist.z * dir.z) + (s * vertical * dir.y));
    float c = (dist.x * dist.x) + (dist.z * dist.z) - (s * vertical * vertical);
    float delta = (b * b) - (4 * a * c);

    if (fabs(delta) < 0.0015) return -1.0;

    float tiny;
    float large;
    float u1 = (-b - sqrt(delta)) / (2 * a);
    float u2 = (-b + sqrt(delta)) / (2 * a);

    if (u1 < 0.01) u1 = -1;
    if (u2 < 0.01) u2 = -1;

    if (u1 > u2)
    {
        tiny = u2;
        large = u1;
    }
    else
    {
        tiny = u1;
        large = u2;
    }

    float yp0 = p0.y + (dir.y * tiny);
    if ((yp0 <= center.y + height) && (yp0 >= center.y))
    {
        return tiny;
    }
    else
    {
        float yp0 = p0.y + (dir.y * large);
        if ((yp0 <= center.y + height) && (yp0 >= center.y))
        {
            return large;
        }
        else
        {
            return -1.0;
        }
    }
}

/**
 * Returns the unit normal vector at a given point.
 * Assumption: The input point p lies on the cone.
 */
glm::vec3 Cone::normal(glm::vec3 p)
{
    glm::vec3 dist = p - center;
    float v = sqrt((dist.x * dist.x) + (dist.z * dist.z));
    glm::vec3 n = glm::vec3 (dist.x, v * (radius / height), dist.z);
    n = glm::normalize(n);
    return n;
}
